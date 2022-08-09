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

    // Prints a serial number for the device
    void SetSerialNumber();
    void PrintSerialNumber();

    // Prints the help message
    void PrintHelp();

    // Avgerage Time
    void SetAverageTime();
    void PrintAverageTime();

    // Adaptive short
    void SetAdaptiveShort();
    void PrintAdaptiveShort();

    // Adaptive long
    void SetAdaptiveLong();
    void PrintAdaptiveLong();

    // Adaptive diff
    void SetAdaptiveDiff();
    void PrintAdaptiveDiff();

    // Adaptive per
    void SetAdaptivePer();
    void PrintAdaptivePer();

    // NO Slope
    void SetNOSlope();
    void PrintNOSlope();

    // NO Zero
    void SetNOZero();
    void PrintNOZero();

    // NO Analog
    void SetNOAnalog();
    void PrintNOAnalog();

    // NO2 Slope
    void SetNO2Slope();
    void PrintNO2Slope();

    // NO2 Zero
    void SetNO2Zero();
    void PrintNO2Zero();

    // NO1 Analog
    void SetNO2Analog();
    void PrintNO2Analog();

    // Cell Flow Slope
    void SetCellFlowSlope();
    void PrintCellFlowSlope();

    // Ozone Flow Slope
    void SetOzoneFlowSlope();
    void PrintOzoneFlowSlope();

    // Mode
    void SetMode();
    void PrintMode();

    // Date
    void SetDate();
    void PrintDate();

    // Time
    void SetTime();
    void PrintTime();

    void PrintMenuMessage();

    void ReadChar(char* chr);
};

#endif // SERIALMENUHANDLER_H
