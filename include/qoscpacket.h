#ifndef QOSCPACKET_H
#define QOSCPACKET_H

#include "QOSC_global.h"
#include <QSharedPointer>
#include <QByteArray>
#include <QIODevice>

class QOSC_EXPORT QOSCPacket
{
public:
    typedef QSharedPointer<QOSCPacket> ptr;
    typedef QList<ptr> list;

    enum Type
    {
        InvalidPacket,
        OSCMessage,
        OSCBundle
    };

    constexpr QOSCPacket(Type t) : type(t) {}
    constexpr QOSCPacket(const QOSCPacket&) = default;
    constexpr QOSCPacket(QOSCPacket&&) = default;
    virtual ~QOSCPacket() {}

    QOSCPacket& operator=(const QOSCPacket&) { return *this; };
    QOSCPacket& operator=(QOSCPacket&&) { return *this; };

    virtual bool isValid() const = 0;

    virtual QByteArray package() const;
    virtual void write(QIODevice* dev) const = 0;

    static Type detectType(const QByteArray& data);
    static Type detectType(QIODevice* dev);

    static ptr read(const QByteArray& data);
    static ptr read(QIODevice* dev);

protected:
    virtual void load(const QByteArray& data);
    virtual void load(QIODevice* dev) = 0;

public:
    const Type type;
};

Q_DECLARE_METATYPE(QOSCPacket::ptr)
Q_DECLARE_METATYPE(QOSCPacket::list)

#endif // QOSCPACKET_H
