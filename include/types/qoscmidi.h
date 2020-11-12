#ifndef QOSCMIDI_H
#define QOSCMIDI_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"

class QOSC_EXPORT QOSCMidi : public QOSCAbstractType
{
public:
    QOSC_TYPE_CTOR(QOSCMidi, QOSC::MidiType);

    constexpr inline QOSCMidi(qint8 p, qint8 s, qint8 d1, qint8 d2) :
        QOSCAbstractType(QOSC::MidiType),
        port(p),
        status(s),
        data1(d1),
        data2(d2)
    {}

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('m'); }
    inline void writeData(QIODevice* dev) const override
    {
        dev->putChar(port);
        dev->putChar(status);
        dev->putChar(data1);
        dev->putChar(data2);
    }

    inline void readData(QIODevice* dev) override
    {
        dev->getChar((char*)&port);
        dev->getChar((char*)&status);
        dev->getChar((char*)&data1);
        dev->getChar((char*)&data2);
    }

    qint8 port = {};
    qint8 status = {};
    qint8 data1 = {};
    qint8 data2 = {};
};

#endif // QOSCMIDI_H
