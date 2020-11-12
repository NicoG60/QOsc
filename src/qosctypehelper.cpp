#include <types/qosctypehelper.h>

#include <types/qosctypes.h>

namespace QOSC
{

QOSCAbstractType::ptr make(const QByteArray& b)
{
    return QOSCAbstractType::ptr(new QOSCBlob(b));
}

QOSCAbstractType::ptr make(const QString& str)
{
    return QOSCAbstractType::ptr(new QOSCString(str));
}

QOSCAbstractType::ptr make(char c)
{
    return QOSCAbstractType::ptr(new QOSCChar(c));
}

QOSCAbstractType::ptr make(qint32 i32)
{
    return QOSCAbstractType::ptr(new QOSCInt32(i32));
}

QOSCAbstractType::ptr make(qint64 i64)
{
    return QOSCAbstractType::ptr(new QOSCInt64(i64));
}

QOSCAbstractType::ptr make(float f)
{
    return QOSCAbstractType::ptr(new QOSCFloat32(f));
}

QOSCAbstractType::ptr make(double d)
{
    return QOSCAbstractType::ptr(new QOSCFloat64(d));
}

QOSCAbstractType::ptr make(const QDateTime& dt)
{
    return QOSCAbstractType::ptr(new QOSCTimeTag(dt));
}

QOSCAbstractType::ptr make(const QColor& c)
{
    return QOSCAbstractType::ptr(new QOSCColor(c));
}

QOSCAbstractType::ptr make(qint8 p, qint8 s, qint8 d1, qint8 d2)
{
    return QOSCAbstractType::ptr(new QOSCMidi(p, s, d1, d2));
}

QOSCAbstractType::ptr make(bool b)
{
    if(b)
        return QOSCAbstractType::ptr(new QOSCTrue());
    else
        return QOSCAbstractType::ptr(new QOSCFalse());
}

QOSCAbstractType::ptr make(std::nullptr_t ptr)
{
    Q_UNUSED(ptr);
    return QOSCAbstractType::ptr(new QOSCNil());
}

}

