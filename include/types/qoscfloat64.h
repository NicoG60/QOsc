#ifndef QOSCFLOAT64_H
#define QOSCFLOAT64_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include "qosctypehelper.h"

class QOSC_EXPORT QOSCFloat64 : public QOSCAbstractType
{
public:
    QOSC_TYPE_CTOR(QOSCFloat64, QOSC::Float64Type);

    QOSC_TYPE_DATA_CTOR(QOSCFloat64, QOSC::Float64Type, Double, double)

    QOSC_ACCESS_IMPL_CAST(Int,   qint32, _f);
    QOSC_ACCESS_IMPL_CAST(Int32, qint32, _f);

    QOSC_ACCESS_IMPL_CAST(Long,  qint64, _f);
    QOSC_ACCESS_IMPL_CAST(Int64, qint64, _f);

    QOSC_ACCESS_IMPL(Float,   float, _f);
    QOSC_ACCESS_IMPL(Float32, float, _f);

    QOSC_ACCESS_IMPL(Double,  double, _f);
    QOSC_ACCESS_IMPL(Float64, double, _f);

    QOSC_DERIVED_OPERATOR(QOSCFloat64, qint32);
    QOSC_DERIVED_OPERATOR(QOSCFloat64, qint64);
    QOSC_DERIVED_OPERATOR(QOSCFloat64, float);
    QOSC_DERIVED_OPERATOR(QOSCFloat64, double);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('d'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _f); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_f); }

private:
    double _f;
};

#endif // QOSCFLOAT64_H
