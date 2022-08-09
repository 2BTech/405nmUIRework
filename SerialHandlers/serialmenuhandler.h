#ifndef SERIALMENUHANDLER_H
#define SERIALMENUHANDLER_H

//#define WRITE_TEST_DATA

#include <QThread>

#include "ValueHandlers/settingshandler.h"
#include "ValueHandlers/datahandler.h"
#include "ValueHandlers/ValueObjects/valueobjects.h"

#include "serialhandlerbase.h"

// This class handles the serial menu functionality of the app
class SerialMenuHandler : public SerialHandlerBase
{
public:
    SerialMenuHandler();

#ifdef WRITE_TEST_DATA
    void OpenSerialPort(QString portName) override
    {
        SerialHandlerBase::OpenSerialPort(portName);
        testWriteTimer.start();
    }
#endif

public slots:
    void EchoMessage(QByteArray message);

private slots:
#ifdef WRITE_TEST_DATA
    void WriteTestString();
#endif

    // Override base version to handle opening the menu
    void OnReadyRead() override;

    // First step in getting the serial number
    void SetSerialNumberStep1();

private:
#ifdef WRITE_TEST_DATA
    QTimer testWriteTimer;
#endif

    // Flag used to track if the seria menu is open
    bool isInMenu = false;

    // Holds messages that were blocked while the user was in the serial menu
    QQueue<QByteArray> menuBlockedMessages;

    // First step in the parsing process
    void ParseReceived() override;

    // Outputs the serial menu header
    void OutputMenuHeader();

    // Prints all settings to the serial port
    void PrintAllSettings();

    // Prints the header to the serial port
    void PrintHeader();

    // Gets a serial number for the device
    void SetSerialNumber();
    void GetSerialNumber();

    // Prints the help message
    void PrintHelp();

    // Avgerage Time
    void SetAverageTime();
    void GetAverageTime();

    // Adaptive short
    void SetAdaptiveShort();
    void GetAdaptiveShort();

    // Adaptive long
    void SetAdaptiveLong();
    void GetAdaptiveLong();

    // Adaptive diff
    void SetAdaptiveDiff();
    void GetAdaptiveDiff();

    // Adaptive per
    void SetAdaptivePer();
    void GetAdaptivePer();

    // NO Slope
    void SetNOSlope();
    void GetNOSlope();

    // NO Zero
    void SetNOZero();
    void GetNOZero();

    // NO Analog
    void SetNOAnalog();
    void GetNOAnalog();

    // NO2 Slope
    void SetNO2Slope();
    void GetNO2Slope();

    // NO2 Zero
    void SetNO2Zero();
    void GetNO2Zero();

    // NO1 Analog
    void SetNO2Analog();
    void GetNO2Analog();

    // Cell Flow Slope
    void SetCellFlowSlope();
    void GetCellFlowSlope();

    // Ozone Flow Slope
    void SetOzoneFlowSlope();
    void GetOzoneFlowSlope();

    // Mode
    void SetMode();
    void GetMode();

    // Date
    void SetDate();
    void GetDate();

    // Time
    void SetTime();
    void GetTime();

    // Password
    bool GetPassword();
};

#endif // SERIALMENUHANDLER_H
