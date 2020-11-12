#ifndef QOSCFLOAT64_H
#define QOSCFLOAT64_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include "qosctypehelper.h"

class QOSC_EXPORT QOSCFloat64 : public QOSCAbstractType
{
public:
    QOSCFloat64() : QOSCAbstractType(QOSC::Float64Type) {}
    QOSCFloat64(const QOSCFloat64& copy) = default;
    QOSCFloat64(QOSCFloat64&& move) = default;

    inline QOSCFloat64(double f) : QOSCAbstractType(QOSC::Float64Type), _f(f) {}

    inline operator double() const { return _f; }

    inline QOSCFloat64& operator=(double f) { _f = f; return *this; };
    QOSCFloat64& operator=(const QOSCFloat64& i) = default;
    QOSCFloat64& operator=(QOSCFloat64&& i) = default;

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('d'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _f); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_f); }

private:
    double _f;
};

#endif // QOSCFLOAT64_H
