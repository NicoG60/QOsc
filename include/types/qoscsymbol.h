#ifndef QOSCSYMBOL_H
#define QOSCSYMBOL_H

#include "QOSC_global.h"
#include "qoscstring.h"

class QOSC_EXPORT QOSCSymbol : public QOSCString
{
public:
    QOSCSymbol() : QOSCString(QOSC::SymbolType) {}
    QOSCSymbol(const QOSCSymbol& copy) = default;
    QOSCSymbol(QOSCSymbol&& move) = default;

    inline QOSCSymbol(const QString& str) : QOSCString(str, QOSC::SymbolType) {}
    inline QOSCSymbol(QString&& str) : QOSCString(str, QOSC::SymbolType) {}
    inline QOSCSymbol& operator=(const QString& str) { QOSCString::operator=(str); return *this; }
    inline QOSCSymbol& operator=(QString&& str) { QOSCString::operator=(str); return *this; }

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('S'); }
};

#endif // QOSCSYMBOL_H
