#include "averagetimeform.h"

AverageTimeForm::AverageTimeForm() : BaseSettingsPage("Averaging Time")
{

}

AverageTimeForm::~AverageTimeForm()
{

}

void AverageTimeForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    avgTimeSelector = AddComboBoxSetting(SettingsHandler::GetInstance()->GetSetting("A"), QRect(150, 100, 180, 40), QStringList() << "Averaging Time" << "5 Seconds" << "1 Minute" << "5 Minutes" << "1 Hour");
    connect(avgTimeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AverageTimeForm::OnSelectNewAvg);
}

void AverageTimeForm::OnSelectNewAvg(int index)
{
    dynamic_cast<ValueObject<uchar>*>(pageSettings[0].second)->setValue(index);
}

void AverageTimeForm::UpdateUI()
{
    avgTimeSelector->setCurrentIndex(dynamic_cast<ValueObject<uchar>*>(pageSettings[0].second)->getValue());
}
