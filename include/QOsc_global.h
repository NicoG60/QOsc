#ifndef QOSC_GLOBAL_H
#define QOSC_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/qobject.h>

#if defined(QOSC_LIBRARY)
#  define QOSC_EXPORT Q_DECL_EXPORT
#else
#  define QOSC_EXPORT Q_DECL_IMPORT
#endif

class QOscMessage;
class QOscBundle;
class QIODevice;

namespace QOsc
{

Q_NAMESPACE_EXPORT(QOSC_EXPORT);

enum QOSC_EXPORT ValueType
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
Q_ENUM_NS(ValueType)

enum QOSC_EXPORT PacketType
{
    InvalidPacket,
    OscMessage,
    OscBundle
};
Q_ENUM_NS(PacketType)

PacketType detectType(const QByteArray& data);
PacketType detectType(QIODevice* dev);

}

#endif // QOSC_GLOBAL_H
