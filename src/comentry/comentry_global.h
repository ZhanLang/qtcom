#ifndef COMENTRY_GLOBAL_H
#define COMENTRY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMENTRY_LIBRARY)
#  define COMENTRY_EXPORT Q_DECL_EXPORT
#else
#  define COMENTRY_EXPORT Q_DECL_IMPORT
#endif

#endif // COMENTRY_GLOBAL_H
