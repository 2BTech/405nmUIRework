#include "settingshandler.h"

SettingsHandler* SettingsHandler::instance = Q_NULLPTR;
SettingsHandler* SettingsHandler::GetInstance()
{
    if(instance == Q_NULLPTR)
    {
        instance = new SettingsHandler();
    }
    return instance;
}

SettingsHandler::SettingsHandler()
{

}

unsigned char SettingsHandler::GetAvgTime()
{
    return avgTime;
}
void SettingsHandler::SetAvgTime(unsigned char val, bool writeSerial, bool writeModbus)
{
    avgTime = val;

    if(writeSerial)
    {
        emit WriteCharSetting(AvgTimeMarker, avgTime);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(AVG_TIME_REGISTER, avgTime);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewAverageTimeSetting();
}

unsigned char SettingsHandler::GetAdShort()
{
    return adShort;
}
void SettingsHandler::SetAdShort(unsigned char val, bool writeSerial, bool writeModbus)
{
    adShort = val;

    if(writeSerial)
    {
        emit WriteCharSetting(AdaptiveShortMarker, adShort);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(AD_SHORT_REGISTER, adShort);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewAdaptiveFilterSetting();
}

unsigned char SettingsHandler::GetAdLong()
{
    return adLong;
}
void SettingsHandler::SetAdLong(unsigned char val, bool writeSerial, bool writeModbus)
{
    adLong = val;

    if(writeSerial)
    {
         emit WriteCharSetting(AdaptiveLongMarker, adLong);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(AD_LONG_REGISTER, adLong);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewAdaptiveFilterSetting();
}

unsigned char SettingsHandler::GetAdDiff()
{
    return adDiff;
}
void SettingsHandler::SetAdDiff(unsigned char val, bool writeSerial, bool writeModbus){
    adDiff = val;

    if(writeSerial)
    {
        emit WriteCharSetting(AdaptiveDifferenceMarker, adDiff);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(AD_DIFF_REGISTER, adDiff);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewAdaptiveFilterSetting();
}

unsigned char SettingsHandler::GetAdPercent()
{
    return adPercent;
}
void SettingsHandler::SetAdPercent(unsigned char val, bool writeSerial, bool writeModbus){
    adPercent = val;

    if(writeSerial)
    {
        emit WriteCharSetting(AdaptivePercentMarker, adPercent);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(AD_PERCENT_REGISTER, adPercent);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewAdaptiveFilterSetting();
}

unsigned char SettingsHandler::GetBitMask()
{
    return bitMask;
}
void SettingsHandler::SetBitMask(unsigned char val, bool writeSerial, bool writeModbus){
    bitMask = val;

    if(writeSerial)
    {
        emit WriteCharSetting(BitMaskMarker, bitMask);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(BITMASK_REGISTER, bitMask);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
}

int SettingsHandler::GetAnalogNO()
{
    return analogNO;
}
void SettingsHandler::SetAnalogNO(int val, bool writeSerial, bool writeModbus){
    analogNO = val;

    if(writeSerial)
    {
        emit WriteIntSetting(NOAnalogMarker, analogNO);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(NO_ANALOG_REGISTER, static_cast<short>(analogNO));
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewNOCalSetting();
}

int SettingsHandler::GetAnalogNO2()
{
    return analogNO2;
}
void SettingsHandler::SetAnalogNO2(int val, bool writeSerial, bool writeModbus)
{
    analogNO2 = val;

    if(writeSerial)
    {
        emit WriteIntSetting(NO2AnalogMarker, analogNO2);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(NO2_ANALOG_REGISTER, static_cast<short>(analogNO2));
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewNO2CalSetting();
}

float SettingsHandler::GetNOSlope()
{
    return noSlope;
}
void SettingsHandler::SetNOSlope(float val, bool writeSerial, bool writeModbus)
{
    noSlope = val;

    if(writeSerial)
    {
        emit WriteFloatSetting(NOSlopeMarker, noSlope);
    }

    if(writeModbus)
    {
        emit SetFloatRegisters(NO_SLOPE_REGISTER, noSlope);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewNOCalSetting();
}

float SettingsHandler::GetNOZero()
{
    return noZero;
}
void SettingsHandler::SetNOZero(float val, bool writeSerial, bool writeModbus)
{
    noZero = val;

    if(writeSerial)
    {
        emit WriteFloatSetting(NOZeroMarker, noZero);
    }

    if(writeModbus)
    {
        emit SetFloatRegisters(NO_ZERO_REGISTER, noZero);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewNOCalSetting();
}

float SettingsHandler::GetNO2Slope()
{
    return no2Slope;
}
void SettingsHandler::SetNO2Slope(float val, bool writeSerial, bool writeModbus)
{
    no2Slope = val;

    if(writeSerial)
    {
        emit WriteFloatSetting(NO2SlopeMarker, no2Slope);
    }

    if(writeModbus)
    {
        emit SetFloatRegisters(NO2_SLOPE_REGISTER, no2Slope);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewNO2CalSetting();
}

float SettingsHandler::GetNO2Zero()
{
    return no2Zero;
}
void SettingsHandler::SetNO2Zero(float val, bool writeSerial, bool writeModbus)
{
    no2Zero = val;

    if(writeSerial)
    {
        emit WriteFloatSetting(NO2ZeroMarker, no2Zero);
    }

    if(writeModbus)
    {
        emit SetFloatRegisters(NO2_ZERO_REGISTER, no2Zero);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewNO2CalSetting();
}

float SettingsHandler::GetOzoneFlowSlope()
{
    return ozoneFlowSlope;
}
void SettingsHandler::SetOzoneFlowSlope(float val, bool writeSerial, bool writeModbus)
{
    ozoneFlowSlope = val;

    if(writeSerial)
    {
        emit WriteFloatSetting(OzoneFlowSlopeMarker, ozoneFlowSlope);
    }

    if(writeModbus)
    {
        emit SetFloatRegisters(OZONE_FLOW_SLOPE_REGISTER, ozoneFlowSlope);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewFlowCalSetting();
}

float SettingsHandler::GetCellFlowSlope()
{
    return cellFlowSlope;
}
void SettingsHandler::SetCellFlowSlope(float val, bool writeSerial, bool writeModbus){
    cellFlowSlope = val;

    if(writeSerial)
    {
        emit WriteFloatSetting(CellFlowSlopeMarker, cellFlowSlope);
    }

    if(writeModbus)
    {
        emit SetFloatRegisters(CELL_FLOW_SLOPE_REGISTER, cellFlowSlope);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewFlowCalSetting();
}

unsigned char SettingsHandler::GetMode()
{
    return mode;
}
void SettingsHandler::SetMode(unsigned char val, bool writeSerial, bool writeModbus)
{
    mode = val;

    if(writeSerial)
    {
        emit WriteCharSetting(ModeMarker, mode);
    }

    if(writeModbus)
    {
        emit SetSingleRegister(MODE_REGISTER, mode);
        emit SetNewSettingCoil();
    }

    WriteSettingsFile();
    emit NewModeSetting();
}

int SettingsHandler::GetSerialNumber()
{
    return serialNumber;
}

void SettingsHandler::SetSerialNumber(int var, bool writeSerial, bool writeModbus)
{
    serialNumber = var;

    if(writeSerial)
    {
        emit WriteIntSetting(SERIAL_NUM_MARKER, var);
    }

    if(writeModbus)
    {
        emit SetIntRegister(OZONE_FLOW_SLOPE_REGISTER, ozoneFlowSlope);
        emit SetNewSettingCoil();
    }

    emit NewSerialNumber();
    if(writeModbus && writeSerial)
    {
        WriteSettingsFile();
    }
}

void SettingsHandler::ReadSettingsFile()
{
    QFile file;

    int tempInt = 0;
    float tempFloat = 0;
    unsigned char tempUChar = 0;

    file.setFileName(QString(WORKING_DIR).append("405nmSettings.txt"));
    if(file.exists() == false)
    {
        file.setFileName("405nmSettings.txt");
    }

    if(file.exists() == true)
    {
        if(file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                line = line.remove('\n').remove('\r');

                if(line.length() < 2)
                    continue;

                switch (line.at(0).toLatin1())
                {
                //Avg Time
                case 'A':
                    tempUChar = static_cast<unsigned char>(atoi(line.remove(0, 1).toLatin1()));
                    if(tempUChar <= 4)
                    {
                        SetAvgTime(tempUChar, false, true);
                        //qDebug() << "Reading avg time in as " << QString::number(avgTime);
                    }
                    else
                    {
                        qDebug() << "Read in bad Avg Time: " << QString::number(tempUChar);
                        SetAvgTime(1, false, true);
                    }
                    break;

                //Bit mask
                case 'B':
                    SetBitMask(static_cast<unsigned char>(atoi(line.remove(0, 1).toLatin1())), false, true);
                    //qDebug() << "Reading bitmask in as " << QString::number(bitMask);
                    break;

                //Ad Short
                case 'C':
                    tempUChar = static_cast<unsigned char>(atoi(line.remove(0, 1).toLatin1()));
                    if(tempUChar >= 1 && tempUChar <= 5)
                    {
                        SetAdShort(tempUChar, false, true);
                        //qDebug() << "Reading ad short in as " << QString::number(adShort);
                    }
                    else
                    {
                        qDebug() << "Read in bad Ad Short: " << QString::number(tempUChar);
                        SetAdShort(1, false, true);
                    }
                    break;

                //Ad Long
                case 'D':
                    tempUChar = static_cast<unsigned char>(atoi(line.remove(0, 1).toLatin1()));
                    if(tempUChar >= 6 && tempUChar <= 50)
                    {
                        SetAdLong(tempUChar, false, true);
                        //qDebug() << "Reading ad long in as " << QString::number(adLong);
                    }
                    else
                    {
                        qDebug() << "Read in bad Ad Long: " << QString::number(tempUChar);
                        SetAdLong(6, false, true);
                    }
                    break;

                //Ad Diff
                case 'E':
                    tempUChar = static_cast<unsigned char>(atoi(line.remove(0, 1).toLatin1()));
                    if(tempUChar <= 50)
                    {
                        SetAdDiff(tempUChar, false, true);
                        //qDebug() << "Reading ad diff in as " << QString::number(adDiff);
                    }
                    else
                    {
                        qDebug() << "Read in bad Ad Diff: " << QString::number(tempUChar);
                    }
                    break;

                //Ad Per
                case 'F':
                    tempUChar = static_cast<unsigned char>(atoi(line.remove(0, 1).toLatin1()));
                    if(tempUChar <= 10)
                    {
                        SetAdPercent(tempUChar, false, true);
                        //qDebug() << "Reading ad percent in as " << QString::number(adPercent);
                    }
                    else
                    {
                        qDebug() << "Read in bad Ad Per: " << QString::number(tempUChar);
                    }
                    break;

                //NO Slope
                case 'G':
                    tempFloat = static_cast<float>(atof(line.remove(0, 1).toLatin1()));
                    if(tempFloat >= 0.5f && tempFloat <= 1.5f)
                    {
                        SetNOSlope(tempFloat, false, true);
                        //qDebug() << "Reading NO slope in as " << QString::number(static_cast<double>(noSlope));
                    }
                    else
                    {
                        qDebug() << "Read in bad NO Slope: " << tempFloat;
                        SetNOSlope(1, false, true);
                    }
                    break;

                //NO Zero
                case 'H':
                    tempFloat = static_cast<float>(atof(line.remove(0, 1).toLatin1()));
                    if(tempFloat >= -50 && tempFloat <= 51)
                    {
                        SetNOZero(tempFloat, false, true);
                        //qDebug() << "Reading NO zero in as " << QString::number(static_cast<double>(noZero));
                    }
                    else
                    {
                        qDebug() << "Read in bad NO Zero: " << tempFloat;
                        SetNOZero(0, false, true);
                    }
                    break;

                //NO Analog
                case 'I':
                    tempInt = atoi(line.remove(0, 1).toLatin1()) & 0xFFFF;
                    if(tempInt >= 10 && tempInt <= 1000)
                    {
                        SetAnalogNO(tempInt, false, true);
                        //qDebug() << "Reading analog no in as " << QString::number(analogNO);
                    }
                    else
                    {
                        qDebug() << "Read in bad NO Analog: " << tempInt;
                        SetAnalogNO(500, false, true);
                    }
                    break;

                //NO2 Slope
                case 'J':
                    tempFloat = static_cast<float>(atof(line.remove(0, 1).toLatin1()));
                    if(tempFloat >= 0.5f && tempFloat <= 1.5f)
                    {
                        SetNO2Slope(tempFloat, false, true);
                        //qDebug() << "Reading NO2 slope in as " << QString::number(static_cast<double>(no2Slope));
                    }
                    else
                    {
                        qDebug() << "Read in bad NO2 slope: " << tempFloat;
                        SetNO2Slope(1, false, true);
                    }
                    break;

                //NO2 Zero
                case 'K':
                    tempFloat = static_cast<float>(atof(line.remove(0, 1).toLatin1()));
                    if(tempFloat >= -50 && tempFloat <= 51)
                    {
                        SetNO2Zero(tempFloat, false, true);
                        //qDebug() << "Reading NO2 zero in as " << QString::number(static_cast<double>(no2Zero));
                    }
                    else
                    {
                        qDebug() << "Read in bad NO2 Zero: " << tempFloat;
                     }
                    break;

                //NO2 Analog
                case 'L':
                    tempInt = atoi(line.remove(0, 1).toLatin1()) & 0xFFFF;
                    if(tempInt >= 1 && tempInt <= 1000)
                    {
                        SetAnalogNO2(tempInt, false, true);
                        //qDebug() << "Reading analog no2 in as " << QString::number(analogNO2);
                    }
                    else
                    {
                        qDebug() << "Read in bad NO2 analog: " << tempInt;
                        SetAnalogNO2(500, false, true);
                    }
                    break;

                //Ozone Flow Slope
                case 'M':
                    tempFloat = static_cast<float>(atof(line.remove(0, 1).toLatin1()));
                    if(tempFloat >= 0.2f && tempFloat <= 2.0f)
                    {
                        SetOzoneFlowSlope(tempFloat, false, true);
                        //qDebug() << "Reading flow slope in as " << QString::number(static_cast<double>(ozoneFlowSlope));
                    }
                    else
                    {
                        qDebug() << "Read in bad Ozone Flow Slope: " << tempFloat;
                        SetOzoneFlowSlope(1, false, true);
                    }
                    break;

                //Cell flow slope
                case 'N':
                    tempFloat = static_cast<float>(atof(line.remove(0, 1).toLatin1()));
                    if(tempFloat >= 0.2f && tempFloat <= 2.0f)
                    {
                        SetCellFlowSlope(tempFloat, false, true);
                        //qDebug() << "Reading flow zero in as " << QString::number(static_cast<double>(cellFlowSlope));
                    }
                    else
                    {
                        qDebug() << "Read in bad Cell Flow Slope: " << tempFloat;
                        SetCellFlowSlope(1, false, true);
                    }
                    break;

                //Mode
                case 'O':
                    tempUChar = static_cast<unsigned char>(atoi(line.remove(0, 1).toLatin1()));
                    if(tempUChar >= 1 && tempUChar <= 4)
                    {
                        SetMode(tempUChar, false, true);
                        //qDebug() << "Reading " << static_cast<int>(mode) << " for mode";
                    }
                    else
                    {
                        qDebug() << "Read in bad mode: " << tempUChar;
                        SetMode(1, false, true);
                    }
                    break;

                // Serial Number
                case 'R':
                    tempInt = tempFloat = atoi(line.remove(0, 1).toLatin1());
                    SetSerialNumber(tempInt, false, true);
                    break;

                default:
                    qDebug() << "Read settings file hit default case: " << line.at(0).toLatin1();
                }
            }
        }
        else
        {
            CreateDefaultSettingsFile();
        }
    }
    else
    {
        CreateDefaultSettingsFile();
    }
}

void SettingsHandler::CreateDefaultSettingsFile()
{
    QFile file(QString(WORKING_DIR).append("405nmSettings.txt"));

    if(file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Creating default settings file";
        file.setFileName("405nmSettings.txt");
        file.open(QIODevice::WriteOnly);
        file.write("A1\n");     //Avg state
        file.write("B0\n");     //Bitmask
        file.write("C1\n");     //Adaptive short length
        file.write("D10\n");    //Adaptive long length
        file.write("E10\n");    //Adaptive difference
        file.write("F10\n");    //Adaptive percent
        file.write("G1\n");     //NO slope
        file.write("H0\n");     //NO zero
        file.write("I500\n");   //NO analog
        file.write("J1\n");     //NO2 slope
        file.write("K0\n");     //NO2 zero
        file.write("L500\n");   //NO2 analog
        file.write("M1\n");     //Ozone Flow slope
        file.write("N1\n");     //Cell Flow Slope
        file.write("O1\n");     //Mode
        file.write("P0\n");     //Date format
        file.write("R-1\n");    //Serial number
        file.close();
    }
    else
    {
        qDebug() << "ERROR: Failed to create default settings file";
    }
}

void SettingsHandler::WriteSettingsFile()
{
    QFile settingsFile;

    settingsFile.setFileName(QString(WORKING_DIR).append("405nmSettings.txt"));
    if(!settingsFile.open(QIODevice::WriteOnly))
    {
        settingsFile.setFileName("405nmSettings.txt");
        if(!settingsFile.open(QIODevice::WriteOnly))
        {
            qDebug() << "ERROR: Failed to open settings file for writing";
            return;
        }
    }

    if(settingsFile.isOpen())
    {
        char arr[20] = {0, };

        sprintf(arr, "A%i\n", static_cast<int>(GetAvgTime()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "B%i\n", static_cast<int>(GetBitMask()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "C%i\n", static_cast<int>(GetAdShort()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "D%i\n", static_cast<int>(GetAdLong()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "E%i\n", static_cast<int>(GetAdDiff()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "F%i\n", static_cast<int>(GetAdPercent()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "G%1.2f\n", static_cast<double>(GetNOSlope()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "H%1.1f\n", static_cast<double>(GetNOZero()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "I%i\n", GetAnalogNO());
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "J%1.2f\n", static_cast<double>(GetNO2Slope()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "K%1.1f\n", static_cast<double>(GetNO2Zero()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "L%i\n", GetAnalogNO2());
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "M%1.2f\n", static_cast<double>(GetOzoneFlowSlope()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "N%1.2f\n", static_cast<double>(GetCellFlowSlope()));
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "O%i\n", GetMode());
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "P%i\n", dateFormat);
        settingsFile.write(arr);
        memset(arr, 0, 20);

        sprintf(arr, "R%i\n", serialNumber);
        settingsFile.write(arr);
    }

    settingsFile.close();
}
