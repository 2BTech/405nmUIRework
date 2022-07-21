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

DataHandler::DataHandler()
{

}

int DataHandler::GetSerialNumber()
{
    return serialNumber;
}

void DataHandler::SetSerialNumber(int serialNumber)
{
    this->serialNumber = serialNumber;
}

long DataHandler::GetLogNumber(){
    return logNumber;
}
void DataHandler::SetLogNumber(long val){
    logNumber = val;
}

float DataHandler::GetNO(){
    return no;
}
void DataHandler::SetNO(float val){
    no = val;
}

float DataHandler::GetNOX(){
    return nox;
}
void DataHandler::SetNOX(float val){
    nox = val;
}

float DataHandler::GetNO2(){
    return no2;
}
void DataHandler::SetNO2(float val){
    no2 = val;
}

float DataHandler::GetCellTemp(){
    return cellTemp;
}
void DataHandler::SetCellTemp(float val){
    cellTemp = val;
}

float DataHandler::GetCellPress(){
    return cellPress;
}
void DataHandler::SetCellPress(float val){
    cellPress = val;
}

float DataHandler::GetCellFlow(){
    return cellFlow;
}
void DataHandler::SetCellFlow(float val){
    cellFlow = val;
}

float DataHandler::GetPDV1(){
    return pdv1;
}
void DataHandler::SetPDV1(float val){
    pdv1 = val;
}

float DataHandler::GetPDV2(){
    return pdv2;
}
void DataHandler::SetPDV2(float val){
    pdv2 = val;
}

float DataHandler::GetScrubberTemp(){
    return scrubberTemp;
}

void DataHandler::SetScrubberTemp(float val){
    scrubberTemp = val;
}

float DataHandler::GetOzoneFlow(){
    return ozoneFlow;
}

void DataHandler::SetOzoneFlow(float val){
    ozoneFlow = val;
}

int DataHandler::GetErrorByte(){
    return errorByte;
}
void DataHandler::SetErrorByte(int val){
    errorByte = val;
}

QString DataHandler::GetDate()
{
    return date;
}
void DataHandler::SetDate(QString val)
{
    date = val;
}

QString DataHandler::GetTime()
{
    return time;
}
void DataHandler::SetTime(QString val)
{
    time = val;
}

char DataHandler::GetMode()
{
    return mode;
}
void DataHandler::SetMode(char val)
{
    mode = val;
}

int DataHandler::GetDutyPercent()
{
    return dutyPercent;
}
void DataHandler::SetDutyPercent(int val)
{
    dutyPercent = val;
}

#define DEBUG_RECEIVE_DATA_LINE
void DataHandler::ReceiveDataline(int serialNumber, long logNumber, float no2, float no, float nox, float cellTemp, float cellPress, float cellFlow, float pdv1, float pdv2, float scrubberTemp, float ozoneFlow, int errorByte, QString date, QString time, char mode, int dutyPercent)
{
    if (dutyPercentHist.count() > 100)
    {
#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "History buffers are full";
#endif
        serNumberHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from serial number";
#endif

        logNumberHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from log number";
#endif

        no2Hist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from no2";
#endif

        noHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from no";
#endif

        noxHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from nox";
#endif

        cellTempHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from cell press";
#endif

        cellPressHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from cell flow";
#endif

        cellFlowHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from pdv1";
#endif

        pdv1Hist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from pdv2";
#endif

        pdv2Hist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from scrubber temp";
#endif

        scrubberTempHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from ozone flow";
#endif

        ozoneFlowHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from error byte";
#endif

        errorByteHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from error byte";
#endif

        modeHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from mode";
#endif

        dutyPercentHist.removeFirst();

#ifdef DEBUG_RECEIVE_DATA_LINE
        qDebug() << "Removed from duty percent";
#endif
    }

    this->serialNumber = serialNumber;
    serNumberHist.append(serialNumber);

    this->logNumber = logNumber;
    logNumberHist.append(logNumberHist);

    this->no2 = no2;
    no2Hist.append(no2);

    this->no = no;
    noHist.append(no);

    this->nox = nox;
    noxHist.append(nox);

    this->cellTemp = cellTemp;
    cellTempHist.append(cellTemp);

    this->cellPress = cellPress;
    cellPressHist.append(cellPress);

    this->cellFlow = cellFlow;
    cellFlowHist.append(cellFlow);

    this->pdv1 = pdv1;
    pdv1Hist.append(pdv1);

    this->pdv2 = pdv2;
    pdv2Hist.append(pdv2);

    this->scrubberTemp = scrubberTemp;
    scrubberTempHist.append(scrubberTemp);

    this->ozoneFlow = ozoneFlow;
    ozoneFlowHist.append(ozoneFlow);

    this->errorByte = errorByte;
    errorByteHist.append(errorByte);

    this->date = date;
    this->time = time;

    this->mode = mode;
    modeHist.append(mode);

    this->dutyPercent = dutyPercent;
    dutyPercentHist.append(dutyPercent);
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
