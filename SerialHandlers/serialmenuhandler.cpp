#include "serialmenuhandler.h"

SerialMenuHandler::SerialMenuHandler() : SerialHandlerBase()
{
#ifdef WRITE_TEST_DATA
    testWriteTimer.setInterval(600);
    connect(&testWriteTimer, &QTimer::timeout, this, &SerialMenuHandler::WriteTestString);
#endif
}

//#define DEBUG_ECHO_MESSAGE
void SerialMenuHandler::EchoMessage(QByteArray message)
{
    qDebug() << "SerialMenuHandler thread: " << QThread::currentThread();
    if (!message.endsWith('\n'))
    {
        message = message.append('\n');
    }

#ifdef DEBUG_ECHO_MESSAGE
    qDebug() << "Echoing message: " << message;
#endif

    if (isInMenu)
    {
        menuBlockedMessages.append(message);
    }
    else
    {
        QueueMessage(message);
    }
}

void SerialMenuHandler::ParseReceived()
{
    qDebug() << "Serial menu handler received: " << received;
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

    // Continue reading bytes until all are handled
    while (serialPort->bytesAvailable())
    {
        // Read in each byte indiidually
        serialPort->read(&in, 1);

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
            if (in == '\r' || in == '\n')
            {
                // Parse data if some exists
                if (!received.isEmpty())
                {
                    ParseReceived();
                    received.clear();
                }
            }
            else
            {
                received.append(in);
            }
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
