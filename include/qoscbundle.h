#ifndef QOSCBUNDLE_H
#define QOSCBUNDLE_H

#include "QOSC_global.h"
#include "qoscpacket.h"
#include "types/qosctimetag.h"

class QOSC_EXPORT QOSCBundle : public QOSCPacket
{
public:
    typedef QSharedPointer<QOSCBundle> ptr;

    QOSCBundle() : QOSCPacket(OSCBundle) {}
    QOSCBundle(const QOSCBundle&) = default;
    QOSCBundle(QOSCBundle&&) = default;

    QOSCBundle& operator=(const QOSCBundle&) = default;
    QOSCBundle& operator=(QOSCBundle&&) = default;

    bool isValid() const override;

    void write(QIODevice* dev) const override;

protected:
    void load(QIODevice* dev) override;

public:
    QOSCTimeTag time;
    QOSCPacket::list elements;
};

#endif // QOSCBUNDLE_H
