#include <qoscvalue.h>
#include "qoscvalue_p.h"

QOscValuePrivate* QOscValuePrivate::newFromType(QOsc::ValueType t)
{
    switch(t)
    {
    case QOsc::Int32Type:
        return new QOscInt32Private;

    case QOsc::Int64Type:
        return new QOscInt64Private;

    case QOsc::Float32Type:
        return new QOscFloat32Private;

    case QOsc::Float64Type:
        return new QOscFloat64Private;

    case QOsc::TrueType:
        return new QOscTruePrivate;

    case QOsc::FalseType:
        return new QOscFalsePrivate;

    case QOsc::NilType:
        return new QOscNilPrivate;

    case QOsc::InfinitumType:
        return new QOscInfinitumPrivate;

    case QOsc::CharType:
        return new QOscCharPrivate;

    case QOsc::ColorType:
        return new QOscColorPrivate;

    case QOsc::StringType:
        return new QOscStringPrivate;

    case QOsc::SymbolType:
        return new QOscSymbolPrivate;

    case QOsc::BlobType:
        return new QOscBlobPrivate;

    case QOsc::TimeTagType:
        return new QOscTimeTagPrivate;

    case QOsc::MidiType:
        return new QOscMidiPrivate;

    default:
        return nullptr;
    }
}

void QOscValuePrivate::copy(const QOscValuePrivate* src, QOscValuePrivate* dst)
{
    Q_ASSERT(src->type == dst->type);

    switch(src->type)
    {
    case QOsc::CharType:
    case QOsc::Int32Type:
        QOSC_PRIVATE_COPY(src, dst, QOscInt32Private, i);
        break;

    case QOsc::Int64Type:
        QOSC_PRIVATE_COPY(src, dst, QOscInt64Private, i);
        break;

    case QOsc::Float32Type:
        QOSC_PRIVATE_COPY(src, dst, QOscFloat32Private, f);
        break;

    case QOsc::Float64Type:
        QOSC_PRIVATE_COPY(src, dst, QOscFloat64Private, f);
        break;

    case QOsc::ColorType:
        QOSC_PRIVATE_COPY(src, dst, QOscColorPrivate, c);
        break;

    case QOsc::SymbolType:
    case QOsc::StringType:
        QOSC_PRIVATE_COPY(src, dst, QOscStringPrivate, str);
        break;

    case QOsc::BlobType:
        QOSC_PRIVATE_COPY(src, dst, QOscBlobPrivate, data);
        break;

    case QOsc::TimeTagType:
        QOSC_PRIVATE_COPY(src, dst, QOscTimeTagPrivate, t);
        break;

    case QOsc::MidiType:
        QOSC_PRIVATE_COPY(src, dst, QOscMidiPrivate, port);
        QOSC_PRIVATE_COPY(src, dst, QOscMidiPrivate, status);
        QOSC_PRIVATE_COPY(src, dst, QOscMidiPrivate, data1);
        QOSC_PRIVATE_COPY(src, dst, QOscMidiPrivate, data2);
        break;

    default:
        break;
    }
}

QOscValuePrivate* QOscValuePrivate::newCopyFrom(const QOscValuePrivate* src)
{
    auto dst = newFromType(src->type);
    copy(src, dst);
    return dst;
}

