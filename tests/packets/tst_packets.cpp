#include <QtTest>

#include <qoscmessage.h>
#include <qoscbundle.h>
#include <types/qosctypes.h>

#include <QDebug>

class packets : public QObject
{
    Q_OBJECT

public:
    packets();
    ~packets();

private slots:
    void test_message();

    void test_match_data();
    void test_match();

    void test_bundle();

};

packets::packets() {}

packets::~packets() {}

void packets::test_message()
{
    QOSCMessage msg("/a/b/c", 10);

    QVERIFY(msg.isValid());

    QVERIFY(msg.args.size() == 1);

    auto a = msg.args[0];
    auto i = a.dynamicCast<QOSCInt32>();

    QVERIFY(i);
    QCOMPARE(i->toInt(), 10);

    QByteArray data = msg.package();

    QCOMPARE(data, QByteArray("/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A", 16));

    auto packet = QOSCPacket::read(QByteArray("/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A", 16));

    QCOMPARE(packet->type, QOSCPacket::OSCMessage);

    auto msg2 = packet.dynamicCast<QOSCMessage>();

    QCOMPARE(msg2->pattern, "/a/b/c");
    QCOMPARE(msg2->args[0]->type, QOSC::Int32Type);
    QCOMPARE(msg2->args[0]->toInt(), 10);
}

void packets::test_match_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("matcher");
    QTest::addColumn<QStringList>("positive");
    QTest::addColumn<QStringList>("negative");

    QTest::newRow("whole address") << "/a/b/c"
                                   << "^/a/b/c$"
                                   << QStringList{"/a/b/c"}
                                   << QStringList{"/", "/a", "/a/b", "/a/b/", "/a/c/b", "/b/a/c", "//", "/a//b"};

    QTest::newRow("1 char part")   << "/a/?/c"
                                   << "^/a/./c$"
                                   << QStringList{"/a/b/c", "/a/f/c", "/a/3/c", "/a/l/c"}
                                   << QStringList{"/", "/a", "/a/b", "/a/b/", "/a/c/b", "/b/a/c", "//", "/a//c"};

    QTest::newRow("1 multi char part")   << "/a/a?c/c"
                                   << "^/a/a.c/c$"
                                   << QStringList{"/a/abc/c", "/a/afc/c", "/a/a3c/c", "/a/a.c/c"}
                                   << QStringList{"/", "/a", "/a/b", "/a/b/", "/a/c/b", "/b/a/c", "//", "/a//c"};

    QTest::newRow("wildcard")      << "/a/reg*"
                                   << "^/a/reg[^/]*$"
                                   << QStringList{"/a/reg", "/a/regedit", "/a/regex", "/a/regexpr", "/a/reg.com"}
                                   << QStringList{"/", "/a", "/a/b", "/a/b/", "/a/c/b", "/b/a/c", "//", "/a//c", "/a/regtruc/c"};

    QTest::newRow("class")         << "/a/k[ia]t/c"
                                   << "^/a/k[ia]t/c$"
                                   << QStringList{"/a/kit/c", "/a/kat/c"}
                                   << QStringList{"/", "/a", "/a/b", "/a/b/", "/a/c/b", "/b/a/c", "//", "/a//c", "/a/ket/c"};

    QTest::newRow("not class")     << "/a/k[!ia]t/c"
                                   << "^/a/k[^ia]t/c$"
                                   << QStringList{"/a/kot/c", "/a/kut/c", "/a/k.t/c"}
                                   << QStringList{"/", "/a", "/a/b", "/a/b/", "/a/c/b", "/b/a/c", "//", "/a//c", "/a/kit/c", "/a/kat/c"};

    QTest::newRow("choice")        << "/a/{foo,bar}/c"
                                   << "^/a/(foo|bar)/c$"
                                   << QStringList{"/a/foo/c", "/a/bar/c"}
                                   << QStringList{"/", "/a", "/a/b", "/a/b/", "/a/c/b", "/b/a/c", "//", "/a//c", "/a/kit/c", "/a/kat/c"};
}

void packets::test_match()
{
    QFETCH(QString,     pattern);
    QFETCH(QString,     matcher);
    QFETCH(QStringList, positive);
    QFETCH(QStringList, negative);

    QOSCMessage msg(pattern, 10);

    QCOMPARE(msg.matcher().pattern(), matcher);

    for(auto& addr : positive)
        QVERIFY(msg.match(addr));

    for(auto& addr : negative)
        QVERIFY(!msg.match(addr));
}

void packets::test_bundle()
{
    QOSCMessage::ptr msg(new QOSCMessage("/a/b/c", 10));
    QOSCBundle::ptr b1(new QOSCBundle());
    QOSCBundle::ptr b2(new QOSCBundle());

    b1->time = QOSCTimeTag::asap();
    b1->elements << msg;

    b2->time = QOSCTimeTag::asap();
    b2->elements << msg;

    b1->elements << b2;

    QByteArray data = b1->package();

    QCOMPARE(data, QByteArray("#bundle" // bundle mark
                              "\x00\x00\x00\x00\x00\x00\x00\x01" // timestamp

                              "\x00\x00\x00\x10" // first element size
                              "/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A" // first element content

                              "\x00\x00\x00\x24" //second element size
                              "#bundle\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x10/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A\x00" // second element content

                              "\x00" // padding
                              , 76));

    auto packet = QOSCPacket::read( QByteArray("#bundle" // bundle mark
                                               "\x00\x00\x00\x00\x00\x00\x00\x01" // timestamp

                                               "\x00\x00\x00\x10" // first element size
                                               "/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A" // first element content

                                               "\x00\x00\x00\x24" //second element size
                                               "#bundle\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x10/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A\x00" // second element content

                                               "\x00" // padding
                                               , 76));

    QCOMPARE(packet->type, QOSCPacket::OSCBundle);

    auto b3 = packet.dynamicCast<QOSCBundle>();

    QCOMPARE(b3->time.toUint64(), 1ul);

    QCOMPARE(b3->elements.size(), 2);

    QCOMPARE(b3->elements[0]->type, QOSCPacket::OSCMessage);

    auto msg2 = b3->elements[0].dynamicCast<QOSCMessage>();

    QCOMPARE(msg2->pattern, "/a/b/c");
    QCOMPARE(msg2->args[0]->type, QOSC::Int32Type);
    QCOMPARE(msg2->args[0]->toInt(), 10);

    QCOMPARE(b3->elements[1]->type, QOSCPacket::OSCBundle);

    auto b4 = b3->elements[1].dynamicCast<QOSCBundle>();

    QCOMPARE(b4->time.toUint64(), 1ull);

    QCOMPARE(b4->elements.size(), 1);

    QCOMPARE(b4->elements[0]->type, QOSCPacket::OSCMessage);

    auto msg3 = b4->elements[0].dynamicCast<QOSCMessage>();

    QCOMPARE(msg3->pattern, "/a/b/c");
    QCOMPARE(msg3->args[0]->type, QOSC::Int32Type);
    QCOMPARE(msg3->args[0]->toInt(), 10);
}

QTEST_APPLESS_MAIN(packets)

#include "tst_packets.moc"
