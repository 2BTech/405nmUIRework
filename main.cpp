#include "mainwindow.h"
#include "SerialHandlers/serialdevicehandler.h"
#include "GlobalDefinitions.h"

#include <QApplication>
#include <QThread>
#include <QFile>

//#define DEBUG_INIT_SER_HANDLERS
void InitialeSerialHandlers(SerialDeviceHandler* deviceHander)
{
    QFile serialSettings(QString(WORKING_DIR).append("SerialSettings.txt"));

#ifdef DEBUG_INIT_SER_HANDLERS
    qDebug() << "Opening file: " << serialSettings.fileName();
#endif

    if (serialSettings.open(QIODevice::ReadOnly))
    {
        QStringList line;
        while(!serialSettings.atEnd())
        {
            line = QString(serialSettings.readLine()).split('|');
            if (line.count() == 2)
            {
                if (line[0] == "ADuC")
                {
#ifdef DEBUG_INIT_SER_HANDLERS
                    qDebug() << "Connecting serial device handler to " << line[0].simplified();
#endif
                    deviceHander->OpenSerialPort(line[1].simplified());
                }
                else if (line[0] == "Menu")
                {
#ifdef DEBUG_INIT_SER_HANDLERS
                    qDebug() << "Connecting serial menu handler to " << line[0].simplified();
#endif
                }
                else
                {
                    qDebug() << "ERROR: unknown settings in serial settings file: " << line;
                }
            }
        }
    }
    else
    {
        qDebug() << "ERROR: Failed to read serial settings file: " << serialSettings.fileName() << " : " << serialSettings.errorString();
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QThread serialDeviceThread;
    SerialDeviceHandler serialDeviceHandler;

    InitialeSerialHandlers(&serialDeviceHandler);

    serialDeviceHandler.MoveToThread(&serialDeviceThread);
    serialDeviceThread.start();

    return a.exec();
}
