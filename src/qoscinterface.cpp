#include <qoscinterface.h>
#include <private/qobject_p.h>
#include <QBuffer>
#include <QNetworkDatagram>
#include <QTimer>
#include <QNetworkInterface>
#include <QUdpSocket>

class QOscInterfacePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QOscInterface);

public:
    QOscInterfacePrivate();

    void rebind();
    void updateLocalAddr();
    void setLocalAddr(const QHostAddress& addr);
    void send(const QByteArray& data);

    void readReady();

    void processMessage(const QOscMessage& msg);

    void processBundle(const QOscBundle& b);
    void executeBundle(const QOscBundle& b);

    QHostAddress remoteAddr;
    quint16      remotePort;
    QHostAddress localAddr;
    quint16      localPort;
    QUdpSocket   socket;

    bool isListening;

    QList<QOscMethod::ptr> methods;
};

QOscInterfacePrivate::QOscInterfacePrivate() :
    QObjectPrivate(),
    remoteAddr("127.0.0.1"),
    remotePort(0),
    localAddr("127.0.0.1"),
    localPort(0),
    isListening(false)
{
}

void QOscInterfacePrivate::rebind()
{
    if(socket.isValid())
    {
        socket.disconnectFromHost();

        if(socket.state() != QAbstractSocket::UnconnectedState)
            socket.waitForDisconnected();
    }

    isListening = socket.bind(localPort);

    if(socket.localPort() != localPort)
    {
        localPort = socket.localPort();
        q_func()->localPortChanged(localPort);
    }
}

void QOscInterfacePrivate::updateLocalAddr()
{
    for(auto& iface : QNetworkInterface::allInterfaces())
    {
        if(!iface.flags().testFlag(QNetworkInterface::IsUp))
            continue;

        for(auto& entry : iface.addressEntries())
        {
            QString addr = QStringLiteral("%1/%2").arg(entry.ip().toString()).arg(entry.netmask().toString());
            auto p = QHostAddress::parseSubnet(addr);

            if(remoteAddr.isInSubnet(p))
            {
                setLocalAddr(entry.ip());
                return;
            }
        }
    }
}

void QOscInterfacePrivate::setLocalAddr(const QHostAddress& addr)
{
    if(!addr.isNull() && !addr.isEqual(localAddr))
    {
        localAddr = addr;
        q_func()->localAddrChanged(addr);
    }
}

void QOscInterfacePrivate::send(const QByteArray& data)
{
    socket.writeDatagram(data, remoteAddr, remotePort);
}

void QOscInterfacePrivate::readReady()
{
    Q_Q(QOscInterface);

    while(socket.hasPendingDatagrams())
    {
        auto datagram = socket.receiveDatagram();
        auto data = datagram.data();

        switch(QOsc::detectType(data))
        {
        case QOsc::OscMessage:
        {
            auto msg = QOscMessage::read(data);
            if(msg.isValid())
            {
                processMessage(msg);
                q->messageReceived(msg);
            }
            break;
        }

        case QOsc::OscBundle:
        {
            auto bundle = QOscBundle::read(data);
            if(bundle.isValid())
            {
                processBundle(bundle);
                q->bundleReceived(bundle);
            }
            break;
        }

        default:
            break;
        }
    }
}

void QOscInterfacePrivate::processMessage(const QOscMessage& msg)
{
    for(auto& m : methods)
    {
        if(msg.match(m->addr))
            m->call(msg);
    }
}

void QOscInterfacePrivate::processBundle(const QOscBundle& b)
{
    auto t = b.time();

    if(t.isNow())
        executeBundle(b);
    else
    {
        qint64 ms = t.toDateTime().toMSecsSinceEpoch();
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        if(ms <= now)
            executeBundle(b);
        else
        {
            auto b2 = b;
            b2.setTime(QOscValue::asap());

            ms -= now;

            QTimer::singleShot(ms,
            [this, b2]()
            {
                processBundle(b2);
            });
        }
    }
}

void QOscInterfacePrivate::executeBundle(const QOscBundle& b)
{
    for(auto& e : b)
        processMessage(e);
}



// =============================================================================




QOscInterface::QOscInterface(QObject* parent) :
    QObject(*new QOscInterfacePrivate(), parent)
{
    Q_D(QOscInterface);
    QObject::connect(&d->socket, &QAbstractSocket::readyRead,
                     this,       &QOscInterface::readReady);
    d->rebind();
}

QHostAddress QOscInterface::remoteAddr() const
{
    return d_func()->remoteAddr;
}

void QOscInterface::setRemoteAddr(const QHostAddress& addr)
{
    Q_D(QOscInterface);

    if(addr != d->remoteAddr)
    {
        d->remoteAddr = addr;
        remoteAddrChanged(addr);
        d->updateLocalAddr();
    }
}

quint16 QOscInterface::remotePort() const
{
    return d_func()->remotePort;
}

void QOscInterface::setRemotePort(quint16 p)
{
    Q_D(QOscInterface);

    if(p != d->remotePort)
    {
        d->remotePort = p;
        remotePortChanged(p);
    }
}

QHostAddress QOscInterface::localAddr() const
{
    return d_func()->localAddr;
}

quint16 QOscInterface::localPort() const
{
    return d_func()->localPort;
}

void QOscInterface::setLocalPort(quint16 p)
{
    Q_D(QOscInterface);

    if(p != d->localPort)
    {
        d->localPort = p;
        localPortChanged(p);
        d->rebind();
    }
}

bool QOscInterface::isListening() const
{
    return d_func()->isListening;
}

void QOscInterface::connect(const QString& addr, QObject* obj, const char* slot)
{
    connect(QOscMethod::ptr(new QOscSlotMethod(addr, obj, slot)));
}

void QOscInterface::disconnect()
{
    d_func()->methods.clear();
}

void QOscInterface::disconnect(const QString& addr)
{
    Q_D(QOscInterface);

    for(auto it = d->methods.begin(); it != d->methods.end();)
    {
        if((*it)->addr == addr)
            it = d->methods.erase(it);
        else
            ++it;
    }
}

void QOscInterface::send(const QOscMessage& m)
{
    d_func()->send(m.package());
}

void QOscInterface::send(const QOscBundle& b)
{
    d_func()->send(b.package());
}

void QOscInterface::connect(QOscMethod::ptr method)
{
    d_func()->methods.append(method);
}

void QOscInterface::readReady()
{
    d_func()->readReady();
}



