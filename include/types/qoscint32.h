#ifndef QOSCINT32_H
#define QOSCINT32_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include "qosctypehelper.h"

class QOSC_EXPORT QOSCInt32 : public QOSCAbstractType
{
protected:
    QOSCInt32(QOSC::Type t) : QOSCAbstractType(t) {}
    QOSCInt32(qint32 i, QOSC::Type t) : QOSCAbstractType(t), _i(i) {}

public:
    QOSCInt32() : QOSCAbstractType(QOSC::Int32Type) {}
    QOSCInt32(const QOSCInt32& copy) = default;
    QOSCInt32(QOSCInt32&& move) = default;

    QOSCInt32& operator=(const QOSCInt32& i) = default;
    QOSCInt32& operator=(QOSCInt32&& i) = default;

    inline QOSCInt32(qint32 i) : QOSCAbstractType(QOSC::Int32Type), _i(i) {}

    inline operator qint32() const { return _i; }

    inline QOSCInt32& operator=(qint32 i) { _i = i; return *this; };

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('i'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _i); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_i); }

protected:
    qint32 _i;
};

#endif // QOSCINT32_H
