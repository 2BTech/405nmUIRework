#include "no2calform.h"

NO2CalForm::NO2CalForm() : BaseSettingsPage("NO2 Calibration")
{

}

NO2CalForm::~NO2CalForm()
{

}

void NO2CalForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    QFont font;
    font.setPointSize(15);

    slopePair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("J"), QRect(140, 70, 240, 50));
    slopePair.second->setText("Slope: 0.00");
    slopePair.second->setFont(font);
    connect(slopePair.first, &QPushButton::clicked, this, &NO2CalForm::OnSlopeClicked);

    zeroPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("K"), QRect(140, 120, 240, 50));
    zeroPair.second->setText("Zero: 0.00");
    zeroPair.second->setFont(font);
    connect(zeroPair.first, &QPushButton::clicked, this, &NO2CalForm::OnZeroClicked);

    analogPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("L"), QRect(140, 170, 240, 50));
    analogPair.second->setText("Analog, 1 V: 500");
    analogPair.second->setFont(font);
    connect(analogPair.first, &QPushButton::clicked, this, &NO2CalForm::OnAnalogClicked);
}

void NO2CalForm::OnSlopeClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &NO2CalForm::ReceiveSlope);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, 0.5, 3.0);
}

void NO2CalForm::ReceiveSlope(QString val)
{
    pageSettings[0].second->setValue_str(val);
    UpdateUI();
}

void NO2CalForm::OnZeroClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &NO2CalForm::ReceiveZero);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, 0.5, 3.0);
}

void NO2CalForm::ReceiveZero(QString val)
{
    pageSettings[1].second->setValue_str(val);
    UpdateUI();
}

void NO2CalForm::OnAnalogClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &NO2CalForm::ReceiveAnalog);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 250, 1000);
}

void NO2CalForm::ReceiveAnalog(QString val)
{
    pageSettings[2].second->setValue_str(val);
    UpdateUI();
}

void NO2CalForm::UpdateUI()
{
    slopePair.second->setText(pageSettings[0].second->getName() + ": " + pageSettings[0].second->ToString());
    zeroPair.second->setText(pageSettings[1].second->getName() + ": " + pageSettings[1].second->ToString());
    analogPair.second->setText(pageSettings[2].second->getName() + ": " + pageSettings[2].second->ToString());
}
