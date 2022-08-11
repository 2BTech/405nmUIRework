#include "serialdevicehandler.h"

SerialDeviceHandler::SerialDeviceHandler() : SerialHandlerBase("Device")
{
    writeTimer.disconnect();

    ConnectToSettingsObjects();

    netManager = new QNetworkAccessManager(this);
    connect(netManager, &QNetworkAccessManager::finished, this, &SerialDeviceHandler::OnFinishPostingSQLData);
}

SerialDeviceHandler::~SerialDeviceHandler()
{
    if (netManager)
    {
        netManager->deleteLater();
    }
}

//#define DEBUG_PARSE_RECEIVED
void SerialDeviceHandler::ParseReceived()
{
    // Return if received is empty. This condition should not happen, but it guarantees this
    if (received.isEmpty())
    {
        return;
    }

    QStringList split = QString(received).split(',');

    switch(received[0])
    {
    // Dataline
    case '$':
#ifdef DEBUG_PARSE_RECEIVED
        qDebug() << "Parsing as dataline";
#endif
        ParseAsDataline();
        break;

    case 'A':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
        if (split.count() == 2)
        {
            ParseSetting(split[0], split[1]);
        }
        break;

    case 'a':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
        WriteSetting(QString(received.at(0) - 0x20));
        break;

    case '>':
        WriteNextMessage();
        break;

    case '_':
        HandleAck();
        break;

    case '*':
        emit EchoMessage(received.remove(0, 1));
        break;

    default:
        qDebug() << "Unhanded message: " << received[0];
        break;
    }
}

