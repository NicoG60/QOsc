#include <qoscbundle.h>
#include <QBuffer>
#include <types/qoscblob.h>

bool QOSCBundle::isValid() const
{
    if(QDateTime(time) < QDateTime::currentDateTime() && quint64(time) != 1)
        return false;

    if(elements.isEmpty())
        return false;

    return true;
}

void QOSCBundle::write(QIODevice* dev) const
{
    dev->write("#bundle");

    time.writeData(dev);

    for(auto& e : elements)
    {
        QBuffer buf;
        buf.open(QIODevice::WriteOnly);

        e->write(&buf);

        QOSCBlob blob(buf.buffer());
        blob.writeData(dev);
    }
}

void QOSCBundle::load(QIODevice* dev)
{
    if(dev->read(7) != "#bundle")
        return;

    time.readData(dev);

    while(!dev->atEnd())
    {
        QOSCBlob blob;
        blob.readData(dev);
        elements << QOSCPacket::read(blob);
    }
}
