#include "mainwindow.h"
#include "SerialHandlers/serialdevicehandler.h"
#include "SerialHandlers/serialmenuhandler.h"
#include "GlobalDefinitions.h"
#include "ValueHandlers/settingshandler.h"

#include <QApplication>
#include <QThread>
#include <QFile>

//#define DEBUG_INIT_SER_HANDLERS
void InitialeSerialHandlers(SerialDeviceHandler* deviceHander, SerialMenuHandler* serialMenuHandler)
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
            line = QString(serialSettings.readLine()).split(',');
#ifdef DEBUG_INIT_SER_HANDLERS
            qDebug () << "Serial settings line: " << line;
#endif
            if (line.count() == 2)
            {
                if (line[0] == "ADuC")
                {
#ifdef DEBUG_INIT_SER_HANDLERS
                    qDebug() << "Connecting serial device handler to " << line[1].simplified();
#endif
                    deviceHander->OpenSerialPort(line[1].simplified());
                }
                else if (line[0] == "Menu")
                {
#ifdef DEBUG_INIT_SER_HANDLERS
                    qDebug() << "Connecting serial menu handler to " << line[1].simplified();
#endif
                    serialMenuHandler->OpenSerialPort(line[1].simplified());
                }
                else
                {
                    qDebug() << "ERROR: unknown settings in serial settings file: " << line;
                }
            }
        }
#ifdef DEBUG_INIT_SER_HANDLERS
        qDebug() << "Finished reading serial settings";
#endif
    }
    else
    {
        qDebug() << "ERROR: Failed to read serial settings file: " << serialSettings.fileName() << " : " << serialSettings.errorString();
    }
}

void InitializeModbus()
{
    //ModbusHandler::GetInstance()->DataTable()->ResizeOutputRegisters(TOTAL_MODBUS_REGISTERS);
    //ModbusHandler::GetInstance()->DataTable()->ResizeOutputCoils(2);

    int registerIndex = 0;
    int coilIndex = 0;

    ModbusHandler* modbus = ModbusHandler::GetInstance();

    const QList<BaseValueObject*> settings = SettingsHandler::GetInstance()->GetAllValues();
    SettingsHandler::GetInstance()->SetRegisterIndex(0);
    SettingsHandler::GetInstance()->SetCoilIndex(coilIndex++);
    for (BaseValueObject* set : settings)
    {
        set->SetRegisterIndex(registerIndex);

        registerIndex += set->GetNumRegisters();
    }
    SettingsHandler::GetInstance()->SetNumRegisters(registerIndex);


    const QList<BaseValueObject*> data = DataHandler::GetInstance()->GetAllValues();
    DataHandler::GetInstance()->SetRegisterIndex(registerIndex);
    DataHandler::GetInstance()->SetCoilIndex(coilIndex++);
    for (BaseValueObject* dat : data)
    {
        dat->SetRegisterIndex(registerIndex);

        registerIndex += dat->GetNumRegisters();
    }
    DataHandler::GetInstance()->SetNumRegisters(registerIndex);

    modbus->DataTable()->ResizeOutputRegisters(registerIndex);
    modbus->DataTable()->ResizeOutputCoils(coilIndex);

    modbus->RegisterValueObjects(settings);
    modbus->RegisterValueObjects(data);

    SettingsHandler::GetInstance()->connect(SettingsHandler::GetInstance(), &ValueHandlerBase::SetModbusCoil, modbus, &ModbusHandler::SetModbusCoil);
    DataHandler::GetInstance()->connect(DataHandler::GetInstance(), &ValueHandlerBase::SetModbusCoil, modbus, &ModbusHandler::SetModbusCoil);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << (QString("Start Generic 2B Touch Software: Ver") + BUILD_VER);
    qDebug() << (QString("Built using qt version: ") + qVersion());
    qDebug() << (QString("Built at: ") + __DATE__ + QString(" ") + __TIME__);
    qDebug() << (QString("Working dir: ") + WORKING_DIR);
    qDebug() << ("Started at: " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss a"));
    qDebug() << ("PID: " + QString::number(QCoreApplication::applicationPid()));
    qDebug() << "Main thread: " << QThread::currentThread();

    // Call this function to read in all settings right away
    SettingsHandler::GetInstance()->ReadSettingsFile();

    MainWindow w;
    w.show();

    QThread serialDeviceThread;
    SerialDeviceHandler serialDeviceHandler;

    QThread serialMenuThread;
    SerialMenuHandler serialMenuHandler;

    serialDeviceHandler.connect(&serialDeviceHandler, &SerialDeviceHandler::ReceivedDataline, &serialMenuHandler, &SerialMenuHandler::EchoMessage);
    serialDeviceHandler.connect(&serialDeviceHandler, &SerialDeviceHandler::EchoMessage, &serialMenuHandler, &SerialMenuHandler::EchoMessage);

    InitialeSerialHandlers(&serialDeviceHandler, &serialMenuHandler);

    serialDeviceHandler.MoveToThread(&serialDeviceThread);
    serialDeviceThread.start();
    //serialDeviceHandler.Disconnect();

    serialMenuHandler.MoveToThread(&serialMenuThread);
    serialMenuThread.start(QThread::HighestPriority);

    qDebug() << "Menu thread: " << &serialMenuThread;
    qDebug() << "Device thread: " << &serialDeviceThread;

    //InitializeModbus();

    return a.exec();
}
