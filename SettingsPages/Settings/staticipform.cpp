#include "staticipform.h"

StaticIPForm::StaticIPForm() : BaseSettingsPage("Static IP Settings")
{
    connect(&setStaticIPProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(OnSetStaticIPExit(int,QProcess::ExitStatus)));
    connect(&setStaticIPProcess, SIGNAL(finished(QProcess::ProcessError)), this, SLOT(OnSetStaticIPError(QProcess::ProcessError)));
    setStaticIPProcess.setProgram("/home/2b/SetStaticIP");

    connect(&clearStaticIPPRocess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(OnSetStaticIPExit(int,QProcess::ExitStatus)));
    connect(&clearStaticIPPRocess, SIGNAL(finished(QProcess::ProcessError)), this, SLOT(OnSetStaticIPError(QProcess::ProcessError)));
    clearStaticIPPRocess.setProgram("/home/2b/ClearStaticIP.sh");

    blocking = new BlockingForm();
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

    if (saveButton)
    {
        saveButton->hide();
    }

    QFont font;
    font.setPointSize(10);

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(70, 70, 340, 40);
    statusLabel->setText("Status: Using dynamic address");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(font);

    //qDebug() << "Created status label";

    font.setPointSize(12);

    ValueObject<QString>* ipSetting = new ValueObject<QString>("Static IP Address", "_S1");
    SettingsHandler::GetInstance()->AddSettingObject(ipSetting, true);
    ipPair = AddLabelSetting(ipSetting, QRect(100, 100, 280, 40));

    //qDebug() << "Created ip setting";

    ipPair.second->setText("IP Address: 000.000.000.000");
    ipPair.second->setFont(font);

    ipPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(ipPair.first, &QPushButton::clicked, this, &StaticIPForm::OnIPClicked);

    //qDebug() << "Created ip pair";

    ValueObject<QString>* gatewaySetting = new ValueObject<QString>("Gateway", "_S2");
    SettingsHandler::GetInstance()->AddSettingObject(gatewaySetting, true);
    gatewayPair = AddLabelSetting(gatewaySetting, QRect(100, 140, 280, 40));

    //qDebug() << "Created gateway setting";

    gatewayPair.second->setText("Gateway: 000.000.000.000");
    gatewayPair.second->setFont(font);

    gatewayPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(gatewayPair.first, &QPushButton::clicked, this, &StaticIPForm::OnGatewayClicked);

    //qDebug() << "Created gateway pair";

    ValueObject<QString>* subnetSetting = new ValueObject<QString>("Subnet Mask", "_S3");
    SettingsHandler::GetInstance()->AddSettingObject(subnetSetting, true);
    subnetPair = AddLabelSetting(subnetSetting, QRect(100, 180, 280, 40));

    //qDebug() << "Created subnet setting";

    subnetPair.second->setText("Subnet Mask: 000.000.000.000");
    subnetPair.second->setFont(font);

    subnetPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(subnetPair.first, &QPushButton::clicked, this, &StaticIPForm::OnSubnetClicked);

    //qDebug() << "Created subnet pair";

    clearButton = new QPushButton(this);
    clearButton->setGeometry(100, 230, 120, 40);
    clearButton->setText("Clear");
    clearButton->setFont(font);
    clearButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(clearButton, &QPushButton::clicked, this, &StaticIPForm::OnClearClicked);

    //qDebug() << "Created clear button";

    applyButton = new QPushButton(this);
    applyButton->setGeometry(260, 230, 120, 40);
    applyButton->setText("Apply");
    applyButton->setFont(font);
    applyButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(applyButton, &QPushButton::clicked, this, &StaticIPForm::OnApplyClicked);

    //qDebug() << "Created apply button";

    SettingsHandler::GetInstance()->ReadSettingsFile();

    //qDebug() << "Read in current settings";

    HandleOldSettingsFile();

    //qDebug() << "Handled old settings file";
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

    setStaticIPProcess.setArguments(QStringList() << pageSettings[0].first->ToString() << pageSettings[1].first->ToString() << pageSettings[2].first->ToString());
    setStaticIPProcess.start();
}

void StaticIPForm::OnClearClicked()
{
    clearStaticIPPRocess.start();
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
        qDebug() << "Found old static ip settings file";
        if (file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Started reading";
            while (!file.atEnd())
            {
                qDebug() << "Reading line";
                QString line = file.readLine();
                line = line.remove('\r').remove('\n');
                switch (line.at(0).toLatin1())
                {
                case 'A':
                    qDebug() << "Before set ip";
                    pageSettings[0].first->setValue_str(line.remove(0, 1));
                    qDebug() << "After set ip";
                    break;

                case 'B':
                    qDebug() << "Before set gateway";
                    pageSettings[1].first->setValue_str(line.remove(0, 1));
                    qDebug() << "After set gateway";
                    break;

                case 'C':
                    qDebug() << "Before set subnet";
                    pageSettings[2].first->setValue_str(line.remove(0, 1));
                    qDebug() << "After set subnet";
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

void StaticIPForm::OnSetStaticIPExit(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit)
    {
        QMessageBox::information(this, "Success", "Successfully set static IP Address to " + pageSettings.first().first->ToString());
    }
    else
    {
        QMessageBox::warning(this, "Failed", "Failed to set static IP Address to " + pageSettings.first().first->ToString());
    }

    blocking->hide();
}

void StaticIPForm::OnSetStaticIPError(QProcess::ProcessError error)
{
    QMessageBox::warning(this, "Failed", "Failed to set static IP Address to " + pageSettings.first().first->ToString());
    qDebug() << "Set static ip error: " << error;
    blocking->hide();
}

void StaticIPForm::OnClearStaticIPExit(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit)
    {
        QMessageBox::information(this, "Success", "Successfully cleared Static IP Address");
    }
    else
    {
        QMessageBox::warning(this, "Failed", "Failed to clear static IP Address");
    }
    blocking->hide();
}

void StaticIPForm::OnClearStaticIPEror(QProcess::ProcessError error)
{
    QMessageBox::warning(this, "Failed", "Failed to clear static IP Address");
    qDebug() << "Clear static ip error: " << error;
    blocking->hide();
}
