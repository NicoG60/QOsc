#ifndef QOSCCHAR_H
#define QOSCCHAR_H

#include "QOSC_global.h"
#include "qoscint32.h"

class QOSC_EXPORT QOSCChar : public QOSCInt32
{
public:
    QOSCChar() : QOSCInt32(QOSC::CharType) {}
    QOSCChar(const QOSCChar& copy) = default;
    QOSCChar(QOSCChar&& move) = default;

    QOSCChar& operator=(const QOSCChar& i) = default;
    QOSCChar& operator=(QOSCChar&& i) = default;

    inline QOSCChar(char c) : QOSCInt32(c, QOSC::CharType) {}
    inline QOSCChar& operator=(char c) { QOSCInt32::operator=(c); return *this; }

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('c'); }
};

#endif // QOSCCHAR_H
