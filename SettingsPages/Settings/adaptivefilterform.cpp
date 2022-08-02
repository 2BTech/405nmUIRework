#include "adaptivefilterform.h"

AdaptiveFilterForm::AdaptiveFilterForm() : BaseSettingsPage("Adaptive Filter")
{

}

AdaptiveFilterForm::~AdaptiveFilterForm()
{

}

void AdaptiveFilterForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    QFont font;
    font.setPointSize(10);

    shortPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("C"), QRect(70, 70, 160, 60));
    connect(shortPair.first, &QPushButton::clicked, this, &AdaptiveFilterForm::OnShortClicked);
    shortPair.second->setFont(font);

    longPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("D"), QRect(250, 70, 160, 60));
    connect(longPair.first, &QPushButton::clicked, this, &AdaptiveFilterForm::OnLongClicked);
    longPair.second->setFont(font);

    diffPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("E"), QRect(70, 140, 160, 60));
    connect(diffPair.first, &QPushButton::clicked, this, &AdaptiveFilterForm::OnDiffClicked);
    diffPair.second->setFont(font);

    perPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("F"), QRect(250, 140, 160, 60));
    connect(perPair.first, &QPushButton::clicked, this, &AdaptiveFilterForm::OnPerClicked);
    perPair.second->setFont(font);
}

void AdaptiveFilterForm::OnShortClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &AdaptiveFilterForm::ReceiveNewShort);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 1, 5);
}

void AdaptiveFilterForm::ReceiveNewShort(QString val)
{
    pageSettings[0].second->setValue_str(val);
    UpdateUI();
    qDebug() << "Received new short: " << val << " : " << pageSettings[0].second->ToString();
}

void AdaptiveFilterForm::OnLongClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &AdaptiveFilterForm::ReceiveNewLong);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 6, 50);
}

void AdaptiveFilterForm::ReceiveNewLong(QString val)
{
    pageSettings[1].second->setValue_str(val);
    UpdateUI();
}

void AdaptiveFilterForm::OnDiffClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &AdaptiveFilterForm::ReceiveNewDiff);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 0, 50);
}

void AdaptiveFilterForm::ReceiveNewDiff(QString val)
{
    pageSettings[2].second->setValue_str(val);
    UpdateUI();
}

void AdaptiveFilterForm::OnPerClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &AdaptiveFilterForm::ReceiveNewPer);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT,0, 10);
}

void AdaptiveFilterForm::ReceiveNewPer(QString val)
{
    pageSettings[3].second->setValue_str(val);
    UpdateUI();
}

void AdaptiveFilterForm::UpdateUI()
{
    shortPair.second->setText("Short: " + pageSettings[0].second->ToString());
    longPair.second->setText("Long: " + pageSettings[1].second->ToString());
    diffPair.second->setText("Difference: " + pageSettings[2].second->ToString());
    perPair.second->setText("Percent: " + pageSettings[3].second->ToString());
}
