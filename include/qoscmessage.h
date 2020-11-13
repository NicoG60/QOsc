#ifndef QOSCMESSAGE_H
#define QOSCMESSAGE_H

#include "QOSC_global.h"
#include "qoscpacket.h"
#include "types/qoscvalue.h"
#include "qoschelper.h"
#include <QString>
#include <QRegularExpression>

#define QOSC_MESSAGE_VALUE_METHOD(FType, Type) \
    inline Type to##FType(int i = 0) const { return args[i]->to##FType(); }

class QOSC_EXPORT QOSCMessage : public QOSCPacket
{
public:
    typedef QSharedPointer<QOSCMessage> ptr;

    QOSCMessage() : QOSCPacket(OSCMessage) {}
    QOSCMessage(const QOSCMessage&) = default;
    QOSCMessage(QOSCMessage&&) = default;

    QOSCMessage& operator=(const QOSCMessage&) = default;
    QOSCMessage& operator=(QOSCMessage&&) = default;

    QOSCMessage(const QString& pattern) :
        QOSCPacket(OSCMessage),
        pattern(pattern)
    {
        compilePattern();
    }

    template<class T>
    QOSCMessage(const QString& pattern, const T& arg) :
        QOSCPacket(OSCMessage),
        pattern(pattern),
        args({ QOSC::makeValue(arg) })
    {
        compilePattern();
    }

    template<>
    QOSCMessage(const QString& pattern, const QOSCValue::ptr& arg) :
        QOSCPacket(OSCMessage),
        pattern(pattern),
        args({ arg })
    {
        compilePattern();
    }

    QOSCMessage(const QString& pattern, qint8 p, qint8 s, qint8 d1, qint8 d2) :
        QOSCPacket(OSCMessage),
        pattern(pattern),
        args({ QOSC::makeValue(p, s, d1, d2) })
    {
        compilePattern();
    }

    bool isValid() const override;

    bool match(const QString& addr) const;

    void write(QIODevice* dev) const override;

    QRegularExpression matcher() const { return _match; }

    // First Value exposition
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

    QOSC::Type valueType(int i = 0) const { return args[i]->type; }

    // QList exposition
    inline QOSCValue::list::iterator       begin()        { return args.begin();  }
    inline QOSCValue::list::const_iterator begin()  const { return args.begin();  }
    inline QOSCValue::list::const_iterator cbegin() const { return args.cbegin(); }

    inline QOSCValue::list::iterator       end()        { return args.end();  }
    inline QOSCValue::list::const_iterator end()  const { return args.end();  }
    inline QOSCValue::list::const_iterator cend() const { return args.cend(); }

    inline int size() const noexcept { return args.size(); }
    inline bool isEmpty() const noexcept { return args.isEmpty(); }
    inline void clear() { args.clear(); }

    inline const QOSCValue::ptr& at(int i)         const { return args.at(i); }
    inline       QOSCValue::ptr& operator[](int i)       { return args[i];    }
    inline const QOSCValue::ptr& operator[](int i) const { return args[i];    }

    inline void reserve(int size)                       { args.reserve(size); }
    inline void append(const QOSCValue::ptr& t)         { args.append(t);     }
    inline void prepend(const QOSCValue::ptr& t)        { args.prepend(t);    }
    inline void insert(int i, const QOSCValue::ptr& t)  { args.insert(i, t);  }
    inline void replace(int i, const QOSCValue::ptr& t) { args.replace(i, t); }
    inline void removeAt(int i)                         { args.removeAt(i);   }

    inline QOSCValue::ptr takeAt(int i) { return args.takeAt(i);   }
    inline QOSCValue::ptr takeFirst()   { return args.takeFirst(); }
    inline QOSCValue::ptr takeLast()    { return args.takeLast();  }

    inline       QOSCValue::ptr& first()       { return args.first(); }
    inline const QOSCValue::ptr& first() const { return args.first(); }
    inline       QOSCValue::ptr& last()        { return args.last();  }
    inline const QOSCValue::ptr& last()  const { return args.last();  }

    inline void removeFirst() { args.removeFirst(); }
    inline void removeLast()  { args.removeLast(); }

    inline QOSCMessage& operator+=(const QOSCValue::ptr& t) { append(t); return *this; }
    inline QOSCMessage& operator<< (const QOSCValue::ptr& t) { append(t); return *this; }

protected:
    void load(QIODevice* dev) override;

    template<class T>
    void loadArg(QIODevice* dev)
    {
        auto a = new T;
        a->readData(dev);
        args.append(QOSCValue::ptr(a));
    }

    void compilePattern();

    QRegularExpression _match;

public:
    QString pattern;
    QOSCValue::list args;
};

#endif // QOSCMESSAGE_H
