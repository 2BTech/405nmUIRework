#include "serialmenuhandler.h"

SerialMenuHandler::SerialMenuHandler()
{

}

#define DEBUG_ECHO_MESSAGE
void SerialMenuHandler::EchoMessage(QByteArray message)
{
#ifdef DEBUG_ECHO_MESSAGE
    qDebug() << "Echoing message: " << message;
#endif

    writeQueue.append(message);

    if (!isSendingMessage)
    {
        isSendingMessage = true;
        WriteNextMessage();
    }
}

void SerialMenuHandler::ParseReceived()
{
    qDebug() << "Serial menu handler received: " << received;
}