void SerialDeviceHandler::ParseSetting(QString marker, QString value)
{
    BaseValueObject* val = SettingsHandler::GetInstance()->GetSetting(marker);
    if (val != Q_NULLPTR)
    {
        disconnect(val, &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
        val->setValue_str(value);
        disconnect(val, &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    }
}

void SerialDeviceHandler::WriteSetting(QString marker)
{
    WriteSetting(SettingsHandler::GetInstance()->GetSetting(marker));
}

void SerialDeviceHandler::WriteSetting(BaseValueObject* setting)
{
    if (setting)
    {
        writeQueue.append(setting->getMarker().toLatin1());
        writeQueue.append(setting->ToString().append('\n').toLatin1());

        acksList.append(setting->getMarker().append(setting->ToString()).toLatin1());
        acksToCheck.append(setting->getMarker().append(setting->ToString()).toLatin1());

        if (!isSendingMessage)
        {
            isSendingMessage = true;
            WriteNextMessage();
        }
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

    // Remove serial number
    split.takeFirst();
    // Remove duty percent
    split.takeLast();

    sqlInsert = split.join(',');

    //qDebug() << "Received dataline: " << finalDataline;
//    qDebug() << "SQL insert: " << sqlInsert;

    emit ReceivedDataline(finalDataline.toLatin1());

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDateTime receivedDateTime = QDateTime(date, time);

    QString tempURL = "http://localhost/WriteToDatabase.php?line=" + sqlInsert;
    tempURL.append(sqlInsert);
    netManager->get(QNetworkRequest(QUrl(tempURL)));

    qDebug() << "SQL post string: " << tempURL;

    if (qAbs(currentDateTime.secsTo(receivedDateTime)) > 2)
    {
        QString command = "sudo date --set=\"" + receivedDateTime.toString("yyyyMMdd hh:mm:ss") + "\"";
        //qDebug() << "System clock is wrong. Setting based off of data line. Command: " << command << " : " << currentDateTime.secsTo(receivedDateTime);
#ifndef Q_OS_WIN
        system(command.toLatin1());
#endif
    }

    if (serialNumber == -1 && writeQueue.isEmpty())
    {
        WriteAllSettings();
    }
}

void SerialDeviceHandler::OnFinishPostingSQLData(QNetworkReply* reply)
{
    if(reply->error() != QNetworkReply::NetworkError::NoError)
    {
        qDebug() << "Failed to post message to sql database: " << reply->errorString();
    }

    reply->deleteLater();
}

void SerialDeviceHandler::WriteAllSettings()
{
    WriteSetting("A");
    WriteSetting("C");
    WriteSetting("D");
    WriteSetting("E");
    WriteSetting("F");
    WriteSetting("I");
    WriteSetting("L");
    WriteSetting("G");
    WriteSetting("H");
    WriteSetting("J");
    WriteSetting("K");
    WriteSetting("M");
    WriteSetting("N");
    WriteSetting("O");
    WriteSetting("R");
}

void SerialDeviceHandler::HandleAck()
{
    // Remove marking character
    if(received.count() > 0)
    {
        received = received.remove(0, 1);

        //qDebug() << "Checking ack: " << received;

        int index = acksList.indexOf(received);
        if(index >= 0)
        {
            //qDebug() << "Received ack. Remaining: " << acksList.count();
            acksList.removeAt(index);
            //acksToCheck.removeAt(acksList.indexOf(received));
        }
        else
        {
            qDebug() << "Received ack for unknown message: " << received;
            for(int i = 0; i < acksList.count(); i++)
            {
                qDebug() << i << " : " << acksList[i];
            }
        }
    }

    WriteNextMessage();
}

void SerialDeviceHandler::CheckForAck()
{
    QByteArray message = acksToCheck.dequeue();

    int index = acksList.indexOf(message);
    if(index >= 0)
    {
        qDebug() << "Failed to receive ack for: " << message;
        if(missedAcksCounter.contains(message))
        {
            missedAcksCounter[message]++;
            if(missedAcksCounter[message] > 5)
            {
                return;
            }
        }
        else
        {
            missedAcksCounter[message] = 1;
        }
        char marker = message[0];
        message.remove(0, 1);
        writeQueue.append(QByteArray().append(marker));

        acksList.append(QByteArray().append(marker).append(message));
        acksToCheck.append(QByteArray().append(marker).append(message));

        writeQueue.append(message.append('\n'));

        if (!isSendingMessage)
        {
            isSendingMessage = true;
            WriteNextMessage();
        }
    }
}

void SerialDeviceHandler::ConnectToSettingsObjects()
{
    SettingsHandler* settinghandler = SettingsHandler::GetInstance();

    connect(settinghandler->GetSetting("A"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("C"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("D"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("E"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("F"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("I"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("L"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("G"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("H"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("J"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("K"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("M"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("N"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("O"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("R"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("P"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
    connect(settinghandler->GetSetting("Q"), &BaseValueObject::ValueChanged, this, &SerialDeviceHandler::OnSettingValueChange);
}

void SerialDeviceHandler::OnSettingValueChange()
{
    //qDebug() << "Called OnSettingValueChange";
    BaseValueObject* setting = dynamic_cast<BaseValueObject*>(sender());
    if (setting)
    {
        WriteSetting(setting);
    }
}

void SerialDeviceHandler::WriteNextMessage()
{
    if(writeQueue.count() > 0)
    {
        QByteArray message = writeQueue.dequeue();
        if(serialPort != Q_NULLPTR)
        {
#ifdef Q_OS_WIN
            serialPort->write(message.data(), message.count());
            serialPort->waitForBytesWritten(3000);
            qDebug() << "Writing in device: " << message;
#else
            serialPort->writeData(message.data(), message.count());
            //serialPort->waitForBytesWritten(3000);
            //qDebug() << "Writing: " << message;
#endif
        }
        else
        {
            qDebug() << "Skipping: " << message;
        }

        // Check if the ack has been received for the message after 30 secs
        if(message.count() > 2)
        {
            QTimer::singleShot(90000, this, SLOT(CheckForAck()));
        }

        if(writeQueue.count() == 0)
        {
            isSendingMessage = false;
        }
    }
//    else
//    {
//        qDebug() << "Recieved prompt to send message with empty queue";
//    }
}
