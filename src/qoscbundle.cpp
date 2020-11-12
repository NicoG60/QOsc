#include <qoscbundle.h>
#include <QBuffer>
#include <types/qoscblob.h>

bool QOSCBundle::isValid() const
{
    if(time.toDateTime() < QDateTime::currentDateTime() && quint64(time) != 1)
        return false;

    if(elements.isEmpty())
        return false;

    return true;
}

void QOSCBundle::write(QIODevice* dev) const
{
    dev->write("#bundle");

    time.writeData(dev);

    qint64 size = 7+8;

    for(auto& e : elements)
    {
        size += 4;
        QByteArray data = e->package();

        QOSC::writeHelper(dev, static_cast<qint32>(data.size()));
        size += dev->write(data);
    }

    while(size % 4 != 0)
    {
        dev->putChar('\0');
        size++;
    }
}

void QOSCBundle::load(QIODevice* dev)
{
    if(dev->read(7) != "#bundle")
        return;

    time.readData(dev);

    while(dev->bytesAvailable() > 0)
    {
        QOSCBlob blob;
        blob.readData(dev);
        elements << QOSCPacket::read(blob.toByteArray());
    }
}
