#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QDateTime>
#include <QObject>
#include <QFile>
#include <QDebug>

#include "GlobalDefinitions.h"

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

    unsigned char GetAvgTime();
    void SetAvgTime(unsigned char val, bool writeSerial , bool writeModbus);

    unsigned char GetAdShort();
    void SetAdShort(unsigned char val, bool writeSerial , bool writeModbus);

    unsigned char GetAdLong();
    void SetAdLong(unsigned char val, bool writeSerial , bool writeModbus);

    unsigned char GetAdDiff();
    void SetAdDiff(unsigned char val, bool writeSerial , bool writeModbus);

    unsigned char GetAdPercent();
    void SetAdPercent(unsigned char val, bool writeSerial, bool writeModbus);

    unsigned char GetBitMask();
    void SetBitMask(unsigned char val, bool writeSerial, bool writeModbus);

    int GetAnalogNO();
    void SetAnalogNO(int val, bool writeSerial, bool writeModbus);

    int GetAnalogNO2();
    void SetAnalogNO2(int val, bool writeSerial, bool writeModbus);

    float GetNOSlope();
    void SetNOSlope(float val, bool writeSerial, bool writeModbus);

    float GetNOZero();
    void SetNOZero(float val, bool writeSerial, bool writeModbus);

    float GetNO2Slope();
    void SetNO2Slope(float val, bool writeSerial, bool writeModbus);

    float GetNO2Zero();
    void SetNO2Zero(float val, bool writeSerial, bool writeModbus);

    float GetOzoneFlowSlope();
    void SetOzoneFlowSlope(float val, bool writeSerial, bool writeModbus);

    float GetCellFlowSlope();
    void SetCellFlowSlope(float val, bool writeSerial, bool writeModbus);

    unsigned char GetMode();
    void SetMode(unsigned char val, bool writeSerial , bool writeModbus);

    int GetSerialNumber();
    void SetSerialNumber(int var, bool writeSerial, bool writeModbus);

    void ReadSettingsFile();
    void WriteSettingsFile();

signals:
    void NewModeSetting();
    void NewNOCalSetting();
    void NewNO2CalSetting();
    void NewFlowCalSetting();
    void NewDateTimeSetting();
    void NewAverageTimeSetting();
    void NewAdaptiveFilterSetting();
    void NewSerialNumber();

    void SetFloatRegisters(int address, float val);
    void SetSingleRegister(int address, short val);
    void SetIntRegister(int address, int value);
    void SetCharArrayRegisters(int address, char * arr, int length);
    void SetNewSettingCoil();

    void WriteFloatSetting(char marker, float val);
    void WriteCharSetting(char marker, unsigned char val);
    //void WriteCharArray(char marker, char * arr, int length);
    void WriteDate(QByteArray date);
    void WriteTime(QByteArray time);
    void WriteIntSetting(char marker, int val);

private:
    static SettingsHandler* instance;
    SettingsHandler();

    unsigned char avgTime = 0;
    QDateTime dateTime;
    unsigned char adShort = 0;
    unsigned char adLong = 0;
    unsigned char adDiff = 0;
    unsigned char adPercent = 0;
    unsigned char bitMask = 0;
    int analogNO = 0;
    int analogNO2 = 0;
    float noSlope = 0;
    float noZero = 0;
    float no2Slope = 0;
    float no2Zero = 0;
    float ozoneFlowSlope = 0;
    float cellFlowSlope = 0;
    unsigned char mode = 0;
    int errorCode = 0;
    unsigned char dateFormat = 0;
    int serialNumber;

    void CreateDefaultSettingsFile();
};

#endif // SETTINGSHANDLER_H
