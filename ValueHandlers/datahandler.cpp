#include "datahandler.h"

DataHandler * DataHandler::instance = Q_NULLPTR;
DataHandler * DataHandler::GetInstance()
{
    if(instance == nullptr)
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
