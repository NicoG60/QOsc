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
    QOSCString() : QOSCAbstractType(QOSC::StringType) {}
    QOSCString(const QOSCString& copy) = default;
    QOSCString(QOSCString&& move) = default;

    inline QOSCString(const QString& str) : QOSCAbstractType(QOSC::StringType), _str(str) {}
    inline QOSCString(QString&& str) : QOSCAbstractType(QOSC::StringType), _str(str) {}

    inline operator QString() const { return _str; }

    QOSCString& operator=(const QOSCString& i) = default;
    QOSCString& operator=(QOSCString&& i) = default;

    inline QOSCString& operator=(const QString& str) { _str = str; return *this; };
    inline QOSCString& operator=(QString&& str) { _str = str; return *this; };

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

        while(dev->peek(&c, 1) == 1 && c == 0)
            dev->getChar(nullptr);
    }

private:
    QString _str = {};
};

#endif // QOSCSTRING_H
