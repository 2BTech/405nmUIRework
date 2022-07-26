#include "datahandler.h"

DataHandler * DataHandler::instance = Q_NULLPTR;
DataHandler * DataHandler::GetInstance()
{
    if(instance == Q_NULLPTR)
    {
        instance = new DataHandler();
    }
    return instance;
}

DataHandler::DataHandler() : ValueHandlerBase()
{
    BuildObjects();

    QDir dir(QString(WORKING_DIR).append("datafiles/"));
    if (!dir.exists())
    {
        dir.mkpath(QString(WORKING_DIR).append("datafiles/"));
    }
}

DataHandler::~DataHandler()
{
    if (serialNumber)
    {
        serialNumber->deleteLater();
        logNumber->deleteLater();
        no->deleteLater();
        no2->deleteLater();
        nox->deleteLater();
        cellTemp->deleteLater();
        cellPress->deleteLater();
        cellFlow->deleteLater();
        pdv1->deleteLater();
        pdv2->deleteLater();
        scrubberTemp->deleteLater();
        ozoneFlow->deleteLater();
        errorByte->deleteLater();
        date->deleteLater();
        time->deleteLater();
        mode->deleteLater();
        dutyPercent->deleteLater();
    }
}

ValueObject<int>* DataHandler::GetSerialNumber()
{
    return serialNumber;
}

ValueObject<int>* DataHandler::GetLogNumber()
{
    return logNumber;
}

ValueObject<float>* DataHandler::GetNO()
{
    return no;
}

ValueObject<float>* DataHandler::GetNO2()
{
    return no2;
}

ValueObject<float>* DataHandler::GetNOx()
{
    return nox;
}

ValueObject<float>* DataHandler::GetCellTemp()
{
    return cellTemp;
}

ValueObject<float>* DataHandler::GetCellPress()
{
    return cellPress;
}

ValueObject<float>* DataHandler::GetCellFlow()
{
    return cellFlow;
}

ValueObject<float>* DataHandler::GetPDV1()
{
    return pdv1;
}

ValueObject<float>* DataHandler::GetPDV2()
{
    return pdv2;
}

ValueObject<float>* DataHandler::GetScrubberTemp()
{
    return scrubberTemp;
}

ValueObject<float>* DataHandler::GetOzoneFlow()
{
    return ozoneFlow;
}

ValueObject<uchar>* DataHandler::GetMode()
{
    return mode;
}

ValueObject<int>* DataHandler::GetDutyPercent()
{
    return dutyPercent;
}

ValueObject<QString>* DataHandler::GetDate()
{
    return date;
}

ValueObject<QString>* DataHandler::GetTime()
{
    return time;
}

ValueObject<int>* DataHandler::GetErrorByte()
{
    return errorByte;
}

//#define DEBUG_RECEIVE_DATA_LINE
void DataHandler::ReceiveDataline(int serialNumber, long logNumber, float no2, float no, float nox, float cellTemp, float cellPress, float cellFlow, float pdv1, float pdv2, float scrubberTemp, float ozoneFlow, int errorByte, QString date, QString time, char mode, int dutyPercent)
{
    this->serialNumber->setValue(serialNumber);
    this->logNumber->setValue(logNumber);
    this->no2->setValue(no2);
    this->no->setValue(no);
    this->nox->setValue(nox);
    this->cellTemp->setValue(cellTemp);
    this->cellPress->setValue(cellPress);
    this->cellFlow->setValue(cellFlow);
    this->pdv1->setValue(pdv1);
    this->pdv2->setValue(pdv2);
    this->scrubberTemp->setValue(scrubberTemp);
    this->ozoneFlow->setValue(ozoneFlow);
    this->errorByte->setValue(errorByte);
    this->date->setValue(date);
    this->time->setValue(time);
    this->mode->setValue(mode);
    this->dutyPercent->setValue(dutyPercent);

    emit ParsedDataLine();
}

void DataHandler::LogDataline(QByteArray dataline)
{
    if (!dataline.endsWith('\n'))
    {
        dataline.append('\n');
    }

    QFile logFile(QString(WORKING_DIR).append("datafiles/405nm_").append(QDate::currentDate().toString("yyyy_MM_dd.csv")));

    bool exits = logFile.exists();

    // Use write only to create file if necesary, or open using append
    if ( (!exits && logFile.open(QIODevice::Append)) || logFile.open(QIODevice::Append))
    {
        if (!exits)
        {
            logFile.write("SerialNumber,LogNumber,NO2,NO,NOx,CellTemp,CellPress,CellFlow,PDV1,PDV2,ScrubberTemp,OzoneFlow,ErrorByte,Date,Time,Mode,DutyPercent\n");
        }
        logFile.write(dataline);

        logFile.close();
    }
    else
    {
        qDebug() << "ERROR: Failed to open log file: " << logFile.fileName() << " : " << logFile.errorString();
    }
}

