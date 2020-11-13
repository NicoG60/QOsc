#include <qoschelper.h>
#include <types/qosctypes.h>

namespace QOSC
{

QOSCValue::ptr makeValue(const QByteArray& b)
{
    return QOSCValue::ptr(new QOSCBlob(b));
}

QOSCValue::ptr makeValue(const QString& str)
{
    return QOSCValue::ptr(new QOSCString(str));
}

QOSCValue::ptr makeValue(char c)
{
    return QOSCValue::ptr(new QOSCChar(c));
}

QOSCValue::ptr makeValue(qint32 i32)
{
    return QOSCValue::ptr(new QOSCInt32(i32));
}

QOSCValue::ptr makeValue(qint64 i64)
{
    return QOSCValue::ptr(new QOSCInt64(i64));
}

QOSCValue::ptr makeValue(float f)
{
    return QOSCValue::ptr(new QOSCFloat32(f));
}

QOSCValue::ptr makeValue(double d)
{
    return QOSCValue::ptr(new QOSCFloat64(d));
}

QOSCValue::ptr makeValue(const QDateTime& dt)
{
    return QOSCValue::ptr(new QOSCTimeTag(dt));
}

QOSCValue::ptr makeValue(const QColor& c)
{
    return QOSCValue::ptr(new QOSCColor(c));
}

QOSCValue::ptr makeValue(qint8 p, qint8 s, qint8 d1, qint8 d2)
{
    return QOSCValue::ptr(new QOSCMidi(p, s, d1, d2));
}

QOSCValue::ptr makeValue(bool b)
{
    if(b)
        return QOSCValue::ptr(new QOSCTrue());
    else
        return QOSCValue::ptr(new QOSCFalse());
}

QOSCValue::ptr makeValue(std::nullptr_t ptr)
{
    Q_UNUSED(ptr);
    return QOSCValue::ptr(new QOSCNil());
}

}

