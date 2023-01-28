#include <QtTest>
#include <QDebug>

#include <QOscValue>

class types : public QObject
{
    Q_OBJECT

public:
    types() = default;
    ~types() override = default;

    template<class F, class T>
    void test_ctor(QOscValue* obj, F fp, T value1, T value2)
    {
        QCOMPARE((obj->*fp)(), value1);
        *obj = value2;
        QCOMPARE((obj->*fp)(), value2);
    }

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

void types::test_int32()
{
    QOscValue test(5);
    QCOMPARE(test.type(), QOsc::Int32Type);
    QCOMPARE(test.toInt32(), 5);

    test = 3;
    QCOMPARE(test.toInt(), 3);
}

void types::test_int64()
{
    QOscValue test(5ll);
    QCOMPARE(test.type(), QOsc::Int64Type);
    QCOMPARE(test.toInt64(), 5ll);

    test = 3ll;
    QCOMPARE(test.toLong(), 3ll);
}

void types::test_float32()
{
    QOscValue test(5.5f);
    QCOMPARE(test.type(), QOsc::Float32Type);
    QCOMPARE(test.toFloat32(), 5.5f);

    test = 1.0f;
    QCOMPARE(test.toFloat(), 1.0f);
}

void types::test_float64()
{
    QOscValue test(5.5);
    QCOMPARE(test.type(), QOsc::Float64Type);
    QCOMPARE(test.toFloat64(), 5.5);

    test = 1.0;
    QCOMPARE(test.toDouble(), 1.0);
}

void types::test_string()
{
    QOscValue test(QString("test"));
    QCOMPARE(test.type(), QOsc::StringType);
    QCOMPARE(test.toString(), "test");

    test = QString("test2");
    QCOMPARE(test.toString(), "test2");
}

void types::test_symbol()
{
    QOscValue test = QOscValue::symbolValue(QString("test"));
    QCOMPARE(test.type(), QOsc::SymbolType);
    QCOMPARE(test.toSymbol(), "test");

    test = QString("test2");
    QCOMPARE(test.toString(), "test2");
}

void types::test_blob()
{
    QOscValue test(QByteArray("test"));
    QCOMPARE(test.type(), QOsc::BlobType);
    QCOMPARE(test.toString(), "test");

    test = QByteArray("test2");
    QCOMPARE(test.toString(), "test2");
}

void types::test_timetag()
{
    QOscValue test(QDateTime(QDate(1900, 1, 1), QTime(0, 0)));
    QCOMPARE(test.type(), QOsc::TimeTagType);
    QCOMPARE(test.toDateTime(), QDateTime(QDate(1900, 1, 1), QTime(0, 0)));

    QVERIFY(QOscValue::asap().isNow());

    QCOMPARE(QOscValue::fromUnixTimestamp(0).toDateTime(), QDateTime(QDate(1970, 1, 1), QTime(0, 0), Qt::UTC));
}

void types::test_char()
{
    QOscValue test('A');
    QCOMPARE(test.type(), QOsc::CharType);
    QCOMPARE(test.toChar(), '\x41');

    test = ' ';
    QCOMPARE(test.toChar(), '\x20');
}

void types::test_color()
{
    QOscValue test(QColor(255, 0, 0));
    QCOMPARE(test.type(), QOsc::ColorType);
    QCOMPARE(test.toColor(), QColor(Qt::red));

    test = QColor(Qt::green);
    QCOMPARE(test.toColor(), QColor(0, 255, 0));
}

void types::test_midi()
{
    QOscValue test = QOscValue::midiValue(0, 1, 2, 3);
    QCOMPARE(test.type(), QOsc::MidiType);
    QCOMPARE(test.port(), 0);
    QCOMPARE(test.status(), 1);
    QCOMPARE(test.data1(), 2);
    QCOMPARE(test.data2(), 3);
}

void types::test_litteral()
{
    QOscValue test(true);
    QCOMPARE(test.type(), QOsc::TrueType);
    QVERIFY(test.isBool());
    QVERIFY(test.isTrue());
    QVERIFY(test.toBool());

    test = QOscValue();
    QCOMPARE(test.type(), QOsc::NilType);
    QVERIFY(!test.isBool());
    QVERIFY(test.isNil());
    QVERIFY(!test.toBool());
}


QTEST_APPLESS_MAIN(types)

#include "tst_types.moc"
