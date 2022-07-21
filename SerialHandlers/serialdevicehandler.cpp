#include "serialdevicehandler.h"

SerialDeviceHandler::SerialDeviceHandler() : SerialHandlerBase()
{

}

void SerialDeviceHandler::ParseReceived()
{
    // Return if received is empty. This condition should not happen, but it guarantees this
    if (received.isEmpty())
    {
        return;
    }

    switch(received[0])
    {
    // Dataline
    case '$':
        ParseAsDataline();
        break;

    default:
        break;
    }
}

void SerialDeviceHandler::ParseAsDataline()
{
    // Split received dataline into each section
    QStringList split = QString(received).split(',');

    QString finalDataline;
    QString sqlInsert;

    // Make sure the dataline is complte
    if (split.count() != 18)
    {
        qDebug() << "Received incomplte dataline";
        return;
    }

    // Remove the makring character
    split.removeFirst();

    // Get the serial number
    int serialNumber = split[0].toInt();

    // Get the log number
    int logNumber = split[1].toInt();

    // Get NO2
    float NO2 = split[2].toFloat();

    // Get NO
    float NO = split[3].toFloat();

    // Get NOx
    float NOx = split[4].toFloat();

    // Get cell temp
    float cellTemp = split[5].toFloat();

    // Get cell press
    float cellPress = split[6].toFloat();

    // Get cell fow
    float cellFlow = split[7].toFloat();

    // Get ozone flow
    float ozoneFlow = split[8].toFloat();

    // Get pdva
    float pdva = split[9].toFloat();

    // Get pdvb
    float pdvB = split[10].toFloat();

    // Get scrubber temp
    float scrubberTemp = split[11].toFloat();

    // Get error byte
    int errorByte = split[12].toInt();

    // Get date
    QDate date = QDate::fromString(split[13], "dd/MM/yy");
    date = date.addYears(100);

    // Get time
    QTime time = QTime::fromString(split[14]);

    // Get mode
    int mode = split[15].toInt();

    // Get duty percent
    int dutyPercent = split[16].toInt();

    // Rejoin the elements to get the dataline.
    finalDataline = split.join(',');

    DataHandler::GetInstance()->ReceiveDataline(serialNumber, logNumber, NO2, NO, NOx, cellTemp, cellPress, cellFlow, pdva, pdvB, scrubberTemp, ozoneFlow, errorByte, split[13], split[14], mode, dutyPercent);
    DataHandler::GetInstance()->LogDataline(finalDataline.toLatin1());

    // Replace date and time in split to be of the proper format
    split[13] = date.toString("yyyy/MM/dd").insert(0, '\'').append('\'');
    split[14] = time.toString(time.toString("hh:mm:ss")).insert(0, '\'').append('\'');

    sqlInsert = split.join(',');

//    qDebug() << "Received dataline: " << finalDataline;
//    qDebug() << "SQL insert: " << sqlInsert;

    emit ReceivedDataline(finalDataline.toLatin1());
}