bool QOscValuePrivate::compare(const QOscValuePrivate* a, const QOscValuePrivate* b)
{
    if(a->type != b->type)
        return false;

    switch(a->type)
    {
    case QOsc::CharType:
    case QOsc::Int32Type:
        return QOSC_PRIVATE_EQ(a, b, QOscInt32Private, i);

    case QOsc::Int64Type:
        return QOSC_PRIVATE_EQ(a, b, QOscInt64Private, i);

    case QOsc::Float32Type:
        return QOSC_PRIVATE_EQ(a, b, QOscFloat32Private, f);

    case QOsc::Float64Type:
        return QOSC_PRIVATE_EQ(a, b, QOscFloat64Private, f);

    case QOsc::ColorType:
        return QOSC_PRIVATE_EQ(a, b, QOscColorPrivate, c);

    case QOsc::SymbolType:
    case QOsc::StringType:
        return QOSC_PRIVATE_EQ(a, b, QOscStringPrivate, str);

    case QOsc::BlobType:
        return QOSC_PRIVATE_EQ(a, b, QOscBlobPrivate, data);

    case QOsc::TimeTagType:
        return QOSC_PRIVATE_EQ(a, b, QOscTimeTagPrivate, t);

    case QOsc::MidiType:
        return QOSC_PRIVATE_EQ(a, b, QOscMidiPrivate, port)
                && QOSC_PRIVATE_EQ(a, b, QOscMidiPrivate, status)
                && QOSC_PRIVATE_EQ(a, b, QOscMidiPrivate, data1)
                && QOSC_PRIVATE_EQ(a, b, QOscMidiPrivate, data2);

    default:
        return false;
    }
}



// =============================================================================



QOscValue::QOscValue(QOscValuePrivate* d) :
    d_ptr(d)
{}

QOscValue::QOscValue() :
    d_ptr(new QOscNilPrivate)
{}

QOscValue::QOscValue(qint32 value) :
    d_ptr(new QOscInt32Private)
{
    d_ptr->fromInt32(value);
}

QOscValue::QOscValue(qint64 value) :
    d_ptr(new QOscInt64Private)
{
    d_ptr->fromInt64(value);
}

QOscValue::QOscValue(float  value) :
    d_ptr(new QOscFloat32Private)
{
    d_ptr->fromFloat32(value);
}

QOscValue::QOscValue(double value) :
    d_ptr(new QOscFloat64Private)
{
    d_ptr->fromFloat64(value);
}

QOscValue::QOscValue(bool value)
{
    if(value)
        d_ptr.reset(new QOscTruePrivate);
    else
        d_ptr.reset(new QOscFalsePrivate);
}

QOscValue::QOscValue(char value) :
    d_ptr(new QOscCharPrivate)
{
    d_ptr->fromChar(value);
}

QOscValue::QOscValue(const QColor& value) :
    d_ptr(new QOscColorPrivate)
{
    d_ptr->fromColor(value);
}

QOscValue::QOscValue(const QString& value) :
    d_ptr(new QOscStringPrivate)
{
    d_ptr->fromString(value);
}

QOscValue::QOscValue(const QByteArray& value) :
    d_ptr(new QOscBlobPrivate)
{
    d_ptr->fromBlob(value);
}

QOscValue::QOscValue(const QDateTime& value) :
    d_ptr(new QOscTimeTagPrivate)
{
    d_ptr->fromTimeTag(value);
}


QOscValue::QOscValue(const QOscValue& copy)
{
    *this = copy;
}

QOscValue::QOscValue(QOscValue&& move) noexcept
{
    swap(move);
}

QOscValue::~QOscValue()
{
    // Not using `= default` in the header because QScopedPointer doesn't allow it
}

QOscValue& QOscValue::operator=(const QOscValue& copy)
{
    if(!d_ptr || d_ptr->type != copy.type())
        d_ptr.reset(QOscValuePrivate::newCopyFrom(copy.d_ptr.get()));
    else
        QOscValuePrivate::copy(copy.d_ptr.get(), d_ptr.get());

    return *this;
}

QOscValue& QOscValue::operator=(QOscValue&& move) noexcept
{
    swap(move);
    return *this;
}

void QOscValue::swap(QOscValue& other)
{
    qSwap(d_ptr, other.d_ptr);
}

QOsc::ValueType QOscValue::type() const
{
    return d_ptr->type;
}

qint32 QOscValue::toInt32(qint32 defaultValue) const
{
    return d_ptr->toInt32(defaultValue);
}

bool QOscValue::isInt32() const
{
    return d_ptr->type == QOsc::Int32Type;
}

qint32 QOscValue::toInt(qint32 defaultValue) const
{
    return toInt32(defaultValue);
}

bool QOscValue::isInt() const
{
    return isInt32();
}

qint64 QOscValue::toInt64(qint64 defaultValue) const
{
    return d_ptr->toInt64(defaultValue);
}

