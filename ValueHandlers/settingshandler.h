#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QDateTime>
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QMap>

#include "GlobalDefinitions.h"
#include "ValueHandlers/ValueObjects/valueobjects.h"

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

#define AVG_TIME_REGISTER 37 //unsinged char, 1 short
#define DATE_REGISTER 38 //char[7], 7 shorts
#define TIME_REGISTER 45 //char[7], 7 shorts
#define AD_SHORT_REGISTER 52 //unsigned char, 1 short
#define AD_LONG_REGISTER 53 //unsigned char, 1 short
#define AD_PERCENT_REGISTER 54 //unsigned char, 1 short
#define AD_DIFF_REGISTER 55 //unsigned char, 1 short
#define BITMASK_REGISTER 56 //Unsinged char, 1 short
#define NO_ANALOG_REGISTER 57
#define NO2_ANALOG_REGISTER 58
#define NO_SLOPE_REGISTER 59 //float
#define NO_ZERO_REGISTER 61
#define NO2_SLOPE_REGISTER 63
#define NO2_ZERO_REGISTER 65
#define OZONE_FLOW_SLOPE_REGISTER 67
#define CELL_FLOW_SLOPE_REGISTER 69
#define MODE_REGISTER 71
#define ERROR_CODE_REGISTER 72
#define DATE_FORMAT_REGISTER 73
#define SETTING_SERIAL_NUMBER_REGISTER 74

class SettingsHandler : public QObject
{
    Q_OBJECT
public:
    static SettingsHandler* GetInstance();

    BaseValueObject* GetSetting(QString marker);

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
    QDateTime dateTime;

    QHash<QString, BaseValueObject*> markerSettingMap;

    QHash<QString,QString> unknownSettinsMap;

    void CreateDefaultSettingsFile();

    // Build all of the setting value objects
    void BuildObjects();
};

#endif // SETTINGSHANDLER_H
