#ifndef QOSCBLOB_H
#define QOSCBLOB_H

#include "QOSC_global.h"
#include "qoscvalue.h"

class QOSC_EXPORT QOSCBlob : public QOSCValue
{
public:
    QOSC_TYPE_CTOR(QOSCBlob, QOSC::BlobType);

    QOSC_TYPE_DATA_CTOR(QOSCBlob, QOSC::BlobType, Blob, const QByteArray&)
    QOSC_TYPE_DATA_CTOR(QOSCBlob, QOSC::BlobType, Blob, QByteArray&&)

    QOSC_ACCESS_IMPL(Blob,      QByteArray, _data);
    QOSC_ACCESS_IMPL(ByteArray, QByteArray, _data);
    QOSC_ACCESS_IMPL_IO(String, QString, _data = v.toLatin1(), QString::fromLatin1(_data));
    QOSC_ACCESS_IMPL_IO(Symbol, QString, _data = v.toLatin1(), QString::fromLatin1(_data));

    QOSC_DERIVED_OPERATOR(QOSCBlob, QByteArray);

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('b'); }
    inline void writeData(QIODevice* dev)    const override
    {
        QByteArray d = _data;

        while(d.size() % 4 != 0)
            d.append('\x0');

        QOSC::writeHelper(dev, static_cast<qint32>(d.size()));
        dev->write(d);
    }

    inline void readData(QIODevice* dev) override
    {
        qint32 size;
        QOSC::readHelper(dev, &size);
        _data = dev->read(size);
    }

private:
    QByteArray _data;
};

#endif // QOSCBLOB_H
