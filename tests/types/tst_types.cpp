#include <QtTest>

#include <types/qosctypes.h>
#include <QDebug>

class QByteArrayTest : public QByteArray
{
public:
    QByteArrayTest() : QByteArray() { qDebug() << "Default constructor of " << this; qDebug() << *this; }
    QByteArrayTest(QByteArrayTest&& m) : QByteArray(m) { qDebug() << "Move constructor of " << this; qDebug() << *this; }
    QByteArrayTest(const QByteArrayTest& c) : QByteArray(c) { qDebug() << "Copy constructor of " << this; qDebug() << *this; }
    QByteArrayTest(int size, char c) : QByteArray(size, c) { qDebug() << "Fill constructor of " << this; qDebug() << *this; }
    QByteArrayTest(const char* c, int size = -1) : QByteArray(c, size) { qDebug() << "Data constructor of " << this; qDebug() << *this; }
    ~QByteArrayTest() { qDebug() << "Destructor constructor of " << this; }
};

class types : public QObject
{
    Q_OBJECT

public:
    types();
    ~types();

    template<class F, class T>
    void test_ctor(QOSCValue* obj, F fp, T value1, T value2)
    {
        QCOMPARE((obj->*fp)(), value1);
        *obj = value2;
        QCOMPARE((obj->*fp)(), value2);
    }

    void test_tag(QOSCValue* obj, char t);

    template<class T>
    void test_write(QOSCValue* obj, T value, const QByteArray& comp)
    {
        *obj = value;

        QBuffer b;
        b.open(QIODevice::WriteOnly);

        obj->writeData(&b);

        b.close();

        QCOMPARE(b.buffer(), comp);
    }

    template<class F, class T>
    void test_read(QOSCValue* obj, const QByteArray& data, F fp, T value)
    {
        QBuffer b;
        b.setData(data);
        b.open(QIODevice::ReadOnly);

        obj->readData(&b);

        b.close();

        QCOMPARE((obj->*fp)(), value);
        QVERIFY(b.atEnd());
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

types::types() {}

types::~types() {}

void types::test_tag(QOSCValue* obj, char t)
{
    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::WriteOnly);

    obj->writeTypeTag(&b);

    b.close();

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], t);
}

void types::test_int32()
{
    QOSCInt32 i32(5);

    test_ctor(&i32, &QOSCValue::toInt32, 5, 3);

    test_tag(&i32, 'i');
    test_write(&i32, 3, QByteArray("\0\0\0\3", 4));
    test_read(&i32, QByteArray("\0\0\0\xFF", 4), &QOSCValue::toInt32, 255);
}

void types::test_int64()
{
    QOSCInt64 i64(5ll);

    test_ctor(&i64, &QOSCValue::toInt64, 5ll, 3ll);

    test_tag(&i64, 'h');
    test_write(&i64, 3ll, QByteArray("\0\0\0\0\0\0\0\x03", 8));
    test_read(&i64, QByteArray("\0\0\0\0\0\0\0\xFF", 8), &QOSCValue::toInt64, 255ll);
}

void types::test_float32()
{
    QOSCFloat32 f32(5.5f);

    test_ctor(&f32, &QOSCValue::toFloat32, 5.5f, 3.0f);

    test_tag(&f32, 'f');
    test_write(&f32, 1.0f, QByteArray("\x3F\x80\0\0", 4));
    test_read(&f32, QByteArray("\x3F\x80\0\0", 4), &QOSCValue::toFloat32, 1.0f);
}

void types::test_float64()
{
    QOSCFloat64 f64(5.5);

    test_ctor(&f64, &QOSCValue::toFloat32, 5.5, 3.0);

    test_tag(&f64, 'd');
    test_write(&f64, 1.0, QByteArray("\x3F\xF0\0\0\0\0\0\0", 8));
    test_read(&f64, QByteArray("\x3F\xF0\0\0\0\0\0\0", 8), &QOSCValue::toFloat32, 1.0);
}

void types::test_string()
{
    QOSCString str("test1");

    test_ctor(&str, &QOSCValue::toString, QString("test1"), QString("test2"));

    test_tag(&str, 's');
    test_write(&str, QString("osc"), QByteArray("osc\0", 4));
    test_write(&str, QString("data"), QByteArray("data\0\0\0\0", 8));
    test_read(&str, QByteArray("osc\0", 4), &QOSCValue::toString, "osc");
    test_read(&str, QByteArray("data\0\0\0\0", 8), &QOSCValue::toString, "data");
}

