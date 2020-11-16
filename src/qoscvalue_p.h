#ifndef QOSCVALUE_P_H
#define QOSCVALUE_P_H

#include <qosc.h>

#include <QtEndian>
#include <QIODevice>
#include <QColor>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QHash>

#define QOSC_PRIVATE_ACCESS(FType, Type, OSCType, Default) \
    virtual inline Type to##FType(const Type& defaultValue) const { return defaultValue; } \
    virtual inline void from##FType(const Type& value) { Q_UNUSED(value) Q_ASSERT_X(false, "from"#FType, "Not a " #FType); } \
    virtual inline bool is##FType() const { return type == OSCType; }

#define QOSC_PRIVATE_COPY(FROM, TO, CLASS, VAR) \
    dynamic_cast<CLASS*>(TO)->VAR = dynamic_cast<const CLASS*>(FROM)->VAR;

#define QOSC_PRIVATE_EQ(A, B, CLASS, VAR) \
    dynamic_cast<const CLASS*>(A)->VAR == dynamic_cast<const CLASS*>(B)->VAR

class QOscValuePrivate
{
public:
    QOscValuePrivate(QOsc::ValueType t) : type(t) {}
    virtual ~QOscValuePrivate() {}

    virtual void writeTypeTag(QIODevice* dev) const = 0;
    virtual void writeData(QIODevice*) const {}
    virtual void readData(QIODevice*) {};

    QOSC_PRIVATE_ACCESS(Int32,   qint32, QOsc::Int32Type,   0);
    QOSC_PRIVATE_ACCESS(Int64,   qint64, QOsc::Int64Type,   0ll);
    QOSC_PRIVATE_ACCESS(Float32, float,  QOsc::Float32Type, 0.0f);
    QOSC_PRIVATE_ACCESS(Float64, double, QOsc::Float64Type, 0.0);

    virtual bool toBool(bool defaulValue)  const { return defaulValue; }
    virtual bool isBool()  const { return type == QOsc::TrueType || type == QOsc::FalseType; }
    virtual bool isTrue()  const { return type == QOsc::TrueType; }
    virtual bool isFalse() const { return type == QOsc::FalseType; }
    virtual bool isInfinitum()  const { return type == QOsc::InfinitumType; }
    virtual bool isNil() const { return type == QOsc::NilType; }

    QOSC_PRIVATE_ACCESS(Char,    char,       QOsc::CharType, '\0');
    QOSC_PRIVATE_ACCESS(Color,   QColor,     QOsc::ColorType, {});
    QOSC_PRIVATE_ACCESS(String,  QString,    QOsc::StringType, {});
    QOSC_PRIVATE_ACCESS(Blob,    QByteArray, QOsc::BlobType, {});
    QOSC_PRIVATE_ACCESS(TimeTag, QDateTime,  QOsc::TimeTagType, {});

    template<class T>
    void writeHelper(QIODevice* dev, T v) const
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

    static QOscValuePrivate* newFromType(QOsc::ValueType t);
    static void copy(const QOscValuePrivate* src, QOscValuePrivate* dst);
    static QOscValuePrivate* newCopyFrom(const QOscValuePrivate* src);
    static bool compare(const QOscValuePrivate *a, const QOscValuePrivate *b);

    virtual inline uint hash(uint seed = 0) { return qHash(type, seed); }

    const QOsc::ValueType type;
};



// =============================================================================



class QOscInt32Private : public QOscValuePrivate
{
protected:
    QOscInt32Private(QOsc::ValueType t) : QOscValuePrivate(t) {}

public:
    QOscInt32Private() : QOscValuePrivate(QOsc::Int32Type) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('i'); }
    inline void writeData(QIODevice* dev) const override { writeHelper(dev, i); }
    inline void readData(QIODevice* dev) override { readHelper(dev, &i); }

    inline qint32 toInt32(const qint32&) const override { return i; }
    inline qint64 toInt64(const qint64&) const override { return i; }
    inline float  toFloat32(const float&) const override { return i; }
    inline double toFloat64(const double&) const override { return i; }

    inline void fromInt32(const qint32& v) override { i = v; }

    inline uint hash(uint seed = 0) override { return qHash(i, seed) ^ QOscValuePrivate::hash(seed); }

    qint32 i;
};



// =============================================================================



