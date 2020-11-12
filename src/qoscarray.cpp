#include <types/qoscarray.h>

void QOSCArray::writeTypeTag(QIODevice* dev) const
{
    dev->putChar('[');

    for(auto& t : _array)
        t->writeTypeTag(dev);

    dev->putChar(']');
}

void QOSCArray::writeData(QIODevice* dev) const
{
    Q_UNUSED(dev);
}
