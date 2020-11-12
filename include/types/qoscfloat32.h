#ifndef QOSCFLOAT32_H
#define QOSCFLOAT32_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include "qosctypehelper.h"

class QOSC_EXPORT QOSCFloat32 : public QOSCAbstractType
{
public:
    QOSC_TYPE_CTOR(QOSCFloat32, QOSC::Float32Type);

    QOSC_TYPE_DATA_CTOR(QOSCFloat32, QOSC::Float32Type, Float, float)

    QOSC_ACCESS_IMPL_CAST(Int,   qint32, _f);
    QOSC_ACCESS_IMPL_CAST(Int32, qint32, _f);

    QOSC_ACCESS_IMPL_CAST(Long,  qint64, _f);
    QOSC_ACCESS_IMPL_CAST(Int64, qint64, _f);

    QOSC_ACCESS_IMPL(Float,   float, _f);
    QOSC_ACCESS_IMPL(Float32, float, _f);

    QOSC_ACCESS_IMPL(Double,  double, _f);
    QOSC_ACCESS_IMPL(Float64, double, _f);

    QOSC_DERIVED_OPERATOR(QOSCFloat32, qint32);
    QOSC_DERIVED_OPERATOR(QOSCFloat32, qint64);
    QOSC_DERIVED_OPERATOR(QOSCFloat32, float);
    QOSC_DERIVED_OPERATOR(QOSCFloat32, double);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('f'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _f); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_f); }

private:
    float _f;
};

#endif // QOSCFLOAT32_H