bool QOscValue::isInt64() const
{
    return d_ptr->type == QOsc::Int64Type;
}

qint64 QOscValue::toLong(qint64 defaultValue) const
{
    return toInt64(defaultValue);
}

bool QOscValue::isLong() const
{
    return isInt64();
}

float QOscValue::toFloat32(float defaultValue) const
{
    return d_ptr->toFloat32(defaultValue);
}

bool QOscValue::isFloat32() const
{
    return d_ptr->type == QOsc::Float32Type;
}

float QOscValue::toFloat(float defaultValue) const
{
    return toFloat32(defaultValue);
}

bool QOscValue::isFloat() const
{
    return isFloat32();
}

double QOscValue::toFloat64(double defaultValue) const
{
    return d_ptr->toFloat64(defaultValue);
}

bool QOscValue::isFloat64() const
{
    return d_ptr->type == QOsc::Float64Type;
}

double QOscValue::toDouble(double defaultValue) const
{
    return toFloat64(defaultValue);
}

bool QOscValue::isDouble() const
{
    return isFloat64();
}

bool QOscValue::toBool(bool defaultValue) const
{
    return d_ptr->toBool(defaultValue);
}

bool QOscValue::isBool() const
{
    return isTrue() || isFalse();
}

bool QOscValue::isTrue() const
{
    return d_ptr->type == QOsc::TrueType;
}

bool QOscValue::isFalse() const
{
    return d_ptr->type == QOsc::FalseType;
}

bool QOscValue::isInfinitum() const
{
    return d_ptr->type == QOsc::InfinitumType;
}

bool QOscValue::isNil() const
{
    return d_ptr->type == QOsc::NilType;
}

char QOscValue::toChar(char defaultValue) const
{
    return d_ptr->toChar(defaultValue);
}

bool QOscValue::isChar() const
{
    return d_ptr->type == QOsc::CharType;
}

QColor QOscValue::toColor(const QColor& defaultValue) const
{
    return d_ptr->toColor(defaultValue);
}

bool QOscValue::isColor() const
{
    return d_ptr->type == QOsc::ColorType;
}

QString QOscValue::toString(const QString& defaultValue) const
{
    return d_ptr->toString(defaultValue);
}

bool QOscValue::isString() const
{
    return d_ptr->type == QOsc::StringType;
}

QString QOscValue::toSymbol(const QString& defaultValue) const
{
    return toString(defaultValue);
}

bool QOscValue::isSymbol() const
{
    return d_ptr->type == QOsc::SymbolType;
}

QByteArray QOscValue::toBlob(const QByteArray& defaultValue) const
{
    return d_ptr->toBlob(defaultValue);
}

bool QOscValue::isBlob() const
{
    return d_ptr->type == QOsc::BlobType;
}

QByteArray QOscValue::toByteArray(const QByteArray& defaultValue) const
{
    return toBlob(defaultValue);
}

bool QOscValue::isByteArray() const
{
    return isBlob();
}

QDateTime QOscValue::toTimeTag(const QDateTime& defaultValue) const
{
    return d_ptr->toTimeTag(defaultValue);
}

bool QOscValue::isTimeTag() const
{
    return d_ptr->type == QOsc::TimeTagType;
}

QDateTime QOscValue::toDateTime(const QDateTime &defaultValue) const
{
    return toTimeTag(defaultValue);
}

bool QOscValue::isDateTime() const
{
    return isTimeTag();
}

bool QOscValue::isNow() const
{
    Q_ASSERT(isTimeTag());
    return dynamic_cast<const QOscTimeTagPrivate*>(d_ptr.get())->isNow();
}

bool QOscValue::isMidi() const
{
    return d_ptr->type == QOsc::MidiType;
}

qint8 QOscValue::port() const
{
    Q_ASSERT(isMidi());
    return dynamic_cast<const QOscMidiPrivate*>(d_ptr.get())->port;
}

qint8 QOscValue::status() const
{
    Q_ASSERT(isMidi());
    return dynamic_cast<const QOscMidiPrivate*>(d_ptr.get())->status;
}

