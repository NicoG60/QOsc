#include <qoscinterface.h>
#include <QBuffer>
#include <QNetworkDatagram>
#include <QTimer>
#include <QNetworkInterface>
#include <QUdpSocket>

class QOscInterfacePrivate
{
    Q_DECLARE_PUBLIC(QOscInterface);

public:
    QOscInterfacePrivate() = default;
    ~QOscInterfacePrivate() = default;

    void rebind();
    void updateLocalAddr();
    void setLocalAddr(const QString& addr);
    void send(const QByteArray& data);

    void readReady();

    void processMessage(const QOscMessage& msg);

    void processBundle(const QOscBundle& b);
    void executeBundle(const QOscBundle& b);

    QOscInterface* q_ptr = nullptr;

    QHostAddress remoteAddr = QHostAddress("127.0.0.1");
    quint16      remotePort = 0;
    QHostAddress localAddr  = QHostAddress("127.0.0.1");
    quint16      localPort  = 0;
    QUdpSocket   socket;

    bool isListening = false;

    QList<QOscMethod::ptr> methods;
};

void QOscInterfacePrivate::rebind()
{
    if(socket.isValid())
    {
        socket.disconnectFromHost();

        if(socket.state() != QAbstractSocket::UnconnectedState)
            socket.waitForDisconnected();
    }

    bool state = socket.bind(localPort);

    if(state != isListening)
    {
        isListening = state;
        emit q_func()->isListeningChanged();
    }

    if(socket.localPort() != localPort)
    {
        localPort = socket.localPort();
        emit q_func()->localPortChanged(localPort);
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
            QString addr = QStringLiteral("%1/%2").arg(entry.ip().toString(), entry.netmask().toString());
            auto p = QHostAddress::parseSubnet(addr);

            if(remoteAddr.isInSubnet(p))
            {
                setLocalAddr(entry.ip().toString());
                return;
            }
        }
    }
}

void QOscInterfacePrivate::setLocalAddr(const QString& addr)
{
    QHostAddress hostAddr(addr);
    if(!hostAddr.isNull() && !hostAddr.isEqual(localAddr))
    {
        localAddr = hostAddr;
        emit q_func()->localAddrChanged(addr);
    }
}

void QOscInterfacePrivate::send(const QByteArray& data)
{
    socket.writeDatagram(data, remoteAddr, remotePort);
    emit q_func()->messageSent();
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
                emit q->messageReceived(msg);
            }
            break;
        }

        case QOsc::OscBundle:
        {
            auto bundle = QOscBundle::read(data);
            if(bundle.isValid())
            {
                processBundle(bundle);
                emit q->bundleReceived(bundle);
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

            QTimer::singleShot(ms, q_func(),
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
    QObject(parent),
    d_ptr(new QOscInterfacePrivate())
{
    Q_D(QOscInterface);
    d->q_ptr = this;
    QObject::connect(&d->socket, &QAbstractSocket::readyRead,
                     this,       &QOscInterface::readReady);
    d->rebind();
}

QOscInterface::~QOscInterface()
{

}

QString QOscInterface::remoteAddr() const
{
    return d_func()->remoteAddr.toString();
}

void QOscInterface::setRemoteAddr(const QString& addr)
{
    Q_D(QOscInterface);

    QHostAddress hostAddr(addr);

    if(hostAddr != d->remoteAddr)
    {
        d->remoteAddr = hostAddr;
        emit remoteAddrChanged(addr);
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
        emit remotePortChanged(p);
    }
}

QString QOscInterface::localAddr() const
{
    return d_func()->localAddr.toString();
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
        emit localPortChanged(p);
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
    if(!m.isValid())
        return;

    d_func()->send(m.package());
}

void QOscInterface::send(const QOscBundle& b)
{
    if(!b.isValid())
        return;

    d_func()->send(b.package());
}

void QOscInterface::connect(const QOscMethod::ptr& method)
{
    d_func()->methods.append(method);
}

void QOscInterface::readReady()
{
    d_func()->readReady();
}



