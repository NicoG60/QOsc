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
    QCOMPARE(qint32(*i), 10);

    QByteArray data = msg.package();

    QCOMPARE(data, QByteArray("/a/b/c\x00\x00,i\x00\x00\x00\x00\x00\x0A", 16));
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

QTEST_APPLESS_MAIN(packets)

#include "tst_packets.moc"
