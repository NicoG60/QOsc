#ifndef QOSCLITERALS_H
#define QOSCLITERALS_H

#include "QOSC_global.h"
#include "qoscvalue.h"

template<char C, QOSC::Type T>
class QOSC_EXPORT QOSCLiteral : public QOSCValue
{
public:
    QOSC_TYPE_CTOR(QOSCLiteral, T);

    virtual bool toBool() const override { return C == 'T' || C == 'I'; }

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar(C); }
};

typedef QOSCLiteral<'T', QOSC::TrueType>      QOSCTrue;
typedef QOSCLiteral<'F', QOSC::FalseType>     QOSCFalse;
typedef QOSCLiteral<'N', QOSC::NilType>       QOSCNil;
typedef QOSCLiteral<'I', QOSC::InfinitumType> QOSCInfinitum;

#endif // QOSCLITERALS_H
