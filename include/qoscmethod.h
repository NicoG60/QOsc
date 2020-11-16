#ifndef QOSCMETHOD_H
#define QOSCMETHOD_H

#include "qoscmessage.h"

class QOSC_EXPORT QOscMethod
{
public:
    typedef QSharedPointer<QOscMethod> ptr;

    QOscMethod(const QString& addr) : addr(addr) {};
    virtual ~QOscMethod() {};

    virtual void call(const QOscMessage& msg) = 0;

    const QString addr;
};

class QOSC_EXPORT QOscSlotMethod : public QOscMethod
{
public:
    QOscSlotMethod(const QString& addr, QObject* obj, const char* slot);

    void call(const QOscMessage& msg) override;

private:
    QObject*    _obj;
    const char* _slot;
};

template<class Func>
class QOscLambdaMethod : public QOscMethod
{
public:
    QOscLambdaMethod(const QString& addr, Func f) : QOscMethod(addr), _f(f) {}

    inline void call(const QOscMessage& msg) override { _f(msg); }

private:
    Func _f;
};

#endif // QOSCMETHOD_H