void DataHandler::BuildObjects()
{
    serialNumber = new ValueObject<int>("Serial Number", "_D1");
    AddValue(serialNumber);

    logNumber = new ValueObject<int>("Log Number", "_D2");
    AddValue(logNumber);

    no = new ValueObject<float>("NO", "_D3");
    no->SetNumSigFigs(1);
    AddValue(no);

    no2 = new ValueObject<float>("NO2", "_D4");
    no2->SetNumSigFigs(1);
    AddValue(no2);

    nox = new ValueObject<float>("NOx", "_D5");
    nox->SetNumSigFigs(1);
    AddValue(nox);

    cellTemp = new ValueObject<float>("Cell Temp", "_D6");
    cellTemp->SetNumSigFigs(1);
    AddValue(cellTemp);

    cellPress = new ValueObject<float>("Cell Press", "_D7");
    cellPress->SetNumSigFigs(1);
    AddValue(cellPress);

    cellFlow = new ValueObject<float>("Cell Flow", "_D8");
    cellFlow->SetNumSigFigs(1);
    AddValue(cellFlow);

    pdv1 = new ValueObject<float>("PDV1", "_D9");
    pdv1->SetNumSigFigs(6);
    AddValue(pdv1);

    pdv2 = new ValueObject<float>("PDV2", "_D10");
    pdv2->SetNumSigFigs(6);
    AddValue(pdv2);

    scrubberTemp = new ValueObject<float>("Scrubber Temp", "_D11", "C");
    scrubberTemp->SetNumSigFigs(1);
    AddValue(scrubberTemp);

    ozoneFlow = new ValueObject<float>("Ozone Flow", "_D12");
    ozoneFlow->SetNumSigFigs(1);
    AddValue(ozoneFlow);

    errorByte = new ValueObject<int>("Error Byte", "_D13");
    AddValue(errorByte);

    date = new ValueObject<QString>("Date", "D14");
    AddValue(date);

    time = new ValueObject<QString>("Time", "_D15");
    AddValue(time);

    mode = new ValueObject<uchar>("Mode", "_D16");
    AddValue(mode);

    dutyPercent = new ValueObject<int>("Duty Percent", "_D17");
    AddValue(dutyPercent);
}

QString DataHandler::GetErrorString()
{
    switch (errorByte->getValue())
    {
    //No Error
    case 0x00:
        return "No Errors";

    //Cell Voltage out of range
    case 0x02:
        return "Cell voltage out of range (< 0.1V or 2.4V)";

    //Cell flow out of range
    case 0x04:
        return "Cell flow out of range (< 1200 or > 1600)";

    //Scrubber temp out of range
    case 0x08:
        return "Scrubber Temp out of range (< 110 or > 113)";

    //Ozone generator volatage out of range
    case 0x20:
        return "Ozone generator voltage out of range (< 0.1V or > 2.4V)";

    //Ozone flow out of range
    case 0x40:
        return "Ozone flow out of range (< 30 or > 110)";

    //Pressure control out of range
    case 0x80:
        return "Pressure control out of range by > 1 mbar";

    default:
        break;
    }

    QStringList lst;
    QString toReturn;

    //Get error sections
    if(errorByte->getValue() & 0x02)
    {
        lst.append("Cell Voltage");
    }

    if(errorByte->getValue() & 0x04)
    {
        lst.append("Cell Flow");
    }

    if(errorByte->getValue() & 0x08)
    {
        lst.append("Scrubber Temp");
    }

    if(errorByte->getValue() & 0x20)
    {
        lst.append("Ozone Generator Voltage");
    }

    if(errorByte->getValue() & 0x40)
    {
        lst.append("Ozone Flow");
    }

    if(errorByte->getValue() & 0x80)
    {
        lst.append("Pressure Control");
    }

    //Combine sections
    for(int i = 0; i < lst.count(); i++)
    {
        //Check if not last item
        if(i + 1 < lst.count())
        {
            toReturn.append(lst.at(i)).append(", ");
        }
        else
        {
            //Remove seperator
            toReturn.remove(toReturn.count() - 2, 2);
            toReturn.append(" and ").append(lst.at(i));
        }
    }
    return toReturn;
}
