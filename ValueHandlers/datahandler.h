#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include <QFile>
#include <QDate>
#include <QDebug>

#include "GlobalDefinitions.h"
#include "ValueHandlers/valuehandlerbase.h"

// This is a singleton class that tracks all of the data for the device.
class DataHandler : public ValueHandlerBase
{
    Q_OBJECT
public:
    static DataHandler* GetInstance();

    ValueObject<int>* GetSerialNumber();
    ValueObject<int>* GetLogNumber();
    ValueObject<float>* GetNO();
    ValueObject<float>* GetNO2();
    ValueObject<float>* GetNOx();
    ValueObject<float>* GetCellTemp();
    ValueObject<float>* GetCellPress();
    ValueObject<float>* GetCellFlow();
    ValueObject<float>* GetPDV1();
    ValueObject<float>* GetPDV2();
    ValueObject<float>* GetScrubberTemp();
    ValueObject<float>* GetOzoneFlow();
    ValueObject<uchar>* GetMode();
    ValueObject<int>* GetDutyPercent();
    ValueObject<QString>* GetDate();
    ValueObject<QString>* GetTime();
    ValueObject<int>* GetErrorByte();

    void ReceiveDataline(int serialNumber, long logNumber, float no2, float no, float nox, float cellTemp, float cellPress, float cellFlow, float pdv1, float pdv2, float scrubberTemp, float ozoneFlow, int errorByte, QString date, QString time, char mode, int dutyPercent);

    void LogDataline(QByteArray dataline);

signals:
    void ParsedDataLine();

private:
    DataHandler();
    ~DataHandler();
    static DataHandler* instance;

    ValueObject<int>* serialNumber = Q_NULLPTR;
    ValueObject<int>* logNumber = Q_NULLPTR;
    ValueObject<float>* no = Q_NULLPTR;
    ValueObject<float>* no2 = Q_NULLPTR;
    ValueObject<float>* nox = Q_NULLPTR;
    ValueObject<float>* cellTemp = Q_NULLPTR;
    ValueObject<float>* cellPress = Q_NULLPTR;
    ValueObject<float>* cellFlow = Q_NULLPTR;
    ValueObject<float>* pdv1 = Q_NULLPTR;
    ValueObject<float>* pdv2 = Q_NULLPTR;
    ValueObject<float>* scrubberTemp = Q_NULLPTR;
    ValueObject<float>* ozoneFlow = Q_NULLPTR;
    ValueObject<int>* errorByte = Q_NULLPTR;
    ValueObject<QString>* date = Q_NULLPTR;
    ValueObject<QString>* time = Q_NULLPTR;
    ValueObject<uchar>* mode = Q_NULLPTR;
    ValueObject<int>* dutyPercent = Q_NULLPTR;

    void BuildObjects() override;
};

#endif // DATAHANDLER_H
