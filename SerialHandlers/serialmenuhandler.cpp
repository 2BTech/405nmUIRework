#include "serialmenuhandler.h"

SerialMenuHandler::SerialMenuHandler() : SerialHandlerBase()
{

}

#define DEBUG_ECHO_MESSAGE
void SerialMenuHandler::EchoMessage(QByteArray message)
{
    message = message.append('\n');

#ifdef DEBUG_ECHO_MESSAGE
    qDebug() << "Echoing message: " << message;
#endif

    writeQueue.append(message);

    if (!isSendingMessage)
    {
#ifdef DEBUG_ECHO_MESSAGE
        qDebug() << "Starting write queue";
#endif
        isSendingMessage = true;
        WriteNextMessage();
    }
#ifdef DEBUG_ECHO_MESSAGE
    else
    {
        qDebug() << "In progress of writing queue. Count: " << writeQueue.count();
    }
#endif
}

void SerialMenuHandler::ParseReceived()
{
    qDebug() << "Serial menu handler received: " << received;
}
