#ifndef SERIALMENUHANDLER_H
#define SERIALMENUHANDLER_H

#include "serialhandlerbase.h"

// This class handles the serial menu functionality of the app
class SerialMenuHandler : public SerialHandlerBase
{
public:
    SerialMenuHandler();

public slots:
    void EchoMessage(QByteArray message);

private:
    // First step in the parsing process
    void ParseReceived() override;
};

#endif // SERIALMENUHANDLER_H