void types::test_symbol()
{
    QOSCSymbol str("test1");

    test_ctor(&str, &QOSCValue::toString, QString("test1"), QString("test2"));

    test_tag(&str, 'S');
    test_write(&str, QString("osc"), QByteArray("osc\0", 4));
    test_write(&str, QString("data"), QByteArray("data\0\0\0\0", 8));
    test_read(&str, QByteArray("osc\0", 4), &QOSCValue::toString, "osc");
    test_read(&str, QByteArray("data\0\0\0\0", 8), &QOSCValue::toString, "data");
}

void types::test_blob()
{
    QOSCBlob blob("test1");

    test_ctor(&blob, &QOSCValue::toByteArray, QByteArray("test1"), QByteArray("test2"));

    test_tag(&blob, 'b');
    test_write(&blob, QByteArray("osc"), QByteArray("\0\0\0\x04osc\0", 8));
    test_write(&blob, QByteArray("data"), QByteArray("\0\0\0\x04""data", 8));
    test_write(&blob, QByteArray("data1"), QByteArray("\0\0\0\x08""data1\0\0\0", 12));
    test_read(&blob, QByteArray("\0\0\0\x04osc\0", 8), &QOSCValue::toString, QByteArray("osc\0", 4));
    test_read(&blob, QByteArray("\0\0\0\x04""data", 8), &QOSCValue::toString, QByteArray("data", 4));
    test_read(&blob, QByteArray("\0\0\0\x08""data1\0\0\0", 12), &QOSCValue::toString, QByteArray("data1\0\0\0", 8));
}

void types::test_timetag()
{
    QOSCTimeTag time(QDateTime(QDate(1900, 1, 1), QTime(0, 0)));

    QCOMPARE(time.toUint64(), 0ul);

    test_ctor(&time, &QOSCValue::toDateTime, QDateTime(QDate(1900, 1, 1), QTime(0, 0)), QDateTime(QDate(1970, 1, 1), QTime(10, 0, 0, 10)));

    test_tag(&time, 't');

    QVERIFY(QOSCTimeTag::asap().isNow());

    test_read(&time, QByteArray("\0\0\0\x04\0\0\0\0", 8), &QOSCValue::toDateTime, QDateTime(QDate(1900, 1, 1), QTime(0, 0, 4)));

    QCOMPARE(time.toUint64(), 4ul << 32);
}

void types::test_char()
{
    QOSCChar c('A');

    test_ctor(&c, &QOSCValue::toChar, '\x41', '0');

    test_tag(&c, 'c');
    test_write(&c, ' ', QByteArray("\0\0\0\x20", 4));
    test_read(&c, QByteArray("\0\0\0\x20", 4), &QOSCValue::toChar, ' ');
}

void types::test_color()
{
    QOSCColor color(QColor(255, 0, 0));

    test_ctor(&color, &QOSCValue::toColor, QColor(Qt::red), QColor("#0000FF00"));

    test_tag(&color, 'r');
    test_write(&color, QColor(Qt::green), QByteArray("\0\xFF\0\xFF", 4));
    test_read(&color, QByteArray("\0\0\xFF\xFF", 4), &QOSCValue::toColor, QColor(Qt::blue));
}

void types::test_midi()
{
    QOSCMidi midi(0, 1, 2, 3);

    QByteArray data;
    QBuffer b(&data);
    b.open(QIODevice::ReadWrite);

    midi.writeTypeTag(&b);

    QCOMPARE(data.size(), 1);
    QCOMPARE(data[0], 'm');

    data.clear();
    b.seek(0);
    midi.writeData(&b);

    QCOMPARE(data.size(), 4);
    QCOMPARE(data.at(0), 0);
    QCOMPARE(data.at(1), 1);
    QCOMPARE(data.at(2), 2);
    QCOMPARE(data.at(3), 3);

    QBuffer b2;
    b2.setData("\0\0\xFF\0", 4);
    b2.open(QIODevice::ReadOnly);

    midi.readData(&b2);

    QCOMPARE(midi.port, 0);
    QCOMPARE(midi.status, 0);
    QCOMPARE(midi.data1, -1);
    QCOMPARE(midi.data2, 0);
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

    QVERIFY(t.toBool());
}


QTEST_APPLESS_MAIN(types)

#include "tst_types.moc"
