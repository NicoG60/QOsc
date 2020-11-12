#ifndef QOSCINT64_H
#define QOSCINT64_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include "qosctypehelper.h"

class QOSC_EXPORT QOSCInt64 : public QOSCAbstractType
{
public:
    QOSCInt64() : QOSCAbstractType(QOSC::Int64Type) {}
    QOSCInt64(const QOSCInt64& copy) = default;
    QOSCInt64(QOSCInt64&& move) = default;

    QOSCInt64& operator=(const QOSCInt64& i) = default;
    QOSCInt64& operator=(QOSCInt64&& i) = default;

    inline QOSCInt64(qint64 i) : QOSCAbstractType(QOSC::Int64Type), _i(i) {}

    inline operator qint64() const { return _i; }

    inline QOSCInt64& operator=(qint64 i) { _i = i; return *this; };

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('h'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _i); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_i); }

private:
    qint64 _i = {};
};

#endif // QOSCINT64_H
