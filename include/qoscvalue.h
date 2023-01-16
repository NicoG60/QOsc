#ifndef QOSCVALUE_H
#define QOSCVALUE_H

#include "qosc_global.h"

#include <QIODevice>
#include <QSharedPointer>
#include <QColor>
#include <QDateTime>

#include <memory>

#define QOSC_VALUE_ACCESS(FType, Type, Default) \
    Type to##FType(Type defaultValue = Default) const; \
    bool is##FType() const;

#define QOSC_VALUE_ACCESS_REF(FType, Type, Default) \
    Type to##FType(const Type& defaultValue = Default) const; \
    bool is##FType() const;

class QOscValuePrivate;
class QOSC_EXPORT QOscValue
{
    friend class QOscMessage;
    friend class QOscMessagePrivate;
    friend class QOscBundle;
    friend class QOscBundlePrivate;

    friend class types;

protected:
    QOscValue(QOscValuePrivate* d);

public:
    QOscValue();

    QOscValue(qint32 value);
    QOscValue(qint64 value);
    QOscValue(float  value);
    QOscValue(double value);
    QOscValue(bool value);
    QOscValue(char value);
    QOscValue(const QColor& value);
    QOscValue(const QString& value);
    QOscValue(const QByteArray& value);
    QOscValue(const QDateTime& value);

    QOscValue(const QOscValue& copy);
    QOscValue(QOscValue&& move) noexcept;
    ~QOscValue();

    QOscValue& operator=(const QOscValue& copy);
    QOscValue& operator=(QOscValue&& move) noexcept;

    void swap(QOscValue& other);

    QOsc::ValueType type() const;

    QOSC_VALUE_ACCESS(Int32, qint32, 0);
    QOSC_VALUE_ACCESS(Int,   qint32, 0);

    QOSC_VALUE_ACCESS(Int64, qint64, 0l);
    QOSC_VALUE_ACCESS(Long,  qint64, 0l);

    QOSC_VALUE_ACCESS(Float32, float, 0.0f);
    QOSC_VALUE_ACCESS(Float,   float, 0.0f);

    QOSC_VALUE_ACCESS(Float64, double, 0.0);
    QOSC_VALUE_ACCESS(Double,  double, 0.0);

    bool toBool(bool defaultValue = false) const;
    bool isBool()      const;
    bool isTrue()      const;
    bool isFalse()     const;
    bool isInfinitum() const;
    bool isNil()       const;

    QOSC_VALUE_ACCESS(Char,  char, '\x00');

    QOSC_VALUE_ACCESS_REF(Color, QColor, {});

    QOSC_VALUE_ACCESS_REF(String, QString, {});
    QOSC_VALUE_ACCESS_REF(Symbol, QString, {});

    QOSC_VALUE_ACCESS_REF(Blob,      QByteArray, {});
    QOSC_VALUE_ACCESS_REF(ByteArray, QByteArray, {});

    QOSC_VALUE_ACCESS_REF(TimeTag,  QDateTime, {});
    QOSC_VALUE_ACCESS_REF(DateTime, QDateTime, {});

    bool isNow() const;

    bool isMidi() const;
    qint8 port()   const;
    qint8 status() const;
    qint8 data1()  const;
    qint8 data2()  const;

    QOscValue& operator =(qint32 value);
    QOscValue& operator =(qint64 value);
    QOscValue& operator =(float value);
    QOscValue& operator =(double value);
    QOscValue& operator =(bool value);
    QOscValue& operator =(char value);
    QOscValue& operator =(const QColor& value);
    QOscValue& operator =(const QString& value);
    QOscValue& operator =(const QByteArray& value);
    QOscValue& operator =(const QDateTime& value);

    bool operator ==(const QOscValue& other) const;
    bool operator !=(const QOscValue& other) const;

    static QOscValue fromUnixTimestamp(qint64 t);

    static QOscValue asap();

    static QOscValue midiValue(qint8 port, qint8 status, qint8 data1, qint8 data2);
    static QOscValue trueValue();
    static QOscValue falseValue();
    static QOscValue nilValue();
    static QOscValue infinitumValue();
    static QOscValue symbolValue(const QString& str);

    friend uint qHash(const QOscValue& value, uint seed = 0)
    {
        return value.hash(seed);
    }

protected:
    uint hash(uint seed) const;

protected:
    std::unique_ptr<QOscValuePrivate> d_ptr;
};

#endif // QOSCVALUE_H
