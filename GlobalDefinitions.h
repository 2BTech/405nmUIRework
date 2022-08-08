#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

#include <QtGlobal>

#include "ValueHandlers/modbusvaluedefs.h"

// This class holds global definitons such as working dir

#ifdef Q_OS_WIN
//#define WORKING_DIR "D:\\_Work\\405WorkingDir\\"
#define WORKING_DIR "D:/_Work/405WorkingDir/"
#else
#include <QDir>
//#define WORKING_DIR "/home/2b/"
#define WORKING_DIR QDir::currentPath().append('/')
#endif

#define BUILD_VER "2.0.0"
#define BUILD_DATE "08/2022"

#define UI_COLOR "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"

#endif // GLOBALDEFINITIONS_H
