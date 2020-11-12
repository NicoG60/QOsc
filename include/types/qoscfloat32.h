#ifndef QOSCFLOAT32_H
#define QOSCFLOAT32_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include "qosctypehelper.h"

class QOSC_EXPORT QOSCFloat32 : public QOSCAbstractType
{
public:
    QOSCFloat32() : QOSCAbstractType(QOSC::Float32Type) {}
    QOSCFloat32(const QOSCFloat32& copy) = default;
    QOSCFloat32(QOSCFloat32&& move) = default;

    inline QOSCFloat32(float f) : QOSCAbstractType(QOSC::Float32Type), _f(f) {}

    inline operator float() const { return _f; }

    inline QOSCFloat32& operator=(float f) { _f = f; return *this; };

    QOSCFloat32& operator=(const QOSCFloat32& i) = default;
    QOSCFloat32& operator=(QOSCFloat32&& i) = default;

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('f'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _f); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_f); }

private:
    float _f;
};

#endif // QOSCFLOAT32_H
