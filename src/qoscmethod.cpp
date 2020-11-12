#include <qoscmethod.h>
#include <QMetaObject>

QOSCSlotMethod::QOSCSlotMethod(const QString& addr, QObject* obj, const char* slot) :
    QOSCMethod(addr),
    _obj(obj),
    _slot(slot)
{

}

void QOSCSlotMethod::call(const QOSCMessage::ptr& msg)
{
    QMetaObject::invokeMethod(_obj, _slot, Q_ARG(const QOSCMessage::ptr&, msg));
}
