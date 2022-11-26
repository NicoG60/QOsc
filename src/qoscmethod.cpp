#include <qoscmethod.h>
#include <QMetaObject>

QOscSlotMethod::QOscSlotMethod(const QString& addr, QObject* obj, const char* slot) :
    QOscMethod(addr),
    _obj(obj),
    _slot(slot)
{

}

void QOscSlotMethod::call(const QOscMessage& msg)
{
    QMetaObject::invokeMethod(_obj, _slot, Qt::DirectConnection, Q_ARG(QOscMessage, msg));
}
