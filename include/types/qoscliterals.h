#ifndef QOSCLITERALS_H
#define QOSCLITERALS_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"

template<char C, QOSC::Type T>
class QOSC_EXPORT QOSCLiteral : public QOSCAbstractType
{
public:
    constexpr QOSCLiteral() : QOSCAbstractType(T) {}
    constexpr QOSCLiteral(const QOSCLiteral& copy) = default;
    constexpr QOSCLiteral(QOSCLiteral&& move) = default;

    QOSCLiteral& operator=(const QOSCLiteral& i) = default;
    QOSCLiteral& operator=(QOSCLiteral&& i) = default;

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar(C); }

    constexpr inline operator bool() const { return C == 'T' || C == 'I'; }
};

typedef QOSCLiteral<'T', QOSC::TrueType>      QOSCTrue;
typedef QOSCLiteral<'F', QOSC::FalseType>     QOSCFalse;
typedef QOSCLiteral<'N', QOSC::NilType>       QOSCNil;
typedef QOSCLiteral<'I', QOSC::InfinitumType> QOSCInfinitum;

#endif // QOSCLITERALS_H
