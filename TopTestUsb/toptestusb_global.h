#ifndef TOPTESTUSB_GLOBAL_H
#define TOPTESTUSB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOPTESTUSB_LIBRARY)
#  define TOPTESTUSBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOPTESTUSBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOPTESTUSB_GLOBAL_H