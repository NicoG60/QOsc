#ifndef QOSCSTRING_H
#define QOSCSTRING_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include <QString>

class QOSC_EXPORT QOSCString : public QOSCAbstractType
{
protected:
    QOSCString(QOSC::Type t) : QOSCAbstractType(t) {}
    QOSCString(const QString& str, QOSC::Type t) : QOSCAbstractType(t), _str(str) {}
    QOSCString(QString&& str, QOSC::Type t) : QOSCAbstractType(t), _str(str) {}

public:
    QOSC_TYPE_CTOR(QOSCString, QOSC::StringType);

    QOSC_TYPE_DATA_CTOR(QOSCString, QOSC::StringType, String, const QString&)
    QOSC_TYPE_DATA_CTOR(QOSCString, QOSC::StringType, String, QString&&)

    QOSC_ACCESS_IMPL(String, QString, _str);
    QOSC_ACCESS_IMPL(Symbol, QString, _str);
    QOSC_ACCESS_IMPL_IO(Blob,      QByteArray, _str = QString::fromLatin1(v), _str.toLatin1());
    QOSC_ACCESS_IMPL_IO(ByteArray, QByteArray, _str = QString::fromLatin1(v), _str.toLatin1());

    QOSC_DERIVED_OPERATOR(QOSCString, QString);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('s'); }
    inline void writeData(QIODevice* dev) const override
    {
        QByteArray str = _str.toLatin1();
        do {
            str.append('\x0');
        } while(str.size() % 4 != 0);

        dev->write(str);
    }

    inline void readData(QIODevice* dev) override
    {
        _str.clear();
        char c = 0;

        while(dev->getChar(&c))
        {
            if(c == 0)
                break;

            _str.append(c);
        }

        int i = _str.size()+1;

        while(i % 4 != 0 && dev->peek(&c, 1) == 1 && c == 0)
        {
            dev->getChar(nullptr);
            i++;
        }
    }

private:
    QString _str = {};
};

#endif // QOSCSTRING_H
