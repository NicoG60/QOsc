#ifndef QOSC_GLOBAL_H
#define QOSC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QOSC_LIBRARY)
#  define QOSC_EXPORT Q_DECL_EXPORT
#else
#  define QOSC_EXPORT Q_DECL_IMPORT
#endif

#endif // QOSC_GLOBAL_H
