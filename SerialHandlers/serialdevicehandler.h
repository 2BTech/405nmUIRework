#ifndef SERIALDEVICEHANDLER_H
#define SERIALDEVICEHANDLER_H

#include <QDate>
#include <QTime>

#include "ValueHandlers/datahandler.h"

#include "serialhandlerbase.h"

// This class is in charge of reading in and parsing data from the 405
class SerialDeviceHandler : public SerialHandlerBase
{
    Q_OBJECT
public:
    SerialDeviceHandler();

signals:
    // Emits the dataline string
    void ReceivedDataline(QByteArray dataline);

private:
    // First step in the parsing process
    void ParseReceived() override;

    // Attempts to parse received as a dataline
    void ParseAsDataline();
};

#endif // SERIALDEVICEHANDLER_H
