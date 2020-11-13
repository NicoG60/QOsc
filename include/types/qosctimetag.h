#ifndef QOSCTIMETAG_H
#define QOSCTIMETAG_H

#include "QOSC_global.h"
#include "qoscvalue.h"
#include <QDateTime>

class QOSC_EXPORT QOSCTimeTag : public QOSCValue
{
    constexpr static quint64 unix_to_ntp = 2208988800ul;
    constexpr static quint64 factor = 1ul << 32;

public:
    QOSC_TYPE_CTOR(QOSCTimeTag, QOSC::TimeTagType);

    QOSC_TYPE_DATA_CTOR(QOSCTimeTag, QOSC::TimeTagType, Double, double)
    QOSC_TYPE_DATA_CTOR(QOSCTimeTag, QOSC::TimeTagType, Uint64, quint64)
    QOSC_TYPE_DATA_CTOR(QOSCTimeTag, QOSC::TimeTagType, DateTime, const QDateTime&)
    QOSC_TYPE_DATA_CTOR(QOSCTimeTag, QOSC::TimeTagType, DateTime, QDateTime&&)

    // Let them be const ref to static var so I can get pointer to them
    static inline const QOSCTimeTag& now()  { static QOSCTimeTag t(1ull); return t; };
    static inline const QOSCTimeTag& asap() { return now(); };

    inline quint64 toUint64() const   { return _t; }
    inline void fromUint64(quint64 v) { _t = v; }

    QOSC_ACCESS_IMPL_IO(Double, double, _t = (v >= 0 ? static_cast<quint64>(v*factor) : 1),
                                        _t/static_cast<double>(factor));
    QOSC_ACCESS_IMPL_IO(DateTime, QDateTime, fromDouble((v.toMSecsSinceEpoch()/1000.0)+unix_to_ntp),
                                             QDateTime::fromMSecsSinceEpoch((toDouble() - unix_to_ntp)*1000))

    inline bool isNow() const { return (_t == 1ull); }

    QOSCTimeTag& operator =(quint64 value) { fromUint64(value); return *this; }
    operator quint64() const { return toUint64(); }
    QOSC_DERIVED_OPERATOR(QOSCTimeTag, double);
    QOSC_DERIVED_OPERATOR(QOSCTimeTag, QDateTime);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('t'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _t); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_t); }

private:
    quint64 _t = {};
};

#endif // QOSCTIMETAG_H
