#ifndef SERIALMENUHANDLER_H
#define SERIALMENUHANDLER_H

//#define WRITE_TEST_DATA

#include <QThread>

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
};

#endif // SERIALMENUHANDLER_H
