#include <QtTest>

#include <types/qosctypes.h>

class types : public QObject
{
    Q_OBJECT

public:
    types();
    ~types();

private slots:
    void test_int32();
    void test_int64();

    void test_float32();
    void test_float64();

    void test_string();
    void test_symbol();
    void test_blob();

    void test_timetag();

    void test_char();

    void test_color();
    void test_midi();
    void test_litteral();
};

types::types() {}

types::~types() {}

void types::test_int32()
{
    QOSCInt32 i32(5);

    QCOMPARE(i32, 5);

    i32 = 3;

    QCOMPARE(i32, 3);

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    i32.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'i');

    data.clear();
    b.seek(0);
    i32.writeData(&b);

    QCOMPARE(data.size(), 4);
}

void types::test_int64()
{
    QOSCInt64 i64(5);

    QCOMPARE(i64, 5);

    i64 = 3;

    QCOMPARE(i64, 3);

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    i64.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'h');

    data.clear();
    b.seek(0);
    i64.writeData(&b);

    QCOMPARE(data.size(), 8);
}

void types::test_float32()
{
    QOSCFloat32 f32(5.5f);

    QCOMPARE(f32, 5.5f);

    f32 = 3.1f;

    QCOMPARE(f32, 3.1f);

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    f32.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'f');

    data.clear();
    b.seek(0);
    f32.writeData(&b);

    QCOMPARE(data.size(), 4);
}

void types::test_float64()
{
    QOSCFloat64 f64(5.5);

    QCOMPARE(f64, 5.5);

    f64 = 3.1;

    QCOMPARE(f64, 3.1);

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    f64.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'd');

    data.clear();
    b.seek(0);
    f64.writeData(&b);

    QCOMPARE(data.size(), 8);
}


void types::test_string()
{
    QOSCString str("test1");

    QCOMPARE(str, QString("test1"));

    str = "test2";

    QCOMPARE(str, QString("test2"));

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    str.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 's');

    data.clear();
    b.seek(0);
    str.writeData(&b);

    QCOMPARE(data.size(), 8);
}

void types::test_symbol()
{
    QOSCSymbol str("test1");

    QCOMPARE(str, QString("test1"));

    str = "test2";

    QCOMPARE(str, QString("test2"));

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    str.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'S');

    data.clear();
    b.seek(0);
    str.writeData(&b);

    QCOMPARE(data.size(), 8);
}

void types::test_blob()
{
    QOSCBlob blob("test1");

    QCOMPARE(blob, QByteArray("test1"));

    blob = "test2";

    QCOMPARE(blob, QByteArray("test2"));

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    blob.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'b');

    data.clear();
    b.seek(0);
    blob.writeData(&b);

    QCOMPARE(data.size(), 12);
}

void types::test_timetag()
{
    QOSCTimeTag time(QDateTime(QDate(1900, 1, 1), QTime(0, 0)));

    quint64 t = time;
    QCOMPARE(t, quint64(0));

    time = QDateTime(QDate(1970, 1, 1), QTime(10, 0, 0, 10));

    QCOMPARE(QDateTime(time), QDateTime(QDate(1970, 1, 1), QTime(10, 0, 0, 10)));

    QVERIFY(QOSCTimeTag::asap().isNow());
}

void types::test_char()
{
    QOSCChar c('A');

    QCOMPARE(c, 65);

    c = '0';

    QCOMPARE(c, '0');

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    c.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'c');

    data.clear();
    b.seek(0);
    c.writeData(&b);

    QCOMPARE(data.size(), 4);
}

void types::test_color()
{
    QOSCColor blob("#FF0000");

    QCOMPARE(QColor(blob), QColor(Qt::red));

    blob = QColor(0, 255, 0, 0);

    QCOMPARE(QColor(blob), QColor("#0000FF00"));

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    blob.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'r');

    data.clear();
    b.seek(0);
    blob.writeData(&b);

    QCOMPARE(data.size(), 4);
    QCOMPARE(data.at(0), 0);
    QCOMPARE(data.at(1), '\xff');
    QCOMPARE(data.at(2), 0);
    QCOMPARE(data.at(3), 0);
}

void types::test_midi()
{
    QOSCMidi blob(0, 1, 2, 3);

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    blob.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'm');

    data.clear();
    b.seek(0);
    blob.writeData(&b);

    QCOMPARE(data.size(), 4);
    QCOMPARE(data.at(0), 0);
    QCOMPARE(data.at(1), 1);
    QCOMPARE(data.at(2), 2);
    QCOMPARE(data.at(3), 3);
}

void types::test_litteral()
{
    QOSCTrue t;

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    t.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'T');

    data.clear();
    b.seek(0);
    t.writeData(&b);

    QVERIFY(data.isEmpty());

    QVERIFY(t);
}


QTEST_APPLESS_MAIN(types)

#include "tst_types.moc"
