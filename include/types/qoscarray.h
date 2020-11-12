#ifndef QOSCARRAY_H
#define QOSCARRAY_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"

class QOSC_EXPORT QOSCArray : public QOSCAbstractType
{
public:
    typedef QList<QOSCAbstractType::ptr> list;

public:
    QOSCArray() : QOSCAbstractType(QOSC::ArrayType) {}
    QOSCArray(const QOSCArray& copy) = default;
    QOSCArray(QOSCArray&& move) = default;

    QOSCArray& operator=(const QOSCArray& i) = default;
    QOSCArray& operator=(QOSCArray&& i) = default;

    inline int size() const { return _array.size(); }

    inline list::iterator begin() { return _array.begin(); }
    inline list::const_iterator begin() const { return _array.begin(); }
    inline list::const_iterator cbegin() { return _array.cbegin(); }

    inline list::iterator end() { return _array.end(); }
    inline list::const_iterator end() const { return _array.end(); }
    inline list::const_iterator cend() { return _array.cend(); }

    template<class... Args>
    inline void append(Args... t) { append(QOSC::make(t...)); }

    template<>
    inline void append(const QOSCAbstractType::ptr& t) { _array.append(t); }

    template<class... Args>
    inline void prepend(Args... t) { prepend(QOSC::make(t...)); }

    template<>
    inline void prepend(const QOSCAbstractType::ptr& t) { _array.prepend(t); }

    template<class T>
    inline QOSCArray& operator <<(const T& t) { append(t); return *this; }

    inline QOSCAbstractType::ptr& operator[](int i) { return _array[i]; }
    inline const QOSCAbstractType::ptr& operator[](int i) const { return _array[i]; }

    void writeTypeTag(QIODevice* dev) const override;
    void writeData(QIODevice* dev) const override;

private:
    QOSCAbstractType::list _array;
};

#endif // QOSCARRAY_H