class QOscInt64Private : public QOscValuePrivate
{
public:
    QOscInt64Private() : QOscValuePrivate(QOsc::Int64Type) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('h'); }
    inline void writeData(QIODevice* dev) const override { writeHelper(dev, i); }
    inline void readData(QIODevice* dev) override { readHelper(dev, &i); }

    inline qint32 toInt32(const qint32&) const override { return i; }
    inline qint64 toInt64(const qint64&) const override { return i; }
    inline float  toFloat32(const float&) const override { return i; }
    inline double toFloat64(const double&) const override { return i; }

    inline void fromInt64(const qint64& v) override { i = v; }

    inline uint hash(uint seed = 0) override { return qHash(i, seed) ^ QOscValuePrivate::hash(seed); }

    qint64 i;
};



// =============================================================================



class QOscFloat32Private : public QOscValuePrivate
{
public:
    QOscFloat32Private() : QOscValuePrivate(QOsc::Float32Type) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('f'); }
    inline void writeData(QIODevice* dev) const override { writeHelper(dev, f); }
    inline void readData(QIODevice* dev) override { readHelper(dev, &f); }

    inline qint32 toInt32(const qint32&) const override { return f; }
    inline qint64 toInt64(const qint64&) const override { return f; }
    inline float  toFloat32(const float&) const override { return f; }
    inline double toFloat64(const double&) const override { return f; }

    inline void fromFloat32(const float& v) override { f = v; }

    inline uint hash(uint seed = 0) override { return qHash(f, seed) ^ QOscValuePrivate::hash(seed); }

    float f;
};



// =============================================================================



class QOscFloat64Private : public QOscValuePrivate
{
public:
    QOscFloat64Private() : QOscValuePrivate(QOsc::Float64Type) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('d'); }
    inline void writeData(QIODevice* dev) const override { writeHelper(dev, f); }
    inline void readData(QIODevice* dev) override { readHelper(dev, &f); }

    inline qint32 toInt32(const qint32&) const override { return f; }
    inline qint64 toInt64(const qint64&) const override { return f; }
    inline float  toFloat32(const float&) const override { return f; }
    inline double toFloat64(const double&) const override { return f; }

    inline void fromFloat64(const double& v) override { f = v; }

    inline uint hash(uint seed = 0) override { return qHash(f, seed) ^ QOscValuePrivate::hash(seed); }

    double f;
};



// =============================================================================



class QOscCharPrivate : public QOscInt32Private
{
public:
    QOscCharPrivate() : QOscInt32Private(QOsc::CharType) {};

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('c'); }

    inline char toChar(const char&) const override { return static_cast<char>(i); }
    inline void fromChar(const char& v) override { i = v; }
};



// =============================================================================



class QOscColorPrivate : public QOscValuePrivate
{
public:
    QOscColorPrivate() : QOscValuePrivate(QOsc::ColorType) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('r'); }
    inline void writeData(QIODevice* dev) const override
    {
        quint32 rgba = ((c.red()   & 0xFF) << 24) +
                       ((c.green() & 0xFF) << 16) +
                       ((c.blue()  & 0xFF) <<  8) +
                        (c.alpha() & 0xFF);
        writeHelper(dev, rgba);
    }

    inline void readData(QIODevice* dev) override
    {
        quint32 rgba;
        readHelper(dev, &rgba);
        int r, g, b, a;

        r = (rgba >> 24) & 0xFF;
        g = (rgba >> 16) & 0xFF;
        b = (rgba >>  8) & 0xFF;
        a = (rgba      ) & 0xFF;

        c = QColor(r, g, b, a);
    }

    inline QColor toColor(const QColor&) const override { return c; }
    inline void fromColor(const QColor& v) override { c = v; }

    inline uint hash(uint seed = 0) override { return qHash(c.rgba(), seed) ^ QOscValuePrivate::hash(seed); }

    QColor c;
};



// =============================================================================



template<char C, QOsc::ValueType T>
class QOscLiteralPrivate : public QOscValuePrivate
{
public:
    QOscLiteralPrivate() : QOscValuePrivate(T) {}

    inline bool toBool(bool) const override { return C == 'T' || C == 'I'; }

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar(C); }

    inline uint hash(uint seed = 0) override { return qHash(C, seed) ^ QOscValuePrivate::hash(seed); }
};

typedef QOscLiteralPrivate<'T', QOsc::TrueType>      QOscTruePrivate;
typedef QOscLiteralPrivate<'F', QOsc::FalseType>     QOscFalsePrivate;
typedef QOscLiteralPrivate<'N', QOsc::NilType>       QOscNilPrivate;
typedef QOscLiteralPrivate<'I', QOsc::InfinitumType> QOscInfinitumPrivate;



// =============================================================================



