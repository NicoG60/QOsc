#ifndef QOSCTYPEHELPER_H
#define QOSCTYPEHELPER_H

#include "QOSC_global.h"
#include <QtEndian>
#include <QIODevice>

class QOSCAbstractType;
namespace QOSC
{

Q_NAMESPACE_EXPORT(QOSC_EXPORT)

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
Q_ENUM_NS(Type)

QSharedPointer<QOSCAbstractType> make(const QByteArray& b);
QSharedPointer<QOSCAbstractType> make(const QString& str);

QSharedPointer<QOSCAbstractType> make(char c);

QSharedPointer<QOSCAbstractType> make(qint32 i32);
QSharedPointer<QOSCAbstractType> make(qint64 i64);

QSharedPointer<QOSCAbstractType> make(float f);
QSharedPointer<QOSCAbstractType> make(double d);

QSharedPointer<QOSCAbstractType> make(const QDateTime& dt);
QSharedPointer<QOSCAbstractType> make(const QColor& c);
QSharedPointer<QOSCAbstractType> make(qint8 p, qint8 s, qint8 d1, qint8 d2);

QSharedPointer<QOSCAbstractType> make(bool b);
QSharedPointer<QOSCAbstractType> make(std::nullptr_t ptr = nullptr);

template<class T>
void writeHelper(QIODevice* dev, T v)
{
    T be = qToBigEndian(v);
    dev->write((const char*)&be, sizeof (T));
}

template<class T>
void readHelper(QIODevice* dev, T* r)
{
    Q_ASSERT(r);
    dev->read((char*)r, sizeof (T));
    *r = qFromBigEndian(*r);
}

}

#endif // QOSCTYPEHELPER_H