qint8 QOscValue::data1() const
{
    Q_ASSERT(isMidi());
    return dynamic_cast<const QOscMidiPrivate*>(d_ptr.get())->data1;
}

qint8 QOscValue::data2() const
{
    Q_ASSERT(isMidi());
    return dynamic_cast<const QOscMidiPrivate*>(d_ptr.get())->data2;
}

QOscValue& QOscValue::operator =(qint32 value)
{
    if(!isInt32())
        d_ptr.reset(new QOscInt32Private);

    d_ptr->fromInt32(value);
    return *this;
}

QOscValue& QOscValue::operator =(qint64 value)
{
    if(!isInt64())
        d_ptr.reset(new QOscInt64Private);

    d_ptr->fromInt64(value);
    return *this;
}

QOscValue& QOscValue::operator =(float value)
{
    if(!isFloat32())
        d_ptr.reset(new QOscFloat32Private);

    d_ptr->fromFloat32(value);
    return *this;
}

QOscValue& QOscValue::operator =(double value)
{
    if(!isFloat64())
        d_ptr.reset(new QOscFloat64Private);

    d_ptr->fromFloat64(value);
    return *this;
}

QOscValue& QOscValue::operator =(bool value)
{
    if(value && !isTrue())
        d_ptr.reset(new QOscTruePrivate);
    else if(!value && !isFalse())
        d_ptr.reset(new QOscFalsePrivate);

    return *this;
}

QOscValue& QOscValue::operator =(char value)
{
    if(!isChar())
        d_ptr.reset(new QOscCharPrivate);

    d_ptr->fromChar(value);
    return *this;
}

QOscValue& QOscValue::operator =(const QColor& value)
{
    if(!isColor())
        d_ptr.reset(new QOscColorPrivate);

    d_ptr->fromColor(value);
    return *this;
}

QOscValue& QOscValue::operator =(const QString& value)
{
    if(!isString() && !isSymbol())
        d_ptr.reset(new QOscStringPrivate);

    d_ptr->fromString(value);
    return *this;
}

QOscValue& QOscValue::operator =(const QByteArray& value)
{
    if(!isBlob())
        d_ptr.reset(new QOscBlobPrivate);

    d_ptr->fromBlob(value);
    return *this;
}

QOscValue& QOscValue::operator =(const QDateTime& value)
{
    if(!isTimeTag())
        d_ptr.reset(new QOscTimeTagPrivate);

    d_ptr->fromTimeTag(value);
    return *this;
}

bool QOscValue::operator ==(const QOscValue& other) const
{
    return QOscValuePrivate::compare(d_ptr.get(), other.d_ptr.get());
}

bool QOscValue::operator !=(const QOscValue& other) const
{
    return !(*this == other);
}

QOscValue QOscValue::fromUnixTimestamp(qint64 t)
{
    auto d = new QOscTimeTagPrivate;
    d->fromUnixEpoch(t);

    return QOscValue(d);
}

QOscValue QOscValue::asap()
{
    auto d = new QOscTimeTagPrivate;
    d->t = 1;

    return QOscValue(d);
}

QOscValue QOscValue::midiValue(qint8 port, qint8 status, qint8 data1, qint8 data2)
{
    auto d = new QOscMidiPrivate;

    d->port   = port;
    d->status = status;
    d->data1  = data1;
    d->data2  = data2;

    return QOscValue(d);
}

QOscValue QOscValue::trueValue()
{
    return QOscValue(new QOscTruePrivate);
}

QOscValue QOscValue::falseValue()
{
    return QOscValue(new QOscFalsePrivate);
}

QOscValue QOscValue::nilValue()
{
    return QOscValue(new QOscNilPrivate);
}

QOscValue QOscValue::infinitumValue()
{
    return QOscValue(new QOscInfinitumPrivate);
}

QOscValue QOscValue::symbolValue(const QString& str)
{
    auto d = new QOscSymbolPrivate;
    d->fromString(str);

    return QOscValue(d);
}

uint QOscValue::hash(uint seed) const
{
    return d_ptr->hash(seed);
}
