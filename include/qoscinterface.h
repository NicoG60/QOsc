#ifndef QOSCINTERFACE_H
#define QOSCINTERFACE_H

#include "qoscmethod.h"
#include "qoscmessage.h"
#include "qoscbundle.h"
#include <QHostAddress>

class QOscInterfacePrivate;
class QOscInterface :  public QObject
{
    Q_OBJECT

public:
    QOscInterface(QObject* parent = nullptr);

    QHostAddress remoteAddr() const;
    void setRemoteAddr(const QHostAddress& addr);

    quint16 remotePort() const;
    void setRemotePort(quint16 p);

    QHostAddress localAddr() const;

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

    template<>
    void send(const QString& pattern, const QOscValue& arg)
    {
        QOscMessage msg(pattern, arg);
        send(msg);
    }

public slots:
    void send(const QOscMessage& m);

    void send(const QOscBundle& b);

signals:
    void remoteAddrChanged(const QHostAddress& addr);
    void remotePortChanged(quint16 port);

    void localAddrChanged(const QHostAddress& addr);
    void localPortChanged(quint16 port);

    void messageReceived(const QOscMessage& msg);
    void bundleReceived(const QOscBundle& bundle);

private:
    Q_DECLARE_PRIVATE(QOscInterface);
    Q_DISABLE_COPY(QOscInterface);
    void connect(QOscMethod::ptr method);

private slots:
    void readReady();
};

Q_DECLARE_METATYPE(QHostAddress);

#endif // QOSCINTERFACE_H
