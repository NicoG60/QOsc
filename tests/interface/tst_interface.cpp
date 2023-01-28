#include <QtTest>
#include <QCoreApplication>

#include <QOsc>
#include <QUdpSocket>
#include <QNetworkDatagram>

class interface : public QObject
{
    Q_OBJECT

public:
    interface() = default;
    ~interface() override = default;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_send();
    void test_receive();
    void test_connect_dispatch();
    void test_time_bundle();

    // These guyes are just here tot est if the examples in the README actually
    // build
    void test_readme_server();
    void test_readme_client();

public:
    void verify_send(const QByteArray& comp);
    void send(const QByteArray& comp);
    QByteArray msgSocketError(const QAbstractSocket &s);
    QOscMessage magicMessage();
    QOscBundle magicBundle();

public:
    const QString localAddr{"127.0.0.1"};
    const quint16 localPort = 8000;
    const QString remoteAddr{"127.0.0.1"};
    const quint16 remotePort = 9000;

    QOscInterface test;
    QUdpSocket    echo;
};

void interface::initTestCase()
{
    QSignalSpy spyLocalPort(&test, SIGNAL(localPortChanged(quint16)));
    QSignalSpy spyLocalAddr(&test, SIGNAL(localAddrChanged(QString)));
    QSignalSpy spyRemotePort(&test, SIGNAL(remotePortChanged(quint16)));
    QSignalSpy spyRemoteAddr(&test, SIGNAL(remoteAddrChanged(QString)));

    test.setLocalPort(localPort);
    test.setRemoteAddr(remoteAddr);
    test.setRemotePort(remotePort);

    QVERIFY(QTest::qWaitFor([=](){return test.isListening();}));

    QCOMPARE(spyLocalPort.count(), 1);
    QCOMPARE(spyLocalAddr.count(), 0);
    QCOMPARE(spyRemotePort.count(), 1);
    QCOMPARE(spyRemoteAddr.count(), 0);

    QCOMPARE(spyLocalPort[0][0].toInt(), localPort);
    QCOMPARE(spyRemotePort[0][0].toInt(), remotePort);

    QCOMPARE(test.localAddr(), localAddr);
    QCOMPARE(test.localPort(), localPort);
    QCOMPARE(test.remoteAddr(), remoteAddr);
    QCOMPARE(test.remotePort(), remotePort);

    echo.bind(QHostAddress(remoteAddr), remotePort);
    QVERIFY(QTest::qWaitFor([=](){return echo.isValid();}));
}

void interface::cleanupTestCase() {}

void interface::test_send()
{
    test.send("/a/b/c", 10);
    verify_send(magicMessage().package());

    QOscValue value(10);
    test.send("/a/b/c", value);
    verify_send(magicMessage().package());

    QOscMessage msg("/a/b/c", value);
    test.send(msg);
    verify_send(magicMessage().package());

    QOscBundle b;
    b.setTime(QOscValue::asap());
    b << QOscMessage("/a/b/c", value);

    test.send(b);
    verify_send(QByteArray("#bundle\x00" // bundle mark
                           "\x00\x00\x00\x00\x00\x00\x00\x01" // timestamp
                           "\x00\x00\x00\x10" // first element size
                           "/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A" // first element content
                           "\x00" // padding
                           , 36));
}

void interface::test_receive()
{
    QSignalSpy spyMsg(&test, SIGNAL(messageReceived(const QOscMessage&)));
    QSignalSpy spyBundle(&test, SIGNAL(bundleReceived(const QOscBundle&)));

    {
        send(magicMessage().package());
        QVERIFY(spyMsg.wait());
        auto msg = spyMsg.last().first().value<QOscMessage>();

        QCOMPARE(msg.pattern(), "/a/b/c");
        QCOMPARE(msg.toInt(), 10);
    }

    {
        send(magicBundle().package());
        QVERIFY(spyBundle.wait());
        auto bundle = spyBundle.last().first().value<QOscBundle>();

        QVERIFY(bundle.time().isNow());

        QCOMPARE(bundle.size(), 2);

        for(auto& e : bundle)
        {
            QCOMPARE(e.pattern(), "/a/b/c");
            QCOMPARE(e.type(), QOsc::Int32Type);
            QCOMPARE(e.toInt(), 10);
        }
    }
}

void interface::test_connect_dispatch()
{
    bool abc = false;
    bool adc = false;

    test.connect("/a/b/c",
    [&](const QOscMessage& m)
    {
        QCOMPARE(m.pattern(), "/a/?/c");
        QCOMPARE(m.toInt(), 10);
        abc = true;
    });

    test.connect("/a/d/c",
    [&](const QOscMessage& m)
    {
        QCOMPARE(m.pattern(), "/a/?/c");
        QCOMPARE(m.toInt(), 10);
        adc = true;
    });

    auto msg = magicMessage();
    msg.setPattern("/a/?/c");
    send(msg.package());

    QVERIFY(QTest::qWaitFor([&](){ return abc && adc; }));

    test.disconnect();
}

