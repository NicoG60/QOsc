#include "qoscstring.h"

void QOSCString::writeData(QIODevice* dev) const
{
    dev->write(_str.toLatin1());
}
