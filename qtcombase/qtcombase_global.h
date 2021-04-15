#ifndef QTCOMBASE_GLOBAL_H
#define QTCOMBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTCOMBASE_LIBRARY)
#  define QTCOMBASE_EXPORT Q_DECL_EXPORT
#else
#  define QTCOMBASE_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCOMBASE_GLOBAL_H
