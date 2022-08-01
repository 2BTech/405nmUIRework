#include "staticipform.h"

StaticIPForm::StaticIPForm() : BaseSettingsPage("Static IP Settings")
{

}

StaticIPForm::~StaticIPForm()
{
    if (statusLabel)
    {
        statusLabel->deleteLater();

//        ipPair.first->deleteLater();
//        ipPair.second->deleteLater();

//        gatewayPair.first->deleteLater();
//        gatewayPair.second->deleteLater();

//        subnetPair.first->deleteLater();
//        subnetPair.second->deleteLater();

        applyButton->deleteLater();
        clearButton->deleteLater();

        statusLabel = Q_NULLPTR;

        applyButton = clearButton = Q_NULLPTR;
    }
}

void StaticIPForm::BuildUIElements()
{
    BasePage::BuildUIElements();

    saveButton->deleteLater();

    QFont font;
    font.setPointSize(10);

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(70, 70, 340, 40);
    statusLabel->setText("Status: Using dynamic address");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(font);

    font.setPointSize(12);

    ValueObject<QString>* ipSetting = new ValueObject<QString>("IP Address", "_S1");
    SettingsHandler::GetInstance()->AddSettingObject(ipSetting, true);
    ipPair = AddLabelSetting(ipSetting, QRect(100, 100, 280, 40));

    ipPair.second->setText("IP Address: 000.000.000.000");
    ipPair.second->setFont(font);

    ipPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(ipPair.first, &QPushButton::clicked, this, &StaticIPForm::OnIPClicked);

    ValueObject<QString>* gatewaySetting = new ValueObject<QString>("Gateway", "_S2");
    SettingsHandler::GetInstance()->AddSettingObject(gatewaySetting, true);
    gatewayPair = AddLabelSetting(gatewaySetting, QRect(100, 140, 280, 40));

    gatewayPair.second->setText("Gateway: 000.000.000.000");
    gatewayPair.second->setFont(font);

    gatewayPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(gatewayPair.first, &QPushButton::clicked, this, &StaticIPForm::OnGatewayClicked);

    ValueObject<QString>* subnetSetting = new ValueObject<QString>("Subnet Mask", "_S3");
    SettingsHandler::GetInstance()->AddSettingObject(subnetSetting, true);
    subnetPair = AddLabelSetting(subnetSetting, QRect(100, 180, 280, 40));

    subnetPair.second->setText("Subnet Mask: 000.000.000.000");
    subnetPair.second->setFont(font);

    subnetPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(subnetPair.first, &QPushButton::clicked, this, &StaticIPForm::OnSubnetClicked);

    clearButton = new QPushButton(this);
    clearButton->setGeometry(100, 230, 120, 40);
    clearButton->setText("Clear");
    clearButton->setFont(font);
    clearButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(clearButton, &QPushButton::clicked, this, &StaticIPForm::OnClearClicked);

    applyButton = new QPushButton(this);
    applyButton->setGeometry(260, 230, 120, 40);
    applyButton->setText("Apply");
    applyButton->setFont(font);
    applyButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(applyButton, &QPushButton::clicked, this, &StaticIPForm::OnApplyClicked);

    SettingsHandler::GetInstance()->ReadSettingsFile();

    HandleOldSettingsFile();
}

void StaticIPForm::OnIPClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &StaticIPForm::ReceiveStaticIP);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_STRING);
}

void StaticIPForm::ReceiveStaticIP(QString val)
{
    pageSettings[0].second->setValue_str(val);
    UpdateUI();
}

void StaticIPForm::OnGatewayClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &StaticIPForm::ReceiveGateway);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_STRING);
}

void StaticIPForm::ReceiveGateway(QString val)
{
    pageSettings[1].second->setValue_str(val);
    UpdateUI();
}

void StaticIPForm::OnSubnetClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &StaticIPForm::ReceiveSubnet);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_STRING);
}

void StaticIPForm::ReceiveSubnet(QString val)
{
    pageSettings[2].second->setValue_str(val);
    UpdateUI();
}

void StaticIPForm::OnApplyClicked()
{
    OnSaveClicked();

    emit UpdateStaticIPSettings(pageSettings[0].first->ToString(), pageSettings[1].first->ToString(), pageSettings[2].first->ToString());
}

void StaticIPForm::OnClearClicked()
{
    emit ClearStaticIPAddress();
}

void StaticIPForm::UpdateUI()
{
    ipPair.second->setText("IP Address: " + pageSettings[0].second->ToString());
    gatewayPair.second->setText("Gateway: " + pageSettings[1].second->ToString());
    subnetPair.second->setText("Subnet Mask: " + pageSettings[2].second->ToString());
}

void StaticIPForm::HandleOldSettingsFile()
{
    QFile file(QString(WORKING_DIR).append("StaticIPSettings.txt"));
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                line = line.remove('\r').remove('\n');
                switch (line.at(0).toLatin1())
                {
                case 'A':
                    pageSettings[0].first->setValue_str(line.remove(0, 1));
                    break;

                case 'B':
                    pageSettings[1].first->setValue_str(line.remove(0, 1));
                    break;

                case 'C':
                    pageSettings[2].first->setValue_str(line.remove(0, 1));
                    break;

                default:
                    qDebug() << "Read in bad character: " << line.at(0).toLatin1();
                }
            }
            file.close();
            file.remove();
        }
    }
}