class QOscMidiPrivate : public QOscValuePrivate
{
public:
    QOscMidiPrivate() : QOscValuePrivate(QOsc::MidiType) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('m'); }
    inline void writeData(QIODevice* dev) const override
    {
        dev->putChar(port);
        dev->putChar(status);
        dev->putChar(data1);
        dev->putChar(data2);
    }

    inline void readData(QIODevice* dev) override
    {
        dev->getChar((char*)&port);
        dev->getChar((char*)&status);
        dev->getChar((char*)&data1);
        dev->getChar((char*)&data2);
    }

    inline uint hash(uint seed = 0) override { return qHash(port, seed)   ^
                                                      qHash(status, seed) ^
                                                      qHash(data1, seed)  ^
                                                      qHash(data2, seed)  ^
                                                      QOscValuePrivate::hash(seed); }

    qint8 port;
    qint8 status;
    qint8 data1;
    qint8 data2;
};



// =============================================================================



class QOscStringPrivate : public QOscValuePrivate
{
protected:
    QOscStringPrivate(QOsc::ValueType t) : QOscValuePrivate(t) {}

public:
    QOscStringPrivate() : QOscValuePrivate(QOsc::StringType) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('s'); }
    inline void writeData(QIODevice* dev) const override
    {
        QByteArray data = str.toLatin1();
        do {
            data.append('\x0');
        } while(data.size() % 4 != 0);

        dev->write(data);
    }

    inline void readData(QIODevice* dev) override
    {
        str.clear();
        char c = 0;

        while(dev->getChar(&c))
        {
            if(c == 0)
                break;

            str.append(c);
        }

        int i = str.size()+1;

        while(i % 4 != 0 && dev->peek(&c, 1) == 1 && c == 0)
        {
            dev->getChar(nullptr);
            i++;
        }
    }

    inline void fromString(const QString& s) override { str = s; }
    inline QString toString(const QString&) const override { return str; }
    inline QByteArray toBlob(const QByteArray&) const override { return str.toLatin1(); }

    inline uint hash(uint seed = 0) override { return qHash(str, seed) ^ QOscValuePrivate::hash(seed); }

    QString str;
};



// =============================================================================



class QOscSymbolPrivate : public QOscStringPrivate
{
public:
    QOscSymbolPrivate() : QOscStringPrivate(QOsc::SymbolType) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('S'); }
};



// =============================================================================



class QOscBlobPrivate : public QOscValuePrivate
{
public:
    QOscBlobPrivate() : QOscValuePrivate(QOsc::BlobType) {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('b'); }
    inline void writeData(QIODevice* dev)    const override
    {
        QByteArray d = data;

        while(d.size() % 4 != 0)
            d.append('\x0');

        writeHelper(dev, static_cast<qint32>(d.size()));
        dev->write(d);
    }

    inline void readData(QIODevice* dev) override
    {
        qint32 size;
        readHelper(dev, &size);
        data = dev->read(size);
    }

    inline void fromBlob(const QByteArray& d) override { data = d; }
    inline QByteArray toBlob(const QByteArray&) const override { return data; }
    inline QString toString(const QString&) const override { return QString::fromLatin1(data); }

    inline uint hash(uint seed = 0) override { return qHash(data, seed) ^ QOscValuePrivate::hash(seed); }

    QByteArray data;
};



// =============================================================================



class QOscTimeTagPrivate : public QOscValuePrivate
{
    constexpr static quint64 unix_to_ntp = 2208988800ul;
    constexpr static quint64 factor = 1ull << 32;

public:
    QOscTimeTagPrivate() : QOscValuePrivate(QOsc::TimeTagType) {}

    inline bool isNow() const { return t == 1; }

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('t'); }
    inline void writeData(QIODevice* dev) const override { writeHelper(dev, t); }

    inline void readData(QIODevice* dev) override { readHelper(dev, &t); }

    inline void fromNtpEpoch(double d) { t = d >= 0 ? static_cast<quint64>(d*factor) : 1ull; }
    inline double toNtpEpoch() const { return t/static_cast<double>(factor); }

    inline void fromUnixEpoch(double d) { fromNtpEpoch(d+unix_to_ntp); }
    inline double toUnixEpoch() const { return toNtpEpoch() - unix_to_ntp; }

    inline void fromTimeTag(const QDateTime& dt) override { fromUnixEpoch(dt.toMSecsSinceEpoch()/1000.0); }
    inline QDateTime toTimeTag(const QDateTime&) const override { return QDateTime::fromMSecsSinceEpoch(toUnixEpoch()*1000, Qt::UTC); }

    inline uint hash(uint seed = 0) override { return qHash(t, seed) ^ QOscValuePrivate::hash(seed); }

    quint64 t;
};



#endif // QOSCVALUE_P_H
