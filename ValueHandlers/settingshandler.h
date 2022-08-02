#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QFile>

#include "ValueHandlers/valuehandlerbase.h"

#define ENABLE_MODBUS

#define SerialComError 1
#define NoSettingsFile 2
#define FailedToConnectToSerial 3
#define MissedPreviousLog 4
#define DetectedCrash 5
#define MissedLog 6
#define SettingsFile 7

//Signaling characters for serial comm
#define AvgTimeMarker 'A'
#define BitMaskMarker 'B'
#define AdaptiveShortMarker 'C'
#define AdaptiveLongMarker 'D'
#define AdaptiveDifferenceMarker 'E'
#define AdaptivePercentMarker 'F'
#define NOSlopeMarker 'G'
#define NOZeroMarker 'H'
#define NOAnalogMarker 'I'
#define NO2SlopeMarker 'J'
#define NO2ZeroMarker 'K'
#define NO2AnalogMarker 'L'
#define OzoneFlowSlopeMarker 'M'
#define CellFlowSlopeMarker 'N'
#define ModeMarker 'O'
#define DateMarker 'P'
#define TimeMarker 'Q'
#define SERIAL_NUM_MARKER 'R'

class SettingsHandler : public ValueHandlerBase
{
    Q_OBJECT
public:
    static SettingsHandler* GetInstance();

    void ReadSettingsFile();
    void WriteSettingsFile();

    bool AddSettingObject(BaseValueObject* val, bool updateSettingsFileOnValChange);

private slots:
    void OnValueChanged();

private:
    static SettingsHandler* instance;
    SettingsHandler();
    ~SettingsHandler();

    ValueObject<uchar>* avgTime = Q_NULLPTR;

    ValueObject<uchar>* adShort = Q_NULLPTR;
    ValueObject<uchar>* adLong = Q_NULLPTR;
    ValueObject<uchar>* adDiff = Q_NULLPTR;
    ValueObject<uchar>* adPercent = Q_NULLPTR;

    ValueObject<int>* analogNO = Q_NULLPTR;
    ValueObject<int>* analogNO2 = Q_NULLPTR;

    ValueObject<float>* noSlope = Q_NULLPTR;
    ValueObject<float>* noZero = Q_NULLPTR;

    ValueObject<float>* no2Slope = Q_NULLPTR;
    ValueObject<float>* no2Zero = Q_NULLPTR;

    ValueObject<float>* ozoneFlowSlope = Q_NULLPTR;
    ValueObject<float>* cellFlowSlope = Q_NULLPTR;

    ValueObject<uchar>* mode = Q_NULLPTR;

    ValueObject<int>* errorCode = Q_NULLPTR;

    ValueObject<uchar>* dateFormat = Q_NULLPTR;

    ValueObject<int>* serialNumber = Q_NULLPTR;

    ValueObject<QString>* date = Q_NULLPTR;
    ValueObject<QString>* time = Q_NULLPTR;

    //unsigned char avgTime = 0;
    //QDateTime dateTime;

    //QHash<QString, BaseValueObject*> markerSettingMap;

    QHash<QString,QString> unknownSettinsMap;

    void CreateDefaultSettingsFile();

    // Build all of the setting value objects
    void BuildObjects() override;
};

#endif // SETTINGSHANDLER_H
