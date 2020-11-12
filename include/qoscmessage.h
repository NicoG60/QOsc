#ifndef QOSCMESSAGE_H
#define QOSCMESSAGE_H

#include "QOSC_global.h"
#include "qoscpacket.h"
#include "types/qoscabstracttype.h"
#include "types/qosctypehelper.h"
#include <QString>
#include <QRegularExpression>

class QOSC_EXPORT QOSCMessage : public QOSCPacket
{
public:
    typedef QSharedPointer<QOSCMessage> ptr;

    QOSCMessage() : QOSCPacket(OSCMessage) {}
    QOSCMessage(const QOSCMessage&) = default;
    QOSCMessage(QOSCMessage&&) = default;

    QOSCMessage& operator=(const QOSCMessage&) = default;
    QOSCMessage& operator=(QOSCMessage&&) = default;

    template<class T>
    QOSCMessage(const QString& pattern, const T& arg) :
        QOSCPacket(OSCMessage),
        pattern(pattern),
        args({ QOSC::make(arg) })
    {
        compilePattern();
    }

    template<>
    QOSCMessage(const QString& pattern, const QOSCAbstractType::ptr& arg) :
        QOSCPacket(OSCMessage),
        pattern(pattern),
        args({ arg })
    {
        compilePattern();
    }

    QOSCMessage(const QString& pattern, qint8 p, qint8 s, qint8 d1, qint8 d2) :
        QOSCPacket(OSCMessage),
        pattern(pattern),
        args({ QOSC::make(p, s, d1, d2) })
    {
        compilePattern();
    }

    bool isValid() const override;

    bool match(const QString& addr) const;

    void write(QIODevice* dev) const override;

    QRegularExpression matcher() const { return _match; }

protected:
    void load(QIODevice* dev) override;

    template<class T>
    void loadArg(QIODevice* dev)
    {
        auto a = new T;
        a->readData(dev);
        args.append(QOSCAbstractType::ptr(a));
    }

    void compilePattern();

    QRegularExpression _match;

public:
    QString pattern;
    QOSCAbstractType::list args;
};

#endif // QOSCMESSAGE_H
