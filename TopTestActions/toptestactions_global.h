#ifndef TOPTESTACTIONS_GLOBAL_H
#define TOPTESTACTIONS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOPTESTACTIONS_LIBRARY)
#  define TOPTESTACTIONSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOPTESTACTIONSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOPTESTACTIONS_GLOBAL_H
