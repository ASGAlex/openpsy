#ifndef PLOTTER_GLOBAL_H
#define PLOTTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLOTTER_LIBRARY)
#  define PLOTTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLOTTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLOTTER_GLOBAL_H
