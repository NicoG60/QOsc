#ifndef QOSCABSTRACTTYPE_H
#define QOSCABSTRACTTYPE_H

#include "QOSC_global.h"
#include "qosctypehelper.h"
#include <QIODevice>
#include <QSharedPointer>
#include <QColor>
#include <QDateTime>

#define QOSC_ACCESS(FType, Type, OSCType, Default) \
    virtual inline Type to##FType()             const { Q_ASSERT_X(is##FType(), "to"#FType, "Not a "#FType); return Default; } \
    virtual inline void from##FType(Type value)       { Q_UNUSED(value); Q_ASSERT_X(false, "from"#FType, "Not a "#FType); } \
    virtual inline bool is##FType()             const { return type == OSCType; }

#define QOSC_ACCESS_IMPL_IO(FType, Type, VarIn, VarOut) \
    inline Type to##FType() const override { return VarOut; }\
    inline void from##FType(Type v) override { VarIn; }

#define QOSC_ACCESS_IMPL_EQ(FType, Type, VarIn, VarOut) QOSC_ACCESS_IMPL_IO(FType, Type, VarIn = v, VarOut)

#define QOSC_ACCESS_IMPL(FType, Type, Var) QOSC_ACCESS_IMPL_IO(FType, Type, Var = v, Var)
#define QOSC_ACCESS_IMPL_CAST(FType, Type, Var) QOSC_ACCESS_IMPL_IO(FType, Type, Var = static_cast<Type>(v), static_cast<Type>(Var))

#define QOSC_OPERATOR(FType, Type) \
    QOSCAbstractType& operator =(Type value) { from##FType(value); return *this; }

#define QOSC_DERIVED_OPERATOR_BASE(Class, Base, Type) \
    Class& operator =(Type value) { Base::operator =(value); return *this; }

#define QOSC_DERIVED_OPERATOR(Class, Type) QOSC_DERIVED_OPERATOR_BASE(Class, QOSCAbstractType, Type)

#define QOSC_TYPE_CTOR_BASE(Class, Base, Type) \
    Class() : Base(Type) {} \
    Class(const Class& copy) = default; \
    Class(Class&& move) = default; \
    Class& operator=(const Class& i) = default; \
    Class& operator=(Class&& i) = default;

#define QOSC_TYPE_CTOR(Class, Type) QOSC_TYPE_CTOR_BASE(Class, QOSCAbstractType, Type)

#define QOSC_TYPE_DATA_CTOR_BASE(Class, Base, OSCType, FType, Type)\
    Class(Type value) : Base(OSCType) { from##FType(value); }

#define QOSC_TYPE_DATA_CTOR(Class, OSCType, FType, Type) QOSC_TYPE_DATA_CTOR_BASE(Class, QOSCAbstractType, OSCType, FType, Type)

class QOSC_EXPORT QOSCAbstractType
{
public:
    typedef QSharedPointer<QOSCAbstractType> ptr;
    typedef QList<ptr> list;

public:
    constexpr QOSCAbstractType(QOSC::Type t) : type(t) {}
    constexpr QOSCAbstractType(const QOSCAbstractType&) = default;
    constexpr QOSCAbstractType(QOSCAbstractType&&) = default;
    virtual ~QOSCAbstractType() {};

    QOSCAbstractType& operator=(const QOSCAbstractType&) { return *this; }
    QOSCAbstractType& operator=(QOSCAbstractType&&) { return *this; }

    virtual void writeTypeTag(QIODevice* dev) const = 0;
    virtual void writeData(QIODevice*) const {}
    virtual void readData(QIODevice*) {};

    QOSC_ACCESS(Int32, qint32, QOSC::Int32Type, 0);
    QOSC_ACCESS(Int,   qint32, QOSC::Int32Type, 0);

    QOSC_ACCESS(Int64, qint64, QOSC::Int64Type, 0l);
    QOSC_ACCESS(Long,  qint64, QOSC::Int64Type, 0l);

    QOSC_ACCESS(Float32, float, QOSC::Float32Type, 0.0f);
    QOSC_ACCESS(Float,   float, QOSC::Float32Type, 0.0f);

    QOSC_ACCESS(Float64, double, QOSC::Float64Type, 0.0);
    QOSC_ACCESS(Double,  double, QOSC::Float64Type, 0.0);

    virtual bool toBool()  const { Q_ASSERT_X(isBool(), "toBool", "Not a Bool"); return false; }
    virtual bool isBool()  const { return type == QOSC::TrueType || type == QOSC::FalseType; }
    virtual bool isTrue()  const { return type == QOSC::TrueType; }
    virtual bool isFalse() const { return type == QOSC::FalseType; }

    QOSC_ACCESS(Char,  char,  QOSC::CharType, '\x00');

    QOSC_ACCESS(Color, QColor, QOSC::ColorType, {});

    QOSC_ACCESS(String, QString, QOSC::StringType, {});
    QOSC_ACCESS(Symbol, QString, QOSC::SymbolType, {});

    QOSC_ACCESS(Blob,      QByteArray, QOSC::BlobType, {});
    QOSC_ACCESS(ByteArray, QByteArray, QOSC::BlobType, {});

    QOSC_ACCESS(DateTime, QDateTime, QOSC::TimeTagType, {});
    QOSC_ACCESS(TimeTag,  QDateTime, QOSC::TimeTagType, {});

    QOSC_OPERATOR(Int32, qint32);
    QOSC_OPERATOR(Int64, qint64);

    QOSC_OPERATOR(Float32, float);
    QOSC_OPERATOR(Float64, double);

    QOSC_OPERATOR(Char,      char);
    QOSC_OPERATOR(Color,     QColor);
    QOSC_OPERATOR(String,    QString);
    QOSC_OPERATOR(ByteArray, QByteArray);
    QOSC_OPERATOR(DateTime,  QDateTime);

    const QOSC::Type type;
};

Q_DECLARE_METATYPE(QOSCAbstractType::ptr)
Q_DECLARE_METATYPE(QOSCAbstractType::list)

#endif // QOSCABSTRACTTYPE_H
