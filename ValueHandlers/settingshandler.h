#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QDateTime>

class SettingsHandler
{
public:
    static SettingsHandler* GetInstance();

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
};

#endif // SETTINGSHANDLER_H
