#ifndef QOSCMETHOD_H
#define QOSCMETHOD_H

#include "qoscmessage.h"

class QOSC_EXPORT QOSCMethod
{
public:
    typedef QSharedPointer<QOSCMethod> ptr;

    QOSCMethod(const QString& addr) : addr(addr) {};
    virtual ~QOSCMethod() {};

    virtual void call(const QOSCMessage::ptr& msg) = 0;

    const QString addr;
};

class QOSC_EXPORT QOSCSlotMethod : public QOSCMethod
{
public:
    QOSCSlotMethod(const QString& addr, QObject* obj, const char* slot);

    void call(const QOSCMessage::ptr& msg) override;

private:
    QObject*    _obj;
    const char* _slot;
};

template<class Func>
class QOSC_EXPORT QOSCLambdaMethod : public QOSCMethod
{
public:
    QOSCLambdaMethod(const QString& addr, Func f) : QOSCMethod(addr), _f(f) {}

    inline void call(const QOSCMessage::ptr& msg) override { _f(msg); }

private:
    Func _f;
};

#endif // QOSCMETHOD_H
