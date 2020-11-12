#ifndef QOSCBLOB_H
#define QOSCBLOB_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"

class QOSC_EXPORT QOSCBlob : public QOSCAbstractType
{
public:
    QOSCBlob() : QOSCAbstractType(QOSC::BlobType) {}
    QOSCBlob(const QOSCBlob& copy) = default;
    QOSCBlob(QOSCBlob&& move) = default;

    inline QOSCBlob(const QByteArray& data) : QOSCAbstractType(QOSC::BlobType), _data(data) {}
    inline QOSCBlob(QByteArray&& data) : QOSCAbstractType(QOSC::BlobType), _data(data) {}

    inline operator QByteArray() const { return _data; }

    QOSCBlob& operator=(const QOSCBlob& i) = default;
    QOSCBlob& operator=(QOSCBlob&& i) = default;

    inline QOSCBlob& operator=(const QByteArray& data) { _data = data; return *this; };
    inline QOSCBlob& operator=(QByteArray&& data) { _data = data; return *this; };

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('b'); }
    inline void writeData(QIODevice* dev) const override
    {
        QByteArray d = _data;

        while(d.size() % 4 != 0)
            d.append('\x0');

        qint32 size = d.size();

        dev->write((const char*)&size, sizeof (qint32));
        dev->write(d);
    }

    inline void readData(QIODevice* dev) override
    {
        qint32 size;
        dev->read((char*)&size, sizeof (qint32));
        _data = dev->read(size);
    }

private:
    QByteArray _data;
};

#endif // QOSCBLOB_H
