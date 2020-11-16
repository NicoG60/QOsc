#ifndef QOSC_GLOBAL_H
#define QOSC_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/qobject.h>

class QOscMessage;
class QOscBundle;
class QIODevice;

namespace QOsc
{

enum ValueType
{
    //ArrayType,
    BlobType,
    CharType,
    ColorType,
    Float32Type,
    Float64Type,
    Int32Type,
    Int64Type,
    TrueType,
    FalseType,
    NilType,
    InfinitumType,
    MidiType,
    StringType,
    SymbolType,
    TimeTagType
};

enum PacketType
{
    InvalidPacket,
    OscMessage,
    OscBundle
};

PacketType detectType(const QByteArray& data);
PacketType detectType(QIODevice* dev);

}

#endif // QOSC_GLOBAL_H