void interface::test_time_bundle()
{
    int abc = 0;

    test.connect("/a/b/c",
    [&](const QOscMessage& m)
    {
        QCOMPARE(m.pattern(), "/a/b/c");
        QCOMPARE(m.toInt(), 10);
        abc++;
    });

    send(magicBundle().package());

    QVERIFY(QTest::qWaitFor([&](){ return abc == 2; }, 100));

    auto bundle = magicBundle();
    bundle.setTime(QDateTime::currentDateTime().addSecs(10));

    QElapsedTimer t;
    t.start();
    send(bundle.package());

    QVERIFY(QTest::qWaitFor([&](){ return abc > 2; }, 11000));
    qDebug() << t.elapsed();
}

void interface::test_readme_server()
{
    // Bind the network interface so you can send and get messages
    QOscInterface iface;
    iface.setRemoteAddr("192.168.0.10");
    iface.setRemotePort(9000);
    iface.setLocalPort(8000);

    // Connect callbacks to get notified of new messages
    iface.connect("/my/osc/pattern",
    [](const QOscMessage& msg)
    {
        // This is the message callback
        // It'll be called each time a message matching the
        // pattern you set is received.

        for(auto& arg : msg)
        {
            if(!arg.isInt())
                continue;

            int i = arg.toInt();

            // do stuff with i

            Q_UNUSED(i);
        }
    });

//    // Alternatively you can use any QObject slots
//    QObject obj;
//    iface.connect("/my/other/pattern", &obj, SLOT(mySlot(const QOscMessage& msg)));

//    // Or get all messages and dispatch them yourself
//    QObject dispatcher;

//    QObject::connect(&iface,      SIGNAL(messageReceived(const QOscMessage&)),
//                     &dispatcher, SLOT(dispatchMessage(const QOscMessage&)));

//    QObject::connect(&iface,      SIGNAL(bundleReceived(const QOscBundle&)),
//                     &dispatcher, SLOT(dispatchBundle(const QOscBundle&)));
}

void interface::test_readme_client()
{
    // Bind the network interface so you can send and get messages
    QOscInterface iface;
    iface.setRemoteAddr("192.168.0.10");
    iface.setRemotePort(9000);
    iface.setLocalPort(8000);

    // Craft the message you want to send
    QOscMessage msg("/my/osc/pattern", QString("Some random string"));

    iface.send(msg);

    // More complex messages with several values
    QOscValue myInt(10);
    QOscValue myMidi = QOscValue::midiValue(/*port:   0       */ qint8(0x00),
                                            /*status: note on */ qint8(0x90),
                                            /*data1:  note A4 */ qint8(0x45),
                                            /*data2:  note vel*/ qint8(0x10));
    QOscValue myColor(QColor(0, 255, 0));

    QOscMessage msg2("/my/osc/pattern");
    msg2 << myInt << myMidi << myColor;

    iface.send(msg2);

    // You can also create bundles
    // They are useful to deliver several messages to different patterns
    // They can also be timed, You can specify an absolute Date and Time when you want them to be executed

    QOscBundle bundle;
    bundle.setTime(QDateTime::currentDateTime().addSecs(15)); // The bundle is for 15sec later
    bundle << QOscMessage("/osc/pattern1", QString("Some random string"))
           << QOscMessage("/osc/pattern2", 125)
           << QOscMessage("/osc/pattern3", true)
           << QOscMessage("/osc/pattern4", 3.14);

    iface.send(bundle);
}

void interface::verify_send(const QByteArray& comp)
{
    QVERIFY2(echo.waitForReadyRead(10000), msgSocketError(echo).constData());

    QVERIFY(echo.hasPendingDatagrams());

    auto datagram = echo.receiveDatagram();

    QVERIFY(!echo.hasPendingDatagrams());

    QCOMPARE(datagram.data(), comp);
}

void interface::send(const QByteArray& comp)
{
    echo.writeDatagram(comp, QHostAddress(localAddr), localPort);
}

QByteArray interface::msgSocketError(const QAbstractSocket &s)
{
    QString result;
    QDebug debug(&result);
    debug.nospace();
    debug.noquote();
    if (!s.localAddress().isNull())
        debug << "local=" << s.localAddress().toString() << ':' << s.localPort();
    if (!s.peerAddress().isNull())
        debug << ", peer=" << s.peerAddress().toString() << ':' << s.peerPort();
    debug << ", type=" << s.socketType() << ", state=" << s.state()
        << ", error=" << s.error() << ": " << s.errorString();
   return result.toLocal8Bit();
}

QOscMessage interface::magicMessage()
{
    return QOscMessage("/a/b/c", 10);
}

QOscBundle interface::magicBundle()
{
    QOscMessage msg("/a/b/c", 10);
    QOscBundle b1;
    QOscBundle b2;

    b1.setTime(QOscValue::asap());
    b1 << msg;

    b2.setTime(QOscValue::asap());
    b2 << msg;

    b1 << b2;

    return b1;
}

QTEST_MAIN(interface)

#include "tst_interface.moc"
