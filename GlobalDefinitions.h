#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

#include <QtGlobal>

// This class holds global definitons such as working dir

#ifdef Q_OS_WIN
//#define WORKING_DIR "D:\\_Work\\405WorkingDir\\"
#define WORKING_DIR "D:/_Work/405WorkingDir/"
#else
#define WORKING_DIR "/home/2b/"
#endif

#define BUILD_VER "0.0.1"

#define UI_COLOR "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"

#endif // GLOBALDEFINITIONS_H
