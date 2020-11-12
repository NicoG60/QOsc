#ifndef QOSCCHAR_H
#define QOSCCHAR_H

#include "QOSC_global.h"
#include "qoscint32.h"

class QOSC_EXPORT QOSCChar : public QOSCInt32
{
public:
    QOSC_TYPE_CTOR_BASE(QOSCChar, QOSCInt32, QOSC::CharType)

    QOSC_TYPE_DATA_CTOR_BASE(QOSCChar, QOSCInt32, QOSC::CharType, Char, char)

    QOSC_ACCESS_IMPL(Char, char, _i);

    QOSC_DERIVED_OPERATOR_BASE(QOSCChar, QOSCInt32, qint32);
    QOSC_DERIVED_OPERATOR_BASE(QOSCChar, QOSCInt32, qint64);
    QOSC_DERIVED_OPERATOR_BASE(QOSCChar, QOSCInt32, char);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('c'); }
};

#endif // QOSCCHAR_H
