#ifndef QOSCINTERFACE_H
#define QOSCINTERFACE_H

#include "QOSC_global.h"
#include "qoscmethod.h"
#include "qoscpacket.h"
#include "qoscmessage.h"
#include "qoscbundle.h"
#include <QUdpSocket>

class QOSC_EXPORT QOSCInterface :  public QObject
{
    Q_OBJECT

public:
    QOSCInterface(QObject* parent = nullptr);

    inline QHostAddress remoteAddr() const { return _remoteAddr; }
    void setRemoteAddr(const QHostAddress& addr);

    inline quint16 remotePort() const { return _remotePort; }
    void setRemotePort(quint16 p);

    inline QHostAddress localAddr() const { return _localAddr; }

    inline quint16 localPort() const { return _localPort; }
    void setLocalPort(quint16 p);

    inline bool isListening() const { return _isListening; }

    void connect(const QString& addr, QObject* obj, const char* slot);

    template<class Func>
    void connect(const QString& addr, Func f)
    {
        _methods.append(QOSCMethod::ptr(new QOSCLambdaMethod<Func>(addr, f)));
    }

    void disconnect();
    void disconnect(const QString& addr);

    template<class T>
    void send(const QString& pattern, const T& arg)
    {
        QOSCMessage msg(pattern, arg);
        send(msg);
    }

    template<>
    void send(const QString& pattern, const QOSCValue::ptr& arg)
    {
        QOSCMessage msg(pattern, arg);
        send(msg);
    }

public slots:
    void send(const QOSCPacket::ptr p);

    void send(const QOSCMessage& m);

    void send(const QOSCBundle& b);

signals:
    void remoteAddrChanged(const QHostAddress& addr);
    void remotePortChanged(quint16 port);

    void localAddrChanged(const QHostAddress& addr);
    void localPortChanged(quint16 port);

    void packetReceived(const QOSCPacket::ptr& ptr);

protected:
    void rebind();
    void updateLocalAddr();
    void setLocalAddr(const QHostAddress& addr);
    void send(const QByteArray& data);

protected slots:
    void readReady();
    void processPacket(const QOSCPacket::ptr& p, const QOSCTimeTag* time = nullptr);

    void processMessage(const QOSCMessage::ptr& msg);

    void processBundle(const QOSCBundle::ptr& b, const QOSCTimeTag *time);
    void executeBundle(const QOSCBundle::ptr& b, const QOSCTimeTag *time);

protected:
    QHostAddress _remoteAddr;
    quint16      _remotePort;
    QHostAddress _localAddr;
    quint16      _localPort;

    bool _isListening;

    QList<QOSCMethod::ptr> _methods;

    QUdpSocket _s;
};

Q_DECLARE_METATYPE(QHostAddress);

#endif // QOSCINTERFACE_H
