#ifndef QOSCSYMBOL_H
#define QOSCSYMBOL_H

#include "QOSC_global.h"
#include "qoscstring.h"

class QOSC_EXPORT QOSCSymbol : public QOSCString
{
public:
    QOSC_TYPE_CTOR_BASE(QOSCSymbol, QOSCString, QOSC::SymbolType);

    QOSC_TYPE_DATA_CTOR_BASE(QOSCSymbol, QOSCString, QOSC::SymbolType, String, const QString&)
    QOSC_TYPE_DATA_CTOR_BASE(QOSCSymbol, QOSCString, QOSC::SymbolType, String, QString&&)

    QOSC_DERIVED_OPERATOR(QOSCSymbol, QString);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('S'); }
};

#endif // QOSCSYMBOL_H
