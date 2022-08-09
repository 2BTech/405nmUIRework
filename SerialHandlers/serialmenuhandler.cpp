#include "serialmenuhandler.h"

SerialMenuHandler::SerialMenuHandler() : SerialHandlerBase("Menu")
{
#ifdef WRITE_TEST_DATA
    testWriteTimer.setInterval(600);
    connect(&testWriteTimer, &QTimer::timeout, this, &SerialMenuHandler::WriteTestString);
#endif
}

//#define DEBUG_ECHO_MESSAGE
void SerialMenuHandler::EchoMessage(QByteArray message)
{
    //qDebug() << "SerialMenuHandler thread: " << QThread::currentThread();
    if (!message.endsWith('\n'))
    {
        message = message.append('\n');
    }

#ifdef DEBUG_ECHO_MESSAGE
    qDebug() << "Echoing message: " << message;
#endif

    if (isInMenu)
    {
#ifdef DEBUG_ECHO_MESSAGE
        qDebug() << "In menu";
#endif
        menuBlockedMessages.append(message);
    }
    else
    {
#ifdef DEBUG_ECHO_MESSAGE
        qDebug() << "Queuing message";
#endif
        QueueMessage(message);
    }
}

#ifdef WRITE_TEST_DATA
void SerialMenuHandler::WriteTestString()
{
    QueueMessage("1113,1252,-3.1,-2.2,-5.3,27.1,791.6,1468,74.9,0.967927,0.175400,109.5,0,21/07/22,13:47:48,3,19\n");
}
#endif

void SerialMenuHandler::OnReadyRead()
{
    // Holds the most recent character
    char in;

#ifdef Q_OS_WIN
    // Continue reading bytes until all are handled
    while (serialPort->bytesAvailable())
    {
        // Read in each byte indiidually
        serialPort->read(&in, 1);
#else
    // Continue reading bytes until all are handled
    while (serialPort->available())
    {
        // Read in each byte indiidually
        serialPort->readData(&in, 1);
#endif

        if (!isInMenu)
        {
            if (in == 'M' || in == 'm')
            {
                isInMenu = true;
                OutputMenuHeader();
            }
        }
        else
        {
            // Check if this signals the end of a line
//            if (in == '\r' || in == '\n')
//            {
//                // Parse data if some exists
//                if (!received.isEmpty())
//                {
//                    ParseReceived();
//                    received.clear();
//                }
//            }
//            else
//            {
//                received.append(in);
//            }
            ParseReceived();
        }
    }
    // Prevent an overflow condition if data is not being properly received. Max line size is around 100
    if (received.count() > 1000)
    {
        received.clear();
    }
}

void SerialMenuHandler::OutputMenuHeader()
{
    QueueMessage("Serial Menu:\r\n");
    QueueMessage("Press ? for list of commands\r\n");
    QueueMessage("Enter Upper case to display value and lower to set\r\n");
    QueueMessage("Press X to exit\r\n");
}

void SerialMenuHandler::ParseReceived()
{
    //qDebug() << "Serial menu handler received: " << received;

    char in;

#ifdef Q_OS_WIN
    serialPort->read(&in,  1);
#else
    serialPort->readData(&in,  1);
#endif

    switch(in)
    {
    // Print all settings
    case '0':
        PrintAllSettings();
        break;

    default:

        break;
    }
}

// Prints all settings to the serial port
void SerialMenuHandler::PrintAllSettings()
{
    GetSerialNumber();
    GetAverageTime();
}

// Prints the header to the serial port
void SerialMenuHandler::PrintHeader()
{
    QueueMessage("Serial#,LOG#,NO2,NO,NOx,CTemp,CPress,CFlow,OZFlow,PDVs,PDVo3,STemp,ErrorByte,Date,Time,Mode,Duty%\r\n");
}

// Gets a serial number for the device
void SerialMenuHandler::GetSerialNumber()
{
    QueueMessage(QString("Serial Number: ").append(SettingsHandler::GetInstance()->GetSetting("R")->ToString()).append('\n').toLatin1());
}

void SerialMenuHandler::SetSerialNumber()
{
#ifdef Q_OS_WIN
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialHandlerBase::OnReadyRead);
#else
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialHandlerBase::OnReadyRead);
#endif

    QueueMessage("Current serial number: " + SettingsHandler::GetInstance()->GetSetting("R")->ToString().toLatin1());
    QueueMessage("Change serial number? (Y,n)\n");

#ifdef Q_OS_WIN
    connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::SetSerialNumberStep1);
#else
    connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::SetSerialNumberStep1);
#endif
}

void SerialMenuHandler::SetSerialNumberStep1()
{
    char
}

// Prints the help message
void SerialMenuHandler::PrintHelp()
{

}

// Avgerage Time
void SerialMenuHandler::SetAverageTime()
{

}

void SerialMenuHandler::GetAverageTime()
{
    switch (dynamic_cast<ValueObject<uchar>*>(SettingsHandler::GetInstance()->GetSetting("A"))->getValue())
    {
    case 1:
        QueueMessage("Averaging Time: 5 secs\n");
        break;

    case 2:
        QueueMessage("Averaging Time: 1 min\n");
        break;

    case 3:
        QueueMessage("Averaging Time: 5 mins\n");
        break;

    case 4:
        QueueMessage("Averaging Time: 1 hour\n");
        break;

    default:
        QueueMessage("Averaging Time: invalid\n");
        break;
    }
}

bool SerialMenuHandler::GetPassword()
{

}
