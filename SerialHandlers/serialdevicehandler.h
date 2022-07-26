#ifndef SERIALDEVICEHANDLER_H
#define SERIALDEVICEHANDLER_H

#include <QDate>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "ValueHandlers/datahandler.h"
#include "ValueHandlers/settingshandler.h"

#include "serialhandlerbase.h"

// This class is in charge of reading in and parsing data from the 405
class SerialDeviceHandler : public SerialHandlerBase
{
    Q_OBJECT
public:
    SerialDeviceHandler();
    ~SerialDeviceHandler();

signals:
    // Emits the dataline string
    void ReceivedDataline(QByteArray dataline);
    void EchoMessage(QByteArray message);

private slots:
    void OnSettingValueChange();
    void CheckForAck();

    void WriteNextMessage() override;

    void OnFinishPostingSQLData(QNetworkReply* reply);

private:
    QQueue<QByteArray> acksToCheck;
    QQueue<QByteArray> acksList;

    QMap<QByteArray, int> missedAcksCounter;

    QTimer ackTimer;

    QNetworkAccessManager* netManager = Q_NULLPTR;

    void ConnectToSettingsObjects();

    // First step in the parsing process
    void ParseReceived() override;

    // Attempts to parse received as a dataline
    void ParseAsDataline();

    // Parses the received message into a settings object
    void ParseSetting(QString marker, QString value);
    // Writes a setting to the device
    void WriteSetting(QString marker);
    void WriteSetting(BaseValueObject* setting);

    void HandleAck();

    void WriteAllSettings();
};

#endif // SERIALDEVICEHANDLER_H
