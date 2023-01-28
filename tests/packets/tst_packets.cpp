#include <QtTest>

#include <QOscMessage>
#include <QOscBundle>
#include <QOscValue>

#include <QDebug>

class packets : public QObject
{
    Q_OBJECT

public:
    packets() = default;
    ~packets() override = default;

private slots:
    void test_message();

    void test_match_data();
    void test_match();

    void test_bundle();

};

void packets::test_message()
{
    QOscMessage msg("/a/b/c", 10);

    QVERIFY(msg.isValid());

    QVERIFY(msg.size() == 1);

    auto& i = msg[0];
    QCOMPARE(i.toInt(), 10);

    QByteArray data = msg.package();

    QCOMPARE(data, QByteArray("/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A", 16));

    auto packet = QOscMessage::read(QByteArray("/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A", 16));

    QCOMPARE(packet.pattern(), "/a/b/c");
    QCOMPARE(packet.type(), QOsc::Int32Type);
    QCOMPARE(packet.toInt(), 10);
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

    QOscMessage msg(pattern, 10);

    QCOMPARE(msg.matcher().pattern(), matcher);

    for(auto& addr : positive)
        QVERIFY(msg.match(addr));

    for(auto& addr : negative)
        QVERIFY(!msg.match(addr));
}

void packets::test_bundle()
{
    QOscMessage msg("/a/b/c", 10);
    QOscBundle b1;
    QOscBundle b2;

    b1.setTime(QOscValue::asap());
    b1 << msg;

    b2.setTime(QOscValue::asap());
    b2 << msg;

    b1 << b2;

    QByteArray data = b1.package();

    QCOMPARE(data, QByteArray("#bundle\x00" // bundle mark
                              "\x00\x00\x00\x00\x00\x00\x00\x01" // timestamp

                              "\x00\x00\x00\x10" // first element size
                              "/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A" // first element content

                              "\x00\x00\x00\x10" // second element size
                              "/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A" // second element content

                              "\x00" // padding
                              , 56));

    auto bundle = QOscBundle::read( QByteArray("#bundle\x00" // bundle mark
                                               "\x00\x00\x00\x00\x00\x00\x00\x01" // timestamp

                                               "\x00\x00\x00\x10" // first element size
                                               "/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A" // first element content

                                               "\x00\x00\x00\x24" //second element size
                                               "#bundle\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x10/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A\x00" // second element content

                                               "\x00" // padding
                                               , 76));

    QVERIFY(bundle.time().isNow());

    QCOMPARE(bundle.size(), 2);

    for(auto& e : bundle)
    {
        QCOMPARE(e.pattern(), "/a/b/c");
        QCOMPARE(e.type(), QOsc::Int32Type);
        QCOMPARE(e.toInt(), 10);
    }
}

QTEST_APPLESS_MAIN(packets)

#include "tst_packets.moc"
