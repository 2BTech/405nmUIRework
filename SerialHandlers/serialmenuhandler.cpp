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
