#ifndef QOSCINT64_H
#define QOSCINT64_H

#include "QOSC_global.h"
#include "qoscvalue.h"

class QOSC_EXPORT QOSCInt64 : public QOSCValue
{
public:
    QOSC_TYPE_CTOR(QOSCInt64, QOSC::Int64Type);

    QOSC_TYPE_DATA_CTOR(QOSCInt64, QOSC::Int64Type, Int64, qint64)

    QOSC_ACCESS_IMPL(Int,   qint32, _i);
    QOSC_ACCESS_IMPL(Int32, qint32, _i);

    QOSC_ACCESS_IMPL(Long,  qint64, _i);
    QOSC_ACCESS_IMPL(Int64, qint64, _i);

    QOSC_ACCESS_IMPL_CAST(Float,   float, _i);
    QOSC_ACCESS_IMPL_CAST(Float32, float, _i);

    QOSC_ACCESS_IMPL_CAST(Double,  double, _i);
    QOSC_ACCESS_IMPL_CAST(Float64, double, _i);

    QOSC_DERIVED_OPERATOR(QOSCInt64, qint32);
    QOSC_DERIVED_OPERATOR(QOSCInt64, qint64);
    QOSC_DERIVED_OPERATOR(QOSCInt64, float);
    QOSC_DERIVED_OPERATOR(QOSCInt64, double);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('h'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _i); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_i); }

private:
    qint64 _i = {};
};

#endif // QOSCINT64_H
