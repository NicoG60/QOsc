#include <qosc_global.h>
#include <qoscmessage.h>
#include <qoscbundle.h>

#include "qoscmessage_p.h"
#include "qoscbundle_p.h"

#include <QBuffer>

namespace QOsc
{

PacketType detectType(const QByteArray& data)
{
    if(data.isEmpty())
        return InvalidPacket;

    switch(data[0])
    {
    case '/':
        return OscMessage;

    case '#':
        return OscBundle;

    default:
        return InvalidPacket;
    }
}

PacketType detectType(QIODevice* dev)
{
    auto data = dev->peek(1);
    return detectType(data);
}

}
