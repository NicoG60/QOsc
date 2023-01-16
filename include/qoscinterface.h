#ifndef QOSCINTERFACE_H
#define QOSCINTERFACE_H

#include "qoscmethod.h"
#include "qoscmessage.h"
#include "qoscbundle.h"
#include <QHostAddress>
#include <memory>

class QOscInterfacePrivate;
class QOSC_EXPORT QOscInterface :  public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString remoteAddr READ remoteAddr WRITE setRemoteAddr NOTIFY remoteAddrChanged)
    Q_PROPERTY(quint16 remotePort READ remotePort WRITE setRemotePort NOTIFY remotePortChanged)
    Q_PROPERTY(QString localAddr READ localAddr NOTIFY localAddrChanged)
    Q_PROPERTY(quint16 localPort READ localPort WRITE setLocalPort NOTIFY localPortChanged)
    Q_PROPERTY(bool isListening READ isListening NOTIFY isListeningChanged)

public:
    QOscInterface(QObject* parent = nullptr);
    ~QOscInterface() override;

    QString remoteAddr() const;
    void setRemoteAddr(const QString& addr);

    quint16 remotePort() const;
    void setRemotePort(quint16 p);

    QString localAddr() const;

    quint16 localPort() const;
    void setLocalPort(quint16 p);

    bool isListening() const;

    void connect(const QString& addr, QObject* obj, const char* slot);

    template<class Func>
    void connect(const QString& addr, Func f)
    {
        connect(QOscMethod::ptr(new QOscLambdaMethod<Func>(addr, f)));
    }

    void disconnect();
    void disconnect(const QString& addr);

    template<class T>
    void send(const QString& pattern, const T& arg)
    {
        QOscMessage msg(pattern, arg);
        send(msg);
    }

public slots:
    void send(const QOscMessage& m);

    void send(const QOscBundle& b);

signals:
    void remoteAddrChanged(const QString& addr);
    void remotePortChanged(quint16 port);

    void localAddrChanged(const QString& addr);
    void localPortChanged(quint16 port);

    void isListeningChanged();

    void messageReceived(const QOscMessage& msg);
    void bundleReceived(const QOscBundle& bundle);

    void messageSent();

private:
    Q_DECLARE_PRIVATE(QOscInterface);
    Q_DISABLE_COPY(QOscInterface);
    std::unique_ptr<QOscInterfacePrivate> d_ptr;

    void connect(const QOscMethod::ptr& method);

private slots:
    void readReady();
};

#endif // QOSCINTERFACE_H
