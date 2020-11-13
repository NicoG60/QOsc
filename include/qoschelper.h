#ifndef QOSCTYPEHELPER_H
#define QOSCTYPEHELPER_H

#include "QOSC_global.h"
#include <QtEndian>
#include <QIODevice>

class QOSCValue;
namespace QOSC
{

//Q_NAMESPACE_EXPORT(QOSC_EXPORT)

enum QOSC_EXPORT Type
{
    ArrayType,
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

QSharedPointer<QOSCValue> makeValue(const QByteArray& b);
QSharedPointer<QOSCValue> makeValue(const QString& str);

QSharedPointer<QOSCValue> makeValue(char c);

QSharedPointer<QOSCValue> makeValue(qint32 i32);
QSharedPointer<QOSCValue> makeValue(qint64 i64);

QSharedPointer<QOSCValue> makeValue(float f);
QSharedPointer<QOSCValue> makeValue(double d);

QSharedPointer<QOSCValue> makeValue(const QDateTime& dt);
QSharedPointer<QOSCValue> makeValue(const QColor& c);
QSharedPointer<QOSCValue> makeValue(qint8 p, qint8 s, qint8 d1, qint8 d2);

QSharedPointer<QOSCValue> makeValue(bool b);
QSharedPointer<QOSCValue> makeValue(std::nullptr_t ptr = nullptr);

template<class T>
void writeHelper(QIODevice* dev, T v)
{
    v = qToBigEndian(v);
    qint64 tmp = dev->write(reinterpret_cast<const char*>(&v), sizeof (T));
    Q_ASSERT(tmp == sizeof (T));
}

template<class T>
void readHelper(QIODevice* dev, T* r)
{
    Q_ASSERT(r);
    T tmp;
    qint64 s = dev->read(reinterpret_cast<char*>(&tmp), sizeof (T));
    Q_ASSERT(s == sizeof (T));
    *r = qFromBigEndian(tmp);
}

}

#endif // QOSCTYPEHELPER_H
