#include <qoscbundle.h>
#include <QBuffer>

#include "qoscbundle_p.h"
#include "qoscvalue_p.h"

QOscBundlePrivate::QOscBundlePrivate(QOscBundle* q) :
    q_ptr(q),
    time(QOscValue::asap())
{}

void QOscBundlePrivate::write(QIODevice* dev) const
{
    Q_Q(const QOscBundle);
    Q_ASSERT(q->isValid());

    dev->write("#bundle\0", 8);

    time.d_ptr->writeData(dev);

    qint64 size = 8+8;

    for(auto& e : *q)
    {
        QOscBlobPrivate blob;
        blob.data = e.package();

        size += 4 + blob.data.size();

        blob.writeData(dev);
    }

    while(size % 4 != 0)
    {
        dev->putChar('\0');
        size++;
    }
}

void QOscBundlePrivate::load(QIODevice* dev)
{
    Q_Q(QOscBundle);

    {
        QByteArray header = dev->read(8);
        header.resize(7);
        if(header != "#bundle")
            return;
    }

    q->clear();

    time.d_ptr->readData(dev);

    // 5 is the minimum for a 4 byte size + 1 byte data
    // anything below that is padding
    while(dev->bytesAvailable() >= 5)
    {
        QOscBlobPrivate blob;
        blob.readData(dev);

        switch(QOsc::detectType(blob.data))
        {
        case QOsc::OscMessage:
            *q << QOscMessage::read(blob.data);
            break;

        case QOsc::OscBundle:
            *q << QOscBundle::read(blob.data);
            break;

        default:
            break;
        }
    }
}



// =============================================================================



QOscBundle::QOscBundle() :
    d_ptr(new QOscBundlePrivate(this))
{}

QOscBundle::QOscBundle(const QOscBundle& copy) :
    QOscBundle()
{
    *this = copy;
}

QOscBundle::QOscBundle(QOscBundle&& move) noexcept
{
    swap(move);
}

QOscBundle::~QOscBundle()
{
    // Not using `= default` in the header because QScopedPointer doesn't allow it
}

QOscBundle& QOscBundle::operator=(const QOscBundle& copy)
{
    QList<QOscMessage>::operator=(copy);
    *d_ptr = *copy.d_ptr;
    d_ptr->q_ptr = this;
    return *this;
}

QOscBundle& QOscBundle::operator=(QOscBundle&& move) noexcept
{
    swap(move);
    return *this;
}

void QOscBundle::swap(QOscBundle& other)
{
    QList<QOscMessage>::swap(other);
    qSwap(d_ptr, other.d_ptr);

    if(d_ptr)
        d_ptr->q_ptr = this;

    if(other.d_ptr)
        other.d_ptr->q_ptr = &other;
}

bool QOscBundle::isValid() const
{
    if(!d_ptr->time.isTimeTag())
        return false;

    if(isEmpty())
        return false;

    for(auto& e : *this)
    {
        if(!e.isValid())
            return false;
    }

    return true;
}

QOscValue QOscBundle::time() const
{
    return d_ptr->time;
}

void QOscBundle::setTime(const QOscValue& time)
{
    if(time.isTimeTag())
        d_ptr->time = time;
}

QOscBundle& QOscBundle::operator <<(const QOscMessage& msg)
{
    QList<QOscMessage>::operator<<(msg);
    return *this;
}

QOscBundle& QOscBundle::operator <<(const QOscBundle& other)
{
    QList<QOscMessage>::operator<<(other);
    return *this;
}

QOscBundle& QOscBundle::operator +=(const QOscMessage& other)
{
    QList<QOscMessage>::operator +=(other);
    return *this;
}

QOscBundle& QOscBundle::operator +=(const QOscBundle& other)
{
    QList<QOscMessage>::operator +=(other);
    return *this;
}

QOscBundle QOscBundle::operator +(const QOscBundle& other) const
{
    QOscBundle b = *this;
    b << other;
    return b;
}

QOscMessage& QOscBundle::operator[](int i)
{
    return QList<QOscMessage>::operator [](i);
}

const QOscMessage& QOscBundle::operator[](int i) const
{
    return QList<QOscMessage>::operator [](i);
}

QByteArray QOscBundle::package() const
{
    QBuffer b;
    b.open(QIODevice::WriteOnly);
    package(&b);
    return b.data();
}

void QOscBundle::package(QIODevice* dev) const
{
    d_ptr->write(dev);
}

QOscBundle QOscBundle::read(const QByteArray& data)
{
    QBuffer b(const_cast<QByteArray*>(&data));
    b.open(QIODevice::ReadOnly);
    return read(&b);
}

QOscBundle QOscBundle::read(QIODevice* dev)
{
    QOscBundle b;
    b.d_ptr->load(dev);
    return b;
}
