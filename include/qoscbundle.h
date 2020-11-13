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

    inline QOSCPacket::list::iterator       begin()        { return elements.begin();  }
    inline QOSCPacket::list::const_iterator begin()  const { return elements.begin();  }
    inline QOSCPacket::list::const_iterator cbegin() const { return elements.cbegin(); }

    inline QOSCPacket::list::iterator       end()        { return elements.end();  }
    inline QOSCPacket::list::const_iterator end()  const { return elements.end();  }
    inline QOSCPacket::list::const_iterator cend() const { return elements.cend(); }

    inline int size() const noexcept { return elements.size(); }
    inline bool isEmpty() const noexcept { return elements.isEmpty(); }
    inline void clear() { elements.clear(); }

    inline const QOSCPacket::ptr& at(int i)         const { return elements.at(i); }
    inline       QOSCPacket::ptr& operator[](int i)       { return elements[i];    }
    inline const QOSCPacket::ptr& operator[](int i) const { return elements[i];    }

    inline void reserve(int size)                       { elements.reserve(size); }
    inline void append(const QOSCPacket::ptr& t)         { elements.append(t);     }
    inline void prepend(const QOSCPacket::ptr& t)        { elements.prepend(t);    }
    inline void insert(int i, const QOSCPacket::ptr& t)  { elements.insert(i, t);  }
    inline void replace(int i, const QOSCPacket::ptr& t) { elements.replace(i, t); }
    inline void removeAt(int i)                         { elements.removeAt(i);   }

    inline QOSCPacket::ptr takeAt(int i) { return elements.takeAt(i);   }
    inline QOSCPacket::ptr takeFirst()   { return elements.takeFirst(); }
    inline QOSCPacket::ptr takeLast()    { return elements.takeLast();  }

    inline       QOSCPacket::ptr& first()       { return elements.first(); }
    inline const QOSCPacket::ptr& first() const { return elements.first(); }
    inline       QOSCPacket::ptr& last()        { return elements.last();  }
    inline const QOSCPacket::ptr& last()  const { return elements.last();  }

    inline void removeFirst() { elements.removeFirst(); }
    inline void removeLast()  { elements.removeLast(); }

    inline QOSCBundle& operator+=(const QOSCPacket::ptr& t) { append(t); return *this; }
    inline QOSCBundle& operator<< (const QOSCPacket::ptr& t) { append(t); return *this; }

protected:
    void load(QIODevice* dev) override;

public:
    QOSCTimeTag time;
    QOSCPacket::list elements;
};

#endif // QOSCBUNDLE_H
