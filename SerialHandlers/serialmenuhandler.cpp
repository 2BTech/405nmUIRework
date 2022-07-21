#include "serialmenuhandler.h"

SerialMenuHandler::SerialMenuHandler()
{

}

void SerialMenuHandler::EchoMessage(QByteArray message)
{
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
