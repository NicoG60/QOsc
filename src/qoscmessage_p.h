#ifndef QOSCMESSAGE_P_H
#define QOSCMESSAGE_P_H

#include <qoscvalue.h>

#include <QString>
#include <QRegularExpression>
#include <QIODevice>

class QOscMessage;
class QOscMessagePrivate
{
public:
    QOscMessagePrivate(QOscMessage* q);

    void write(QIODevice* dev) const;

    void load(QIODevice* dev);

    template<class T>
    QOscValue loadArg(QIODevice* dev)
    {
        auto a = new T;
        a->readData(dev);
        return QOscValue(a);
    }

    void compilePattern();

    QOscMessage* q_ptr;
    Q_DECLARE_PUBLIC(QOscMessage)

    QString pattern;
    QRegularExpression matcher;
};

#endif // QOSCMESSAGE_P_H
