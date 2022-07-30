#include "nocalform.h"

NOCalForm::NOCalForm() : BaseSettingsPage("NO Calibration")
{

}

NOCalForm::~NOCalForm()
{

}

void NOCalForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    QFont font;
    font.setPointSize(15);

    slopePair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("G"), QRect(140, 70, 240, 50));
    slopePair.second->setText("Slope: 0.00");
    slopePair.second->setFont(font);
    connect(slopePair.first, &QPushButton::clicked, this, &NOCalForm::OnSlopeClicked);

    zeroPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("H"), QRect(140, 120, 240, 50));
    zeroPair.second->setText("Zero: 0.00");
    zeroPair.second->setFont(font);
    connect(zeroPair.first, &QPushButton::clicked, this, &NOCalForm::OnZeroClicked);

    analogPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("I"), QRect(140, 170, 240, 50));
    analogPair.second->setText("Analog, 1 V: 500");
    analogPair.second->setFont(font);
    connect(analogPair.first, &QPushButton::clicked, this, &NOCalForm::OnAnalogClicked);
}

void NOCalForm::OnSlopeClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &NOCalForm::ReceiveSlope);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, 0.5, 3.0);
}

void NOCalForm::ReceiveSlope(QString val)
{
    pageSettings[0].second->setValue_str(val);
    qDebug() << "Received new slope: " << val << " : " << pageSettings[0].second->ToString();
    UpdateUI();
}

void NOCalForm::OnZeroClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &NOCalForm::ReceiveZero);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, 0.5, 3.0);
}

void NOCalForm::ReceiveZero(QString val)
{
    pageSettings[1].second->setValue_str(val);
    UpdateUI();
}

void NOCalForm::OnAnalogClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &NOCalForm::ReceiveAnalog);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 250, 1000);
}

void NOCalForm::ReceiveAnalog(QString val)
{
    pageSettings[2].second->setValue_str(val);
    UpdateUI();
}

void NOCalForm::UpdateUI()
{
    slopePair.second->setText(pageSettings[0].second->getName() + ": " + pageSettings[0].second->ToString());
    zeroPair.second->setText(pageSettings[1].second->getName() + ": " + pageSettings[1].second->ToString());
    analogPair.second->setText(pageSettings[2].second->getName() + ": " + pageSettings[2].second->ToString());
}
