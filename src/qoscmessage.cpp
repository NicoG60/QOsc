#include <qoscmessage.h>

#include "qoscmessage_p.h"
#include "qoscvalue_p.h"
#include "qoscbundle.h"

#include <QBuffer>

QOscMessagePrivate::QOscMessagePrivate(QOscMessage* q) :
    q_ptr(q)
{}

void QOscMessagePrivate::write(QIODevice* dev) const
{
    Q_Q(const QOscMessage);

    if(!q->isValid())
        return;

    QOscStringPrivate tmp;
    tmp.str = pattern;
    tmp.writeData(dev);

    QBuffer buf;
    buf.open(QIODevice::WriteOnly);
    buf.putChar(',');

    for(auto& arg : *q)
        arg.d_ptr->writeTypeTag(&buf);

    tmp.str = QString::fromLatin1(buf.data());
    tmp.writeData(dev);

    for(auto& arg : *q)
        arg.d_ptr->writeData(dev);
}

void QOscMessagePrivate::load(QIODevice* dev)
{
    Q_Q(QOscMessage);
    q->clear();

    QOscStringPrivate tmp;

    tmp.readData(dev);
    pattern = tmp.str;
    compilePattern();

    tmp.readData(dev);
    QString tmptags = tmp.str;

    // does not support arrays yet
    static QRegularExpression reg(R"(^,[ifsbhtdScrmTFNI]*$)");
    if(reg.match(tmptags).hasMatch())
    {
        for(auto& t : tmptags.mid(1))
        {
            switch (t.toLatin1())
            {
            case 'i':
                q->append(loadArg<QOscInt32Private>(dev));
                break;

            case 'f':
                q->append(loadArg<QOscFloat32Private>(dev));
                break;

            case 's':
                q->append(loadArg<QOscStringPrivate>(dev));
                break;

            case 'b':
                q->append(loadArg<QOscBlobPrivate>(dev));
                break;

            case 'h':
                q->append(loadArg<QOscInt64Private>(dev));
                break;

            case 't':
                q->append(loadArg<QOscTimeTagPrivate>(dev));
                break;

            case 'd':
                q->append(loadArg<QOscFloat64Private>(dev));
                break;

            case 'S':
                q->append(loadArg<QOscSymbolPrivate>(dev));
                break;

            case 'c':
                q->append(loadArg<QOscCharPrivate>(dev));
                break;

            case 'r':
                q->append(loadArg<QOscColorPrivate>(dev));
                break;

            case 'm':
                q->append(loadArg<QOscMidiPrivate>(dev));
                break;

            case 'T':
                q->append(loadArg<QOscTruePrivate>(dev));
                break;

            case 'F':
                q->append(loadArg<QOscFalsePrivate>(dev));
                break;

            case 'I':
                q->append(loadArg<QOscInfinitumPrivate>(dev));
                break;

            case 'N':
                q->append(loadArg<QOscNilPrivate>(dev));
                break;
            }
        }
    }
    else
    {
        QByteArray data = dev->readAll();
        q->append(QOscValue(data));
    }
}

void QOscMessagePrivate::compilePattern()
{
    QString copy = pattern;

    copy.replace('.', "\\.");
    copy.replace('(', "\\(");
    copy.replace(')', "\\)");
    copy.replace('*', "[^/]*");
    copy.replace("[!", "[^");
    copy.replace('?',  '.');

    QRegularExpression reg(R"({(([^,]+)(,([^,]+))+)})");
    QRegularExpressionMatch m;
    while((m = reg.match(copy)).hasMatch())
    {
        auto content = QRegularExpression::escape(m.captured(1));
        content.replace("\\,", "|");
        copy.replace(m.capturedStart(), m.capturedLength(), "(" + content + ")");
    }

    copy.prepend('^');
    copy.append('$');

    matcher.setPattern(copy);

    if(!matcher.isValid())
        pattern.clear();
}



// =============================================================================



QOscMessage::QOscMessage() :
    d_ptr(new QOscMessagePrivate(this))
{}

QOscMessage::QOscMessage(const QOscMessage& copy) :
    QOscMessage()
{
    *this = copy;
}

QOscMessage::QOscMessage(QOscMessage&& move) noexcept
{
    swap(move);
}

QOscMessage::QOscMessage(const QString& pattern) :
    QOscMessage()
{
    d_ptr->pattern = pattern;
    d_ptr->compilePattern();
}

QOscMessage::~QOscMessage()
{
    // Not using `= default` in the header because QScopedPointer doesn't allow it
}

QOscMessage& QOscMessage::operator=(const QOscMessage& copy)
{
    QList<QOscValue>::operator=(copy);
    *d_ptr = *copy.d_ptr;
    d_ptr->q_ptr = this;
    return *this;
}

QOscMessage& QOscMessage::operator=(QOscMessage&& move) noexcept
{
    swap(move);
    return *this;
}

void QOscMessage::swap(QOscMessage& other)
{
    QList<QOscValue>::swap(other);
    qSwap(d_ptr, other.d_ptr);

    if(d_ptr)
        d_ptr->q_ptr = this;

    if(other.d_ptr)
        other.d_ptr->q_ptr = &other;
}

bool QOscMessage::isValid() const
{
    Q_D(const QOscMessage);
    return !d->pattern.isEmpty() && d->matcher.isValid() && !isEmpty();
}

bool QOscMessage::match(const QString& addr) const
{
    return d_func()->matcher.match(addr).hasMatch();
}

QString QOscMessage::pattern() const
{
    return d_ptr->pattern;
}

QRegularExpression QOscMessage::matcher() const
{
    return d_ptr->matcher;
}

void QOscMessage::setPattern(const QString& p)
{
    Q_D(QOscMessage);
    if(p != d->pattern)
    {
        d->pattern = p;
        d->compilePattern();
    }
}

QOscMessage& QOscMessage::operator <<(const QOscValue& v)
{
    QList<QOscValue>::operator <<(v);
    return *this;
}

QOscMessage& QOscMessage::operator +=(const QOscValue& v)
{
    QList<QOscValue>::operator +=(v);
    return *this;
}

QOscMessage QOscMessage::operator +(const QOscValue& v) const
{
    QOscMessage other = *this;
    other += v;
    return other;
}

QOscBundle QOscMessage::operator +(const QOscMessage& v) const
{
    QOscBundle b;
    b << *this << v;
    return b;
}

QOscValue& QOscMessage::operator[](int i)
{
    return QList<QOscValue>::operator [](i);
}

const QOscValue& QOscMessage::operator[](int i) const
{
    return QList<QOscValue>::operator [](i);
}

QByteArray QOscMessage::package() const
{
    QBuffer b;
    b.open(QIODevice::WriteOnly);
    package(&b);
    return b.data();
}

void QOscMessage::package(QIODevice* dev) const
{
    d_ptr->write(dev);
}

QOscMessage QOscMessage::read(const QByteArray& data)
{
    QBuffer b(const_cast<QByteArray*>(&data));
    b.open(QIODevice::ReadOnly);
    return read(&b);
}

QOscMessage QOscMessage::read(QIODevice* dev)
{
    QOscMessage msg;
    msg.d_ptr->load(dev);
    return msg;
}
