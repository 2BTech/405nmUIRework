#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include <QFile>

// This is a singleton class that tracks all of the data for the device.
class DataHandler : public QObject
{
public:
    static DataHandler* GetInstance();

    int GetSerialNumber();
    void SetSerialNumber(int serialNumber);

    long GetLogNumber();
    void SetLogNumber(long val);

    float GetNO();
    void SetNO(float val);

    float GetNOX();
    void SetNOX(float val);

    float GetNO2();
    void SetNO2(float val);

    float GetCellTemp();
    void SetCellTemp(float val);

    float GetCellPress();
    void SetCellPress(float val);

    float GetCellFlow();
    void SetCellFlow(float val);

    float GetPDV1();
    void SetPDV1(float val);

    float GetPDV2();
    void SetPDV2(float val);

    float GetScrubberTemp();
    void SetScrubberTemp(float val);

    float GetOzoneFlow();
    void SetOzoneFlow(float val);

    int GetErrorByte();
    void SetErrorByte(int val);

    QString GetDate();
    void SetDate(QString val);

    QString GetTime();
    void SetTime(QString val);

    char GetMode();
    void SetMode(char val);

    int GetDutyPercent();
    void SetDutyPercent(int val);

private:
    DataHandler();
    static DataHandler* instance;

    int serialNumber;
    long logNumber = 0;
    float no = 0;
    float nox = 0;
    float no2 = 0;
    float cellTemp = 0;
    float cellPress = 0;
    float cellFlow = 0;
    float pdv1 = 0;
    float pdv2 = 0;
    float ozoneFlow = 0;
    float scrubberTemp = 0;
    int errorByte = 0;
    QString date;
    QString time;
    char mode;
    int dutyPercent;
};

#endif // DATAHANDLER_H
