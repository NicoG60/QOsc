#include <qoscinterface.h>
#include <QBuffer>
#include <QNetworkDatagram>
#include <QTimer>
#include <QNetworkInterface>

QOSCInterface::QOSCInterface(QObject* parent) :
    QObject(parent),
    _remoteAddr("127.0.0.1"),
    _remotePort(0),
    _localAddr("127.0.0.1"),
    _localPort(0)
{
    QObject::connect(&_s, &QAbstractSocket::readyRead, this, &QOSCInterface::readReady);
    rebind();
}

void QOSCInterface::setRemoteAddr(const QHostAddress& addr)
{
    if(addr != _remoteAddr)
    {
        _remoteAddr = addr;
        remoteAddrChanged(addr);
        updateLocalAddr();
    }
}

void QOSCInterface::setRemotePort(quint16 p)
{
    if(p != _remotePort)
    {
        _remotePort = p;
        remotePortChanged(p);
    }
}

void QOSCInterface::setLocalPort(quint16 p)
{
    if(p != _localPort)
    {
        _localPort = p;
        localPortChanged(p);
        rebind();
    }
}

void QOSCInterface::connect(const QString& addr, QObject* obj, const char* slot)
{
    auto ptr = new QOSCSlotMethod(addr, obj, slot);
    _methods.append(QOSCMethod::ptr(ptr));
}

void QOSCInterface::disconnect()
{
    _methods.clear();
}

void QOSCInterface::disconnect(const QString& addr)
{
    for(auto it = _methods.begin(); it != _methods.end();)
    {
        if((*it)->addr == addr)
            it = _methods.erase(it);
        else
            ++it;
    }
}

void QOSCInterface::send(const QOSCPacket::ptr p)
{
    send(p->package());
}

void QOSCInterface::send(const QOSCMessage& m)
{
    send(m.package());
}

void QOSCInterface::send(const QOSCBundle& b)
{
    send(b.package());
}

void QOSCInterface::rebind()
{
    if(_s.isValid())
    {
        _s.disconnectFromHost();

        if(_s.state() != QAbstractSocket::UnconnectedState)
            _s.waitForDisconnected();
    }

    _isListening = _s.bind(_localPort);

    if(_s.localPort() != _localPort)
    {
        _localPort = _s.localPort();
        localPortChanged(_s.localPort());
    }
}

void QOSCInterface::updateLocalAddr()
{
    for(auto& iface : QNetworkInterface::allInterfaces())
    {
        if(!iface.flags().testFlag(QNetworkInterface::IsUp))
            continue;

        for(auto& entry : iface.addressEntries())
        {
            QString addr = QStringLiteral("%1/%2").arg(entry.ip().toString()).arg(entry.netmask().toString());
            auto p = QHostAddress::parseSubnet(addr);

            if(_remoteAddr.isInSubnet(p))
            {
                setLocalAddr(entry.ip());
                return;
            }
        }
    }
}

void QOSCInterface::setLocalAddr(const QHostAddress& addr)
{
    if(!addr.isNull() && !addr.isEqual(_localAddr))
    {
        _localAddr = addr;
        localAddrChanged(addr);
    }
}

void QOSCInterface::send(const QByteArray& data)
{
    _s.writeDatagram(data, _remoteAddr, _remotePort);
}

void QOSCInterface::readReady()
{
    while(_s.hasPendingDatagrams())
    {
        auto datagram = _s.receiveDatagram();
        auto packet = QOSCPacket::read(datagram.data());

        processPacket(packet);
        emit packetReceived(packet);
    }
}

void QOSCInterface::processPacket(const QOSCPacket::ptr& p, const QOSCTimeTag *time)
{
    if(!p->isValid())
        return;

    switch(p->type)
    {
    case QOSCPacket::OSCMessage:
        processMessage(p.dynamicCast<QOSCMessage>());
        break;

    case QOSCPacket::OSCBundle:
        processBundle(p.dynamicCast<QOSCBundle>(), time);
        break;

    default:
        break;
    }
}

void QOSCInterface::processMessage(const QOSCMessage::ptr& msg)
{
    for(auto& m : _methods)
    {
        if(msg->match(m->addr))
            m->call(msg);
    }
}

void QOSCInterface::processBundle(const QOSCBundle::ptr& b, const QOSCTimeTag* time)
{
    const QOSCTimeTag* t = time ? time : &b->time;

    if(t->isNow())
        executeBundle(b, t);
    else
    {
        qint64 ms = t->toDateTime().toMSecsSinceEpoch();
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        if(ms <= now)
            executeBundle(b, &QOSCTimeTag::asap());
        else
        {
            ms -= now;
            QTimer::singleShot(ms,
            [=]()
            {
                processBundle(b, &QOSCTimeTag::asap());
            });
        }
    }
}

void QOSCInterface::executeBundle(const QOSCBundle::ptr& b, const QOSCTimeTag *time)
{
    for(auto& e : b->elements)
        processPacket(e, time);
}
