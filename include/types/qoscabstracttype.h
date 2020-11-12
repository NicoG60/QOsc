#ifndef QOSCABSTRACTTYPE_H
#define QOSCABSTRACTTYPE_H

#include "QOSC_global.h"
#include "qosctypehelper.h"
#include <QIODevice>
#include <QSharedPointer>

class QOSC_EXPORT QOSCAbstractType
{
public:
    typedef QSharedPointer<QOSCAbstractType> ptr;
    typedef QList<ptr> list;

public:
    constexpr QOSCAbstractType(QOSC::Type t) : type(t) {}
    constexpr QOSCAbstractType(const QOSCAbstractType&) = default;
    constexpr QOSCAbstractType(QOSCAbstractType&&) = default;
    virtual ~QOSCAbstractType() {};

    QOSCAbstractType& operator=(const QOSCAbstractType&) { return *this; }
    QOSCAbstractType& operator=(QOSCAbstractType&&) { return *this; }

    virtual void writeTypeTag(QIODevice* dev) const = 0;
    virtual void writeData(QIODevice*) const {}
    virtual void readData(QIODevice*) {};

    const QOSC::Type type;
};

Q_DECLARE_METATYPE(QOSCAbstractType::ptr)
Q_DECLARE_METATYPE(QOSCAbstractType::list)

#endif // QOSCABSTRACTTYPE_H
