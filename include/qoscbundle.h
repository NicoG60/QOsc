#ifndef QOSCBUNDLE_H
#define QOSCBUNDLE_H

#include "qoscmessage.h"
#include <memory>

class QOscBundlePrivate;
class QOSC_EXPORT QOscBundle : public QList<QOscMessage>
{
public:
    QOscBundle();
    QOscBundle(const QOscBundle& copy);
    QOscBundle(QOscBundle&& move) noexcept;
    ~QOscBundle();

    QOscBundle& operator=(const QOscBundle&copy);
    QOscBundle& operator=(QOscBundle&& move) noexcept;

    void swap(QOscBundle& other);

    bool isValid() const;

    QOscValue time() const;
    void setTime(const QOscValue& time);

    QOscBundle& operator <<(const QOscMessage& msg);
    QOscBundle& operator <<(const QOscBundle& other);

    QOscBundle& operator +=(const QOscMessage& other);
    QOscBundle& operator +=(const QOscBundle& other);
    QOscBundle  operator +(const QOscBundle& other) const;

    QOscMessage& operator[](int i);
    const QOscMessage& operator[](int i) const;

    QByteArray package() const;
    void package(QIODevice* dev) const;

    static QOscBundle read(const QByteArray& data);
    static QOscBundle read(QIODevice* dev);

private:
    std::unique_ptr<QOscBundlePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QOscBundle)
};

Q_DECLARE_METATYPE(QOscBundle);

#endif // QOSCBUNDLE_H
