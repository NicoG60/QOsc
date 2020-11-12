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
    QOSC_TYPE_CTOR(QOSCInt32, QOSC::Int32Type);

    QOSC_TYPE_DATA_CTOR(QOSCInt32, QOSC::Int32Type, Int, qint32)

    QOSC_ACCESS_IMPL(Int,   qint32, _i);
    QOSC_ACCESS_IMPL(Int32, qint32, _i);

    QOSC_ACCESS_IMPL(Long,  qint64, _i);
    QOSC_ACCESS_IMPL(Int64, qint64, _i);

    QOSC_ACCESS_IMPL_CAST(Float,   float, _i);
    QOSC_ACCESS_IMPL_CAST(Float32, float, _i);

    QOSC_ACCESS_IMPL_CAST(Double,  double, _i);
    QOSC_ACCESS_IMPL_CAST(Float64, double, _i);

    QOSC_DERIVED_OPERATOR(QOSCInt32, qint32);
    QOSC_DERIVED_OPERATOR(QOSCInt32, qint64);
    QOSC_DERIVED_OPERATOR(QOSCInt32, float);
    QOSC_DERIVED_OPERATOR(QOSCInt32, double);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('i'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _i); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_i); }

protected:
    qint32 _i;
};

#endif // QOSCINT32_H
