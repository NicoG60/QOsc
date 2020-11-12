#include <qoscpacket.h>
#include <qoscbundle.h>
#include <qoscmessage.h>
#include <QBuffer>

QByteArray QOSCPacket::package() const
{
    QByteArray data;
    QBuffer buf(&data);

    buf.open(QIODevice::WriteOnly);

    write(&buf);

    buf.close();

    return data;
}

QOSCPacket::Type QOSCPacket::detectType(const QByteArray& data)
{
    if(data.isEmpty())
        return InvalidPacket;

    switch(data[0])
    {
    case '/':
        return OSCMessage;

    case '#':
        return OSCBundle;

    default:
        return InvalidPacket;
    }
}

QOSCPacket::Type QOSCPacket::detectType(QIODevice* dev)
{
    auto data = dev->peek(1);
    return detectType(data);
}

QOSCPacket::ptr QOSCPacket::read(const QByteArray& data)
{
    QOSCPacket::ptr r;

    switch(detectType(data))
    {
    case OSCBundle:
        r.reset(new QOSCBundle());
        break;

    case OSCMessage:
        r.reset(new QOSCMessage());
        break;

    default:
        break;
    }

    if(r)
        r->load(data);

    return r;
}

QOSCPacket::ptr QOSCPacket::read(QIODevice* dev)
{
    QOSCPacket::ptr r;

    switch(detectType(dev))
    {
    case OSCBundle:
        r.reset(new QOSCBundle());
        break;

    case OSCMessage:
        r.reset(new QOSCMessage());
        break;

    default:
        break;
    }

    if(r)
        r->load(dev);

    return r;
}

void QOSCPacket::load(const QByteArray& data)
{
    QBuffer buf(const_cast<QByteArray*>(&data));

    buf.open(QIODevice::ReadOnly);

    load(&buf);
}
