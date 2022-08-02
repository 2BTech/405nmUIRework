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

SettingsHandler::SettingsHandler() : ValueHandlerBase()
{
    BuildObjects();
}

SettingsHandler::~SettingsHandler()
{
    avgTime->deleteLater();

    adShort->deleteLater();
    adLong->deleteLater();
    adDiff->deleteLater();
    adPercent->deleteLater();

    analogNO->deleteLater();
    analogNO2->deleteLater();

    noSlope->deleteLater();
    noZero->deleteLater();

    no2Slope->deleteLater();
    no2Zero->deleteLater();

    ozoneFlowSlope->deleteLater();
    cellFlowSlope->deleteLater();

    mode->deleteLater();

    errorCode->deleteLater();

    dateFormat->deleteLater();

    serialNumber->deleteLater();

    avgTime = adShort = adLong = adDiff = adPercent = mode = dateFormat = Q_NULLPTR;

    analogNO = analogNO2 = errorCode = serialNumber = Q_NULLPTR;

    noSlope = noZero = no2Slope = no2Zero = ozoneFlowSlope = cellFlowSlope = Q_NULLPTR;
}

void SettingsHandler::OnValueChanged()
{
    WriteSettingsFile();
}

void SettingsHandler::ReadSettingsFile()
{
    QFile file;

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

                // Check if new or old style save
                if (line.contains(','))
                {
                    // New Style
                    QStringList split = line.split(',');
                    if (split.count() == 2)
                    {
                        if (markerValueMap.contains(split[0]))
                        {
                            markerValueMap[split[0]]->setValue_str(split[1]);
                        }
                        else
                        {
                            //qDebug() << "Unknown setting marker: " << split[0];
                            unknownSettinsMap[split[0]] = split[1];
                        }
                    }
                }
                else
                {
                    if (markerValueMap.contains(QString().append(line[0])))
                    {
                        markerValueMap[QString().append(line[0])]->setValue_str(line.remove(0, 1));
                    }
                    else
                    {
                        //qDebug() << "Unknown setting marker: " << line[0];
                        unknownSettinsMap[QString(line[0])] = line.remove(0, 1);
                    }
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
        file.write("A,1\n");     //Avg state
        file.write("B,0\n");     //Bitmask
        file.write("C,1\n");     //Adaptive short length
        file.write("D,10\n");    //Adaptive long length
        file.write("E,10\n");    //Adaptive difference
        file.write("F,10\n");    //Adaptive percent
        file.write("G,1\n");     //NO slope
        file.write("H,0\n");     //NO zero
        file.write("I,500\n");   //NO analog
        file.write("J,1\n");     //NO2 slope
        file.write("K,0\n");     //NO2 zero
        file.write("L,500\n");   //NO2 analog
        file.write("M,1\n");     //Ozone Flow slope
        file.write("N,1\n");     //Cell Flow Slope
        file.write("O,1\n");     //Mode
        file.write("P,0\n");     //Date format
        file.write("R,-1\n");    //Serial number
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
        const QStringList keys = markerValueMap.keys();
        for (const QString &str : keys)
        {
            settingsFile.write(str.toLatin1());
            settingsFile.write(",");
            settingsFile.write(markerValueMap[str]->ToString().toLatin1());
            settingsFile.write("\n");
        }
        const QStringList unknownKeys = unknownSettinsMap.keys();
        for (const QString &str : unknownKeys)
        {
            settingsFile.write(str.toLatin1());
            settingsFile.write(",");
            settingsFile.write(unknownSettinsMap[str].toLatin1());
            settingsFile.write("\n");
        }
    }

    settingsFile.close();
}

void SettingsHandler::BuildObjects()
{
    avgTime = new ValueObject<uchar>("Averaging Time", "A", "");
    AddSettingObject(avgTime, true);

    adShort = new ValueObject<uchar>("Adaptive Short", "C", "");
    AddSettingObject(adShort, true);

    adLong = new ValueObject<uchar>("Adaptive Long", "D", "");
    AddSettingObject(adLong, true);

    adDiff = new ValueObject<uchar>("Adaptive Difference", "E", "");
    AddSettingObject(adDiff, true);

    adPercent = new ValueObject<uchar>("Adaptive Percent", "F", "");
    AddSettingObject(adPercent, true);

    analogNO = new ValueObject<int>("NO V Scale", "I", " = 1 V");
    AddSettingObject(analogNO, true);

    analogNO2 = new ValueObject<int>("NO2 V Scale", "L", " = 1 V");
    AddSettingObject(analogNO2, true);

    noSlope = new ValueObject<float>("NO Slope", "G", "");
    AddSettingObject(noSlope, true);

    noZero = new ValueObject<float>("NO Zero", "H", "");
    AddSettingObject(noZero, true);

    no2Slope = new ValueObject<float>("NO2 Slope", "J", "");
    AddSettingObject(no2Slope, true);

    no2Zero = new ValueObject<float>("NO2 Zero", "K", "");
    AddSettingObject(no2Zero, true);

    ozoneFlowSlope = new ValueObject<float>("Ozone Flow Slope", "M", "");
    AddSettingObject(ozoneFlowSlope, true);

    cellFlowSlope = new ValueObject<float>("Cell Flow Slope", "N", "");
    AddSettingObject(cellFlowSlope, true);

    mode = new ValueObject<uchar>("Mode", "O", "");
    AddSettingObject(mode, true);

    errorCode = new ValueObject<int>("Error Code", "_1", "");
    AddSettingObject(errorCode, true);

    dateFormat = new ValueObject<uchar>("Date Format", "_2", "");
    AddSettingObject(dateFormat, true);

    serialNumber = new ValueObject<int>("Serial Number", "R", "");
    AddSettingObject(serialNumber, true);

    date = new ValueObject<QString>("Date", "P");
    AddSettingObject(date, true);

    time = new ValueObject<QString>("Time", "Q");
    AddSettingObject(time, true);

    ReadSettingsFile();

    connect(avgTime, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(adShort, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(adLong, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(adDiff, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(adPercent, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(analogNO, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(analogNO2, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(noSlope, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(noZero, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(no2Slope, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(no2Zero, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(ozoneFlowSlope, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(cellFlowSlope, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(mode, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(errorCode, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(dateFormat, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(serialNumber, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(date, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
    connect(time, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
}

bool SettingsHandler::AddSettingObject(BaseValueObject* set, bool updateSettingsFileOnValChange)
{
    if (set == Q_NULLPTR)
    {
        qDebug() << "ERROR: Trying to add null setting object";
        return false;
    }
    else
    {
        if (AddValue(set))
        {
            if (updateSettingsFileOnValChange)
            {
                connect(set, &BaseValueObject::ValueChanged, this, &SettingsHandler::OnValueChanged);
            }

            if (unknownSettinsMap.contains(set->getMarker()))
            {
                set->setValue_str(unknownSettinsMap.take(set->getMarker()));
            }
        }
        return true;
    }
}

















