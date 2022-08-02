#include "modbusserverform.h"

ModbusServerForm::ModbusServerForm() : BaseSettingsPage("Modbus Server")
{
    connect(this, &ModbusServerForm::UpdateModbus, ModbusHandler::GetInstance(), &ModbusHandler::UpdateModbusSettings);
    connect(ModbusHandler::GetInstance(), &ModbusHandler::TCPSlaveConnection, this, &ModbusServerForm::OnModbusConnection);
    connect(ModbusHandler::GetInstance(), &ModbusHandler::Disconnected, this, &ModbusServerForm::OnModbusDisconnect);
}

ModbusServerForm::~ModbusServerForm()
{
    if (statusLabel)
    {
//        ipPair.first->deleteLater();
//        ipPair.second->deleteLater();

//        portPair.first->deleteLater();
//        portPair.second->deleteLater();

//        idPair.first->deleteLater();
//        idPair.second->deleteLater();

//        updateButton->deleteLater();

//        statusLabel->deleteLater();

//        updateButton = Q_NULLPTR;

        statusLabel = Q_NULLPTR;
    }
}

void ModbusServerForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    QFont font;
    font.setPointSize(10);

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(70, 70, 340, 40);
    statusLabel->setText("Not Connected");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(font);

    font.setPointSize(12);

    ValueObject<QString>* tempSetting = new ValueObject<QString>("IP Address", "_M1");
    SettingsHandler::GetInstance()->AddSettingObject(tempSetting, true);

    ipPair = AddLabelSetting(tempSetting, QRect(100, 100, 280, 40));

    ipPair.second->setText("IP Address: 000.000.000.000");
    ipPair.second->setFont(font);

    ipPair.first->setGeometry(100, 100, 280, 40);
    ipPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(ipPair.first, &QPushButton::clicked, this, &ModbusServerForm::OnIPAddressClicked);

    ValueObject<int>* tempIntSetting = new ValueObject<int>("Port", "_M2");
    SettingsHandler::GetInstance()->AddSettingObject(tempIntSetting, true);

    portPair = AddLabelSetting(tempIntSetting, QRect(100, 140, 280, 40));

    portPair.second->setText("Port: 50200");
    portPair.second->setFont(font);

    portPair.first->setGeometry(100, 140, 280, 40);
    portPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(portPair.first, &QPushButton::clicked, this, &ModbusServerForm::OnPortClicked);

    ValueObject<uchar>* tempUCharSetting = new ValueObject<uchar>("Device ID", "_M3");
    SettingsHandler::GetInstance()->AddSettingObject(tempUCharSetting, true);
    idPair = AddLabelSetting(tempUCharSetting, QRect(100, 180, 280, 40));

    idPair.second->setText("Device ID: 50200");
    idPair.second->setFont(font);

    idPair.first->setGeometry(100, 180, 280, 40);
    idPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(idPair.first, &QPushButton::clicked, this, &ModbusServerForm::OnDeviceIDClicked);

//    updateButton = new QPushButton(this);
//    updateButton->setGeometry(180, 230, 120, 40);
//    updateButton->setText("Update");
//    updateButton->setFont(font);
//    updateButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
//    connect(updateButton, &QPushButton::clicked, this, &ModbusServerForm::OnUpdateClicked);
    saveButton->setText("Update");

    SettingsHandler::GetInstance()->ReadSettingsFile();

    CheckForOldSettings();

    emit UpdateModbus(0, dynamic_cast<ValueObject<QString>*>(pageSettings[0].first)->getValue(), dynamic_cast<ValueObject<int>*>(pageSettings[1].first)->getValue(), dynamic_cast<ValueObject<uchar>*>(pageSettings[2].first)->getValue());
}

void ModbusServerForm::OnIPAddressClicked()
{
    QString address = Utilities::GetIPAddress();
    pageSettings[0].first->setValue_str(address);
    pageSettings[0].second->setValue_str(address);
    UpdateUI();
}

void ModbusServerForm::OnPortClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &ModbusServerForm::ReceiveNewPort);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 0, 65535);
}

void ModbusServerForm::ReceiveNewPort(QString val)
{
    pageSettings[1].second->setValue_str(val);
    UpdateUI();
}

void ModbusServerForm::OnDeviceIDClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &ModbusServerForm::ReceiveNewID);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 0, 255);
}

void ModbusServerForm::ReceiveNewID(QString val)
{
    pageSettings[2].second->setValue_str(val);
    UpdateUI();
}

void ModbusServerForm::OnSaveClicked()
{
    BaseSettingsPage::OnSaveClicked();

    emit UpdateModbus(0, dynamic_cast<ValueObject<QString>*>(pageSettings[0].first)->getValue(), dynamic_cast<ValueObject<int>*>(pageSettings[1].first)->getValue(), dynamic_cast<ValueObject<uchar>*>(pageSettings[2].first)->getValue());
}

void ModbusServerForm::OnModbusConnection(QString peerIP)
{
    statusLabel->setText("Connection from: " + peerIP);
}

void ModbusServerForm::OnModbusDisconnect()
{
    statusLabel->setText("Not connected");
}

void ModbusServerForm::UpdateUI()
{
    ipPair.second->setText("IP Addr: " + pageSettings[0].second->ToString());
    portPair.second->setText("Port: " + pageSettings[1].second->ToString());
    idPair.second->setText("Device ID: " + pageSettings[2].second->ToString());
}

void ModbusServerForm::showEvent(QShowEvent* event)
{
    BaseSettingsPage::showEvent(event);

    OnIPAddressClicked();
}

void ModbusServerForm::CheckForOldSettings()
{
    QFile oldSettings(QString(WORKING_DIR).append("ModbusSettings.txt"));
    if (oldSettings.exists())
    {
        if (oldSettings.open(QIODevice::ReadOnly))
        {
            while (!oldSettings.atEnd())
            {
                QString line = oldSettings.readLine();

                if (line.startsWith('A'))
                {
                    SettingsHandler::GetInstance()->GetSetting("_M2")->setValue_str(line.remove(0, 1));
                }
                else if (line.startsWith('B'))
                {
                    SettingsHandler::GetInstance()->GetSetting("_M3")->setValue_str(line.remove(0, 1));
                }
            }
            oldSettings.close();
            oldSettings.remove();
        }
    }
}
