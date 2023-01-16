#ifndef QOSCMESSAGE_H
#define QOSCMESSAGE_H

#include "qoscvalue.h"
#include "qosc_global.h"

#include <QList>
#include <QVector>
#include <QSet>
#include <QString>
#include <QRegularExpression>

#include <memory>

#define QOSC_MESSAGE_VALUE_METHOD(FType, Type) \
    inline Type to##FType(int i = 0) const { return at(i).to##FType(); }

class QOscBundle;
class QOscMessagePrivate;

class QOSC_EXPORT QOscMessage : public QList<QOscValue>
{
public:
    QOscMessage();
    QOscMessage(const QOscMessage& copy);
    QOscMessage(QOscMessage&& move) noexcept;
    QOscMessage(const QString& pattern);

    template<class T>
    QOscMessage(const QString& pattern, const T& arg) :
        QOscMessage(pattern)
    {
        append(QOscValue(arg));
    }

    ~QOscMessage();

    QOscMessage& operator=(const QOscMessage& copy);
    QOscMessage& operator=(QOscMessage&& move) noexcept;

    void swap(QOscMessage& other);

    bool isValid() const;

    bool match(const QString& addr) const;

    QString pattern() const;
    QRegularExpression matcher() const;

    void setPattern(const QString& p);

    QOSC_MESSAGE_VALUE_METHOD(Int32, qint32);
    QOSC_MESSAGE_VALUE_METHOD(Int,   qint32);

    QOSC_MESSAGE_VALUE_METHOD(Int64, qint64);
    QOSC_MESSAGE_VALUE_METHOD(Long,  qint64);

    QOSC_MESSAGE_VALUE_METHOD(Float32, float);
    QOSC_MESSAGE_VALUE_METHOD(Float,   float);

    QOSC_MESSAGE_VALUE_METHOD(Float64, double);
    QOSC_MESSAGE_VALUE_METHOD(Double,  double);

    QOSC_MESSAGE_VALUE_METHOD(Bool, bool);

    QOSC_MESSAGE_VALUE_METHOD(Char,  char);

    QOSC_MESSAGE_VALUE_METHOD(Color, QColor);

    QOSC_MESSAGE_VALUE_METHOD(String, QString);
    QOSC_MESSAGE_VALUE_METHOD(Symbol, QString);

    QOSC_MESSAGE_VALUE_METHOD(Blob,      QByteArray);
    QOSC_MESSAGE_VALUE_METHOD(ByteArray, QByteArray);

    QOSC_MESSAGE_VALUE_METHOD(DateTime, QDateTime);
    QOSC_MESSAGE_VALUE_METHOD(TimeTag,  QDateTime);

    QOsc::ValueType type(int i = 0) const { return at(i).type(); }

    QOscMessage& operator <<(const QOscValue& v);

    QOscMessage& operator +=(const QOscValue& v);

    QOscMessage operator +(const QOscValue& v) const;

    QOscBundle operator +(const QOscMessage& v) const;

    QOscValue& operator[](int i);
    const QOscValue& operator[](int i) const;

    QByteArray package() const;
    void package(QIODevice* dev) const;

    static QOscMessage read(const QByteArray& data);
    static QOscMessage read(QIODevice* dev);

private:
    std::unique_ptr<QOscMessagePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QOscMessage);
};

Q_DECLARE_METATYPE(QOscMessage);

#endif // QOSCMESSAGE_H
