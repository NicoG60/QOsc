#include <qoscinterface.h>
#include <QBuffer>
#include <QNetworkDatagram>
#include <QTimer>

QOSCInterface::QOSCInterface(QObject* parent) :
    QObject(parent),
    _remoteAddr("127.0.0.1"),
    _remotePort(0),
    _localAddr("127.0.0.1"),
    _localPort(0)
{
    rebind();
}

void QOSCInterface::setRemoteAddr(const QHostAddress& addr)
{
    if(addr != _remoteAddr)
    {
        _remoteAddr = addr;
        remoteAddrchanged(addr);
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
        remotePortChanged(p);
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
    _isListening = _s.bind(_localPort);
    if(_s.localPort() != _localPort)
    {
        _localPort = _s.localPort();
        remotePortChanged(_s.localPort());
    }
}

void QOSCInterface::updateLocalAddr()
{
    setLocalAddr(_s.localAddress());
}

void QOSCInterface::setLocalAddr(const QHostAddress& addr)
{
    if(addr != _localAddr)
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
    }
}

void QOSCInterface::processPacket(const QOSCPacket::ptr& p, QOSCTimeTag *time)
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

void QOSCInterface::processBundle(const QOSCBundle::ptr& b, QOSCTimeTag* time)
{
    QOSCTimeTag* time_ = time ? time : &b->time;

    if(time_->isNow())
    {
        for(auto& e : b->elements)
            processPacket(e, time_);
    }
    else
    {
        QDateTime dt = time_->toDateTime();
        qint64 ms = dt.toMSecsSinceEpoch();
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        QTimer::singleShot(ms - now,
        [=]()
        {
            QOSCTimeTag tmp(quint64(1));
            processBundle(b, &tmp);
        });
    }
}
