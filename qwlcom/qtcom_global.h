#ifndef QTCOM_GLOBAL_H
#define QTCOM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTCOM_LIBRARY)
#  define QTCOM_EXPORT Q_DECL_EXPORT
#else
#  define QTCOM_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCOM_GLOBAL_H
