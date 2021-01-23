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

QString version();
QString commit();

QString displayVersion();

}

#endif // QOSC_GLOBAL_H
