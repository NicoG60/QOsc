#ifndef QOSCTIMETAG_H
#define QOSCTIMETAG_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include <QDateTime>

class QOSC_EXPORT QOSCTimeTag : public QOSCAbstractType
{
    constexpr static quint64 unix_to_ntp = 2208988800ul;
    constexpr static quint64 factor = 1ul << 32;

public:
    QOSCTimeTag() : QOSCAbstractType(QOSC::TimeTagType) {}
    QOSCTimeTag(const QOSCTimeTag& copy) = default;
    QOSCTimeTag(QOSCTimeTag&& move) = default;

    static inline QOSCTimeTag now()  { return {1ull}; };
    static inline QOSCTimeTag asap() { return {1ull}; };

    QOSCTimeTag& operator=(const QOSCTimeTag& i) = default;
    QOSCTimeTag& operator=(QOSCTimeTag&& i) = default;

    inline QOSCTimeTag(quint64 t) : QOSCAbstractType(QOSC::TimeTagType), _t(t) {}

    inline QOSCTimeTag(double t) : QOSCAbstractType(QOSC::TimeTagType), _t(t >= 0 ? static_cast<quint64>(t*factor) : 1) {}

    inline QOSCTimeTag(const QDateTime& t) :
        QOSCTimeTag((t.toMSecsSinceEpoch()/1000.0)+unix_to_ntp)
    { }

    inline operator quint64() const { return _t; }
    inline operator double() const { return _t/static_cast<double>(factor); }
    inline operator QDateTime() const { return QDateTime::fromMSecsSinceEpoch((operator double() - unix_to_ntp)*1000); }

    inline bool isNow() const { return (_t == 1ull); }

    inline QOSCTimeTag& operator=(qint64 t) { _t = t; return *this; }
    inline QOSCTimeTag& operator=(double t) { *this = QOSCTimeTag(t); return *this; }
    inline QOSCTimeTag& operator=(const QDateTime& t) { *this = QOSCTimeTag(t); return *this; }

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('t'); }
    inline void writeData(QIODevice* dev) const override { QOSC::writeHelper(dev, _t); }

    inline void readData(QIODevice* dev) override { QOSC::readHelper(dev, &_t); }

private:
    quint64 _t = {};
};

#endif // QOSCTIMETAG_H
