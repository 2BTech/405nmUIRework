#include "serialmenuhandler.h"

SerialMenuHandler::SerialMenuHandler() : SerialHandlerBase("Menu")
{
#ifdef WRITE_TEST_DATA
    testWriteTimer.setInterval(600);
    connect(&testWriteTimer, &QTimer::timeout, this, &SerialMenuHandler::WriteTestString);
#endif

    writeTimer.setInterval(100);
}

//#define DEBUG_ECHO_MESSAGE
void SerialMenuHandler::EchoMessage(QByteArray message)
{
    //qDebug() << "SerialMenuHandler thread: " << QThread::currentThread();
    if (!message.endsWith("\r\n"))
    {
        message = message.append("\r\n");
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

    //qDebug() << "Menu ready read";

#ifdef Q_OS_WIN
    // Continue reading bytes until all are handled
    while (serialPort->bytesAvailable())
    {
#else
    // Continue reading bytes until all are handled
    while (serialPort->bytesAvailable())
    {
        // Read in each byte indiidually
        //serialPort->readData(&in, 1);
#endif

        if (!isInMenu)
        {
            //qDebug() << "Not in menu";
            // Read in each byte indiidually
            ReadChar(&in);
            //qDebug() << "In: " << in;
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
            //qDebug() << "In menu";
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
    PrintMenuMessage();
}

void SerialMenuHandler::ParseReceived()
{
    //qDebug() << "Serial menu handler received: " << received;

    char in = 0;

    ReadChar(&in);
    //serialPort->readAll();

    //qDebug() << "Read in: " << in << " : " << QString::number(in);
    QueueMessage(QByteArray().append(in).append("\r\n"));

    switch (in)
    {
        // Print help
    case '?':
        PrintHelp();
        PrintMenuMessage();
        break;

        // Print all settings
    case '0':
        PrintAllSettings();
        PrintMenuMessage();
        break;

    case '1':
        PrintHeader();
        PrintMenuMessage();
        break;

    case '9':
        SetSerialNumber();
        break;

    case 'a':
        PrintAverageTime();
        PrintMenuMessage();
        break;

    case 'A':
        SetAverageTime();
        break;

    case 'c':
        PrintAdaptiveShort();
        PrintMenuMessage();
        break;

    case 'C':
        SetAdaptiveShort();
        break;

    case 'd':
        PrintAdaptiveLong();
        PrintMenuMessage();
        break;

    case 'D':
        SetAdaptiveLong();
        break;

    case 'e':
        PrintAdaptiveDiff();
        PrintMenuMessage();
        break;

    case 'E':
        SetAdaptiveDiff();
        break;

    case 'f':
        PrintAdaptivePer();
        PrintMenuMessage();
        break;

    case 'F':
        SetAdaptivePer();
        break;

    case 'g':
        PrintNOSlope();
        PrintMenuMessage();
        break;

    case 'G':
        SetNOSlope();
        break;

    case 'h':
        PrintNOZero();
        PrintMenuMessage();
        break;

    case 'H':
        SetNOZero();
        break;

    case 'i':
        PrintNOAnalog();
        PrintMenuMessage();
        break;

    case 'I':
        SetNOAnalog();
        break;

    case 'j':
        PrintNO2Slope();
        PrintMenuMessage();
        break;

    case 'J':
        SetNO2Slope();
        break;

    case 'k':
        PrintNO2Zero();
        PrintMenuMessage();
        break;

    case 'K':
        SetNO2Zero();
        break;

    case 'l':
        PrintNO2Analog();
        PrintMenuMessage();
        break;

    case 'L':
        SetNO2Analog();
        break;

    case 'm':
        PrintOzoneFlowSlope();
        PrintMenuMessage();
        break;

    case 'M':
        SetOzoneFlowSlope();
        break;

    case 'n':
        PrintCellFlowSlope();
        PrintMenuMessage();
        break;

    case 'N':
        SetCellFlowSlope();
        break;

    case 'o':
        PrintDate();
        PrintMenuMessage();
        break;

    case 'O':
        SetDate();
        break;

    case 'p':
        PrintTime();
        PrintMenuMessage();
        break;

    case 'P':
        SetTime();
        break;

    case 'q':
        PrintMode();
        PrintMenuMessage();
        break;

    case 'Q':
        SetMode();
        break;

    case 'X':
    case 'x':
        QueueMessage("Exiting Menu\r\n");
        isInMenu = false;
        while (!menuBlockedMessages.isEmpty())
        {
            QueueMessage(menuBlockedMessages.takeFirst());
        }
        break;

    default:
        QueueMessage("Unknown Command\r\n");
        PrintMenuMessage();
        break;
    }
}

// Prints all settings to the serial port
void SerialMenuHandler::PrintAllSettings()
{
    PrintSerialNumber();
    PrintAverageTime();
    PrintMode();
    PrintAdaptiveShort();
    PrintAdaptiveLong();
    PrintAdaptiveDiff();
    PrintAdaptivePer();
    PrintNOSlope();
    PrintNOZero();
    PrintNOAnalog();
    PrintNO2Slope();
    PrintNO2Zero();
    PrintNO2Analog();
    PrintCellFlowSlope();
    PrintOzoneFlowSlope();
    PrintDate();
    PrintTime();
}

// Prints the header to the serial port
void SerialMenuHandler::PrintHeader()
{
    QueueMessage("Serial#,LOG#,NO2,NO,NOx,CTemp,CPress,CFlow,OZFlow,PDVs,PDVo3,STemp,ErrorByte,Date,Time,Mode,Duty%\r\n");
}

// Gets a serial number for the device
void SerialMenuHandler::PrintSerialNumber()
{
    QueueMessage(QString("Serial Number: ").append(SettingsHandler::GetInstance()->GetSerialNumber()->ToString()).append("\r\n").toLatin1());
}

void SerialMenuHandler::SetSerialNumber()
{
    //GetSerialNumber();
    QueueMessage("Current Serial Number: " + SettingsHandler::GetInstance()->GetSerialNumber()->ToString().toLatin1() + "\r\n");
    QueueMessage("Change serial number?(Y/n)\r\n");

#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif

        char temp = 0;
        ReadChar(&temp);
        if (temp != 'Y' && temp != 'y' && temp != '\n' && temp != '\r')
        {
#ifdef USE_EXT_SER
            disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
            connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
            disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
            connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
            disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
            connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
            PrintMenuMessage();
            return;
        }
        else
        {
            QueueMessage("Enter password: ");
            received.clear();
            serialPort->readAll();

#ifdef USE_EXT_SER
            disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
            connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
            disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
            connect(serialPort, &SerialPort::ReadyRead, this, [=]() {

#else
            disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
            connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif
#endif
                char temp = 0;
                while (serialPort->bytesAvailable())
                {
                    ReadChar(&temp);

                    //qDebug() << "Read: " << temp;

                    if (temp == '\r' || temp == '\n')
                    {
                        qDebug() << "Entered password: " << received;
                        serialPort->readAll();
                        if (received == "boulder123")
                        {
                            received.clear();
                            QueueMessage("\r\nEnter new serial number\r\n");

#ifdef USE_EXT_SER
                            disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                            connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                            disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                            connect(serialPort, &SerialPort::ReadyRead, this, [=]() {

#else
                            disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                            connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif
#endif
                                char temp = 0;
                                while (serialPort->bytesAvailable())
                                {
                                    ReadChar(&temp);

                                    if (temp == '\r' || temp == '\n')
                                    {
                                        int sn = received.toInt();
                                        received.clear();
                                        if (sn >= 1000 && sn <= 9999)
                                        {
                                            SettingsHandler::GetInstance()->GetSerialNumber()->setValue(sn);
                                            QueueMessage("\r\nSet Serial Number to " + SettingsHandler::GetInstance()->GetSerialNumber()->ToString().toLatin1() + "\r\n");
                                        }
                                        else
                                        {

                                            qDebug() << "Entered value: " << sn << (sn >= 1000 ? "true" : "false") << (sn <= 9999 ? "true" : "false");
                                            QueueMessage("\r\nInvalid\r\n");
                                        }
                                        PrintMenuMessage();
                                    }
                                    else if (temp == 4 || temp == 8)
                                    {
                                        received = received.remove(received.count() - 1, 1);
                                        QueueMessage("\b \b");
                                    }
                                    else
                                    {
                                        received.append(temp);
                                        QueueMessage(QByteArray().append(temp));
                                    }
                                }
                                });
                                }
                        else
                        {
                            received.clear();
                            QueueMessage("\r\nIncorrect\r\n");
                            PrintMenuMessage();

#ifdef USE_EXT_SER
                            disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                            connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                            disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                            connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                            disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                            connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                        }
                    }
                    else if (temp == 8 || temp == 4)
                    {
                        received = received.remove(received.count() - 1, 1);
                        QueueMessage("\b \b");
                    }
                    else
                    {
                        received.append(temp);
                        QueueMessage(QByteArray().append(temp));
                    }
                }
            });
        }
    });
}

// Prints the help message
void SerialMenuHandler::PrintHelp()
{
    QueueMessage("Help menu:\r\n");
    QueueMessage("? - Print Help\r\n");
    QueueMessage("0 - Print All Settings\r\n");
    QueueMessage("1 - Print Header\r\n");
    QueueMessage("A - Set averaging time\r\n");
    QueueMessage("a - Print Averaging time\r\n");
    QueueMessage("C - Set Adaptive Short\r\n");
    QueueMessage("c - Print Adaptive Short\r\n");
    QueueMessage("D - Set Adaptive Long\r\n");
    QueueMessage("d - Print Adaptive Long\r\n");
    QueueMessage("E - Set Adaptive Difference\r\n");
    QueueMessage("e - Print Adaptive Difference\r\n");
    QueueMessage("F - Set Adaptive Percent\r\n");
    QueueMessage("f - Print Adaptive Percent\r\n");
    QueueMessage("G - Set NO Slope\r\n");
    QueueMessage("g - Print NO Slope\r\n");
    QueueMessage("H - Set NO Zero\r\n");
    QueueMessage("h - Print NO Zero\r\n");
    QueueMessage("I - Set NO V scale\r\n");
    QueueMessage("i - Print NO V scale\r\n");
    QueueMessage("J - Set NO2 Slope\r\n");
    QueueMessage("j - Print NO2 Slope\r\n");
    QueueMessage("K - Set NO2 Zero\r\n");
    QueueMessage("k - Print NO2 Zero\r\n");
    QueueMessage("L - Set NO2 V Scale\r\n");
    QueueMessage("l - Print NO2 V Scale\r\n");
    QueueMessage("M - Set Ozone Flow Slope\r\n");
    QueueMessage("m - Print Ozone Flow Slope\r\n");
    QueueMessage("N - Set Cell Flow Slope\r\n");
    QueueMessage("n - Print Cell Flow Slope\r\n");
    QueueMessage("O - Set Date\r\n");
    QueueMessage("o - Print Date\r\n");
    QueueMessage("P - Set Time\r\n");
    QueueMessage("P - Print Time\r\n");
    QueueMessage("Q - Set Mode\r\n");
    QueueMessage("Q - Print Mode\r\n");
    QueueMessage("X - Exit Serial Menu\r\n");
}

// Avgerage Time
void SerialMenuHandler::SetAverageTime()
{
    PrintAverageTime();

    QueueMessage("Enter a new averaging time:\r\n");
    QueueMessage("1 - 5 Seconds\r\n");
    QueueMessage("2 - 1 Minute\r\n");
    QueueMessage("3 - 5 Minutes\r\n");
    QueueMessage("4 - 1 Hour\r\n");

    // Reading bytes here, not in ready read so will temporarily disble the connect
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        ReadChar(&temp);
        qDebug() << "Read in byte: " << temp;
        QueueMessage("Setting avergaing time to ");
        switch (temp)
        {
        case '1':
            QueueMessage("5 Seconds\r\n");
            break;

        case '2':
            QueueMessage("1 Minute\r\n");
            break;

        case '3':
            QueueMessage("5 Minutes\r\n");
            break;

        case '4':
            QueueMessage("1 Hour\r\n");
            break;

        default:
            QueueMessage("Invalid\r\n");
            break;
        }
        if (temp >= '1' && temp <= '4')
        {
            SettingsHandler::GetInstance()->GetAverageTime()->setValue_str(QString().append(temp));
        }
#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintAverageTime()
{
    switch (SettingsHandler::GetInstance()->GetAverageTime()->getValue())
    {
    case 1:
        QueueMessage("Averaging Time: 5 secs\r\n");
        break;

    case 2:
        QueueMessage("Averaging Time: 1 min\r\n");
        break;

    case 3:
        QueueMessage("Averaging Time: 5 mins\r\n");
        break;

    case 4:
        QueueMessage("Averaging Time: 1 hour\r\n");
        break;

    default:
        QueueMessage("Averaging Time: invalid\r\n");
        break;
    }
}

void SerialMenuHandler::SetAdaptiveShort()
{
    QueueMessage("Current adaptive short length: " + SettingsHandler::GetInstance()->GetAdShort()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new adaptive short length (1 - 5):\r\n");
    received.clear();

#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        int val = received.toInt(&ok);
        if (ok)
        {
            if (val >= 1 && val <= 5)
            {
                QueueMessage("Setting adaptive short length to " + received + " from " + SettingsHandler::GetInstance()->GetAdShort()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetAdShort()->setValue(val);
            }
            else
            {
                QueueMessage("Invalid value\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintAdaptiveShort()
{
    QueueMessage("Adaptive Short Length: " + SettingsHandler::GetInstance()->GetAdShort()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetAdaptiveLong()
{
    QueueMessage("Current adaptive long length: " + SettingsHandler::GetInstance()->GetAdLong()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new adaptive long length (6 - 50):\r\n");

    received.clear();

#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                QueueMessage("\b \b");
                received.remove(received.count() - 1, 1);
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        int val = received.toInt(&ok);
        if (ok)
        {
            if (val >= 6 && val <= 50)
            {
                QueueMessage("Setting adaptive long length to " + received + " from " + SettingsHandler::GetInstance()->GetAdLong()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetAdLong()->setValue(val);
            }
            else
            {
                QueueMessage("Invalid value\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintAdaptiveLong()
{
    QueueMessage("Adaptive Long Length: " + SettingsHandler::GetInstance()->GetAdLong()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetAdaptiveDiff()
{
    QueueMessage("Current adaptive difference: " + SettingsHandler::GetInstance()->GetAdDiff()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new adaptive difference (0 - 50):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        int val = received.toInt(&ok);
        if (ok)
        {
            if (val >= 0 && val <= 50)
            {
                QueueMessage("Setting adaptive difference to " + received + " from " + SettingsHandler::GetInstance()->GetAdDiff()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetAdDiff()->setValue(val);
            }
            else
            {
                QueueMessage("Invalid value\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintAdaptiveDiff()
{
    QueueMessage("Adaptive Difference: " + SettingsHandler::GetInstance()->GetAdDiff()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetAdaptivePer()
{
    QueueMessage("Current adaptive percent: " + SettingsHandler::GetInstance()->GetAdPercent()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new adaptive percent (0 - 10):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        int val = received.toInt(&ok);
        if (ok && val >= 0 && val <= 10)
        {
            QueueMessage("Setting adaptive percent length to " + received + " from " + SettingsHandler::GetInstance()->GetAdPercent()->ToString().toLatin1() + "\r\n");
            SettingsHandler::GetInstance()->GetAdPercent()->setValue(val);
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintAdaptivePer()
{
    QueueMessage("Adaptive Percent: " + SettingsHandler::GetInstance()->GetAdPercent()->ToString().toLatin1() + " \r\n");
}

void SerialMenuHandler::SetNOSlope()
{
    QueueMessage("Current NO Slope: " + SettingsHandler::GetInstance()->GetNOSlope()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new NO Slope (0.20 - 3.00):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        float val = received.toFloat(&ok);
        if (ok)
        {
            if (val >= 0.2f && val <= 3)
            {
                QueueMessage("Setting NO Slope to " + received + " from " + SettingsHandler::GetInstance()->GetNOSlope()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetNOSlope()->setValue(val);
            }
            else
            {
                QueueMessage("Invalid value\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintNOSlope()
{
    QueueMessage("NO Slope: " + SettingsHandler::GetInstance()->GetNOSlope()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetNOZero()
{
    QueueMessage("Current NO Zero: " + SettingsHandler::GetInstance()->GetNOZero()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new NO Zero (-50.0 - 50.0):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        serialPort->readAll();
        QueueMessage("\r\n");

        bool ok;
        float val = received.toFloat(&ok);
        if (ok)
        {
            if (val >= -50.9f && val <= 50.9f)
            {
                QueueMessage("Setting NO Zero to " + received + " from " + SettingsHandler::GetInstance()->GetNOZero()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetNOZero()->setValue(val);
            }
            else
            {
                QueueMessage("Invalid value\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintNOZero()
{
    QueueMessage("NO Zero: " + SettingsHandler::GetInstance()->GetNOZero()->ToString().toLatin1() + " \r\n");
}

void SerialMenuHandler::SetNOAnalog()
{
    QueueMessage("Current NO V Scale: 1 V = " + SettingsHandler::GetInstance()->GetAnalogNO()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new NO V Scale (250 - 1000):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::ReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        int val = received.toInt(&ok);
        if (ok)
        {
            if (val >= 250 && val <= 1000)
            {
                QueueMessage("Setting NO V Scale to " + received + " from " + SettingsHandler::GetInstance()->GetAnalogNO()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetAnalogNO()->setValue(val);
            }
            else
            {
                QueueMessage("Value out of range\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintNOAnalog()
{
    QueueMessage("NO Analog V Scale: 1V = " + SettingsHandler::GetInstance()->GetAnalogNO()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetNO2Slope()
{
    QueueMessage("Current NO2 Slope: " + SettingsHandler::GetInstance()->GetNO2Slope()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new NO2 Slope (0.20 - 3.00):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        float val = received.toFloat(&ok);
        if (ok)
        {
            if (val >= 0.2f && val <= 3)
            {
                QueueMessage("Setting NO2 Slope to " + received + " from " + SettingsHandler::GetInstance()->GetNO2Slope()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetNO2Slope()->setValue(val);
            }
            else
            {
                QueueMessage("Value out of range\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintNO2Slope()
{
    QueueMessage("NO2 Slope: " + SettingsHandler::GetInstance()->GetNO2Slope()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetNO2Zero()
{
    QueueMessage("Current NO2 Zero: " + SettingsHandler::GetInstance()->GetNO2Zero()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new NO2 Zero (-50.0 - 50.0):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        float val = received.toFloat(&ok);
        if (ok)
        {
            if (val >= -50.9f && val <= 50.9f)
            {
                QueueMessage("Setting NO2 Zero to " + received + " from " + SettingsHandler::GetInstance()->GetNO2Zero()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetNO2Zero()->setValue(val);
            }
            else
            {
                QueueMessage("Value out of range\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintNO2Zero()
{
    QueueMessage("NO2 Zero: " + SettingsHandler::GetInstance()->GetNO2Zero()->ToString().toLatin1() + " \r\n");
}

void SerialMenuHandler::SetNO2Analog()
{
    QueueMessage("Current NO2 V Scale: 1 V = " + SettingsHandler::GetInstance()->GetAnalogNO2()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new NO2 V Scale (250 - 1000):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        int val = received.toInt(&ok);
        if (ok)
        {
            if (val >= 250 && val <= 1000)
            {
                QueueMessage("Setting NO2 V Scale to " + received + " from " + SettingsHandler::GetInstance()->GetAnalogNO2()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetAnalogNO2()->setValue(val);
            }
            else
            {
                QueueMessage("Value out of range\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintNO2Analog()
{
    QueueMessage("NO2 V Scale: 1V = " + SettingsHandler::GetInstance()->GetAnalogNO2()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetOzoneFlowSlope()
{
    QueueMessage("Current Ozone Flow Slope: " + SettingsHandler::GetInstance()->GetOzoneFlowSlope()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new Ozone Flow Slope (0.20 - 3.00):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        float val = received.toFloat(&ok);
        if (ok)
        {
            if (val >= 0.2f && val <= 3)
            {
                QueueMessage("Setting Ozone Flow Slope to " + received + " from " + SettingsHandler::GetInstance()->GetOzoneFlowSlope()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetOzoneFlowSlope()->setValue(val);
            }
            else
            {
                QueueMessage("Value out of range\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintOzoneFlowSlope()
{
    QueueMessage("Ozone Flow Slope: " + SettingsHandler::GetInstance()->GetOzoneFlowSlope()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetCellFlowSlope()
{
    QueueMessage("Current Cell Flow Slope: " + SettingsHandler::GetInstance()->GetCellFlowSlope()->ToString().toLatin1() + "\r\n");
    QueueMessage("Enter a new Cell Flow Slope (0.20 - 3.00):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        bool ok;
        float val = received.toFloat(&ok);
        if (ok)
        {
            if (val >= 0.2f && val <= 3)
            {
                QueueMessage("Setting Cell Flow Slope to " + received + " from " + SettingsHandler::GetInstance()->GetCellFlowSlope()->ToString().toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetCellFlowSlope()->setValue(val);
            }
            else
            {
                QueueMessage("Value out of range\r\n");
            }
        }
        else
        {
            QueueMessage("Invalid value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintCellFlowSlope()
{
    QueueMessage("Cell Flow Slope: " + SettingsHandler::GetInstance()->GetCellFlowSlope()->ToString().toLatin1() + "\r\n");
}

void SerialMenuHandler::SetMode()
{
    PrintMode();

    QueueMessage("Enter a new Measurement Mode:\r\n");
    QueueMessage("1 - NO\r\n");
    QueueMessage("2 - NO2\r\n");
    QueueMessage("3 - NO & NO2\r\n");
    QueueMessage("Enter ctrl+d to cancel\r\n");

#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        ReadChar(&temp);

        qDebug() << "Read in byte: " << temp;
        temp -= 0x30;
        QueueMessage("Setting avergaing time to ");
        switch (temp)
        {
        case 0:
            QueueMessage("Warming up\r\n");
            break;

        case 1:
            QueueMessage("NO\r\n");
            break;

        case 2:
            QueueMessage("NO2\r\n");
            break;

        case 3:
            QueueMessage("NO & NO2\r\n");
            break;

        default:
            QueueMessage("Invalid\r\n");
            break;
        }
        if (temp >= 1 && temp <= 3)
        {
            SettingsHandler::GetInstance()->GetMode()->setValue(temp);
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintMode()
{
    QueueMessage("Measurement Mode: ");
    switch (SettingsHandler::GetInstance()->GetMode()->getValue())
    {
    case 1:
        QueueMessage("NO\r\n");
        break;

    case 2:
        QueueMessage("NO2\r\n");
        break;

    case 3:
        QueueMessage("NO & NO2\r\n");
        break;

    default:
        QueueMessage("Invalid\r\n");
        break;
    }
}

void SerialMenuHandler::SetDate()
{
    QueueMessage("Current Date: " + QDate::currentDate().toString("dd/MM/yyyy").toLatin1() + "\r\n");
    QueueMessage("Enter a new date: dd/mm/yyyy\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
                break;
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
        {
            return;
        }

        QueueMessage("\r\n");
        serialPort->readAll();

        QStringList split = QString(received).split('/');
        if (split.count() == 3)
        {
            QDate nDate = QDate(split[2].toInt(), split[1].toInt(), split[0].toInt());
            if (nDate.isValid())
            {
                QueueMessage("Setting date to " + nDate.toString("dd/MM/yyyy").toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetDate()->setValue(nDate.toString("ddMMyy"));
            }
            else
            {
                QueueMessage("Received invalid date\r\n");
                qDebug() << "Invalid date: " << nDate;
            }
        }
        else
        {
            QueueMessage("Invalid Value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintDate()
{
    QueueMessage("Date: " + QDate::currentDate().toString("dd/MM/yyyy").toLatin1() + "\r\n");
}

void SerialMenuHandler::SetTime()
{
    QueueMessage("Current Time: " + QTime::currentTime().toString("hh:mm:ss").toLatin1() + "\r\n");
    QueueMessage("Enter a new time (hh:mm:ss):\r\n");

    received.clear();
#ifdef USE_EXT_SER
    disconnect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QextSerialPort::readyRead, this, [=]() {
#else

#ifndef Q_OS_WIN
    disconnect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &SerialPort::ReadyRead, this, [=]() {
#else
    disconnect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
    connect(serialPort, &QSerialPort::readyRead, this, [=]() {
#endif

#endif
        char temp = 0;
        while (serialPort->bytesAvailable())
        {
            ReadChar(&temp);
            if (temp == '\n' || temp == '\r')
            {
                break;
            }
            // Delete key
            else if (temp == 8)
            {
                received.remove(received.count() - 1, 1);
                QueueMessage("\b \b");
            }
            else if (temp == 0 || temp == 4)
            {
#ifdef USE_EXT_SER
                disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
                disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
                disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
                connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
                PrintMenuMessage();
                return;
            }
            else
            {
                received.append(temp);
                QueueMessage(QByteArray().append(temp));
            }
        }
        // If temp != \n or \r, then the read is not complete
        if (temp != '\r' && temp != '\n')
            return;

        QueueMessage("\r\n");
        serialPort->readAll();

        QStringList split = QString(received).split(':');
        if (split.count() == 3)
        {
            QTime nTime = QTime(split[0].toInt(), split[1].toInt(), split[2].toInt());
            if (nTime.isValid())
            {
                QueueMessage("Setting date to " + nTime.toString("hh:mm:ss").toLatin1() + "\r\n");
                SettingsHandler::GetInstance()->GetTime()->setValue(nTime.toString("hhmmss"));
            }
            else
            {
                QueueMessage("Received invalid time\r\n");
                qDebug() << "Invalid time: " << nTime;
            }
        }
        else
        {
            QueueMessage("Invalid Value\r\n");
        }

#ifdef USE_EXT_SER
        disconnect(serialPort, &QextSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QextSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#else

#ifndef Q_OS_WIN
        disconnect(serialPort, &SerialPort::ReadyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &SerialPort::ReadyRead, this, &SerialMenuHandler::OnReadyRead);
#else
        disconnect(serialPort, &QSerialPort::readyRead, Q_NULLPTR, Q_NULLPTR);
        connect(serialPort, &QSerialPort::readyRead, this, &SerialMenuHandler::OnReadyRead);
#endif

#endif
        PrintMenuMessage();
        });
        }

void SerialMenuHandler::PrintTime()
{
    QueueMessage("Time: " + QTime::currentTime().toString("hh:mm:ss").toLatin1() + "\r\n");
}

void SerialMenuHandler::PrintMenuMessage()
{
    //    QueueMessage("Serial Menu:\r\n");
    //    QueueMessage("Press ? for list of commands\r\n");
    //    QueueMessage("Enter Upper case to display value and lower to set\r\n");
    //    QueueMessage("Press X to exit\r\n");
    QueueMessage("menu>");
}

void SerialMenuHandler::ReadChar(char* chr)
{
#ifndef Q_OS_WIN
    serialPort->readData(chr, 1);
#else
    serialPort->read(chr, 1);
#endif
}









