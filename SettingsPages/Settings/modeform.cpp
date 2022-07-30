#include "modeform.h"

ModeForm::ModeForm() : BaseSettingsPage("Mode")
{

}

ModeForm::~ModeForm()
{

}

void ModeForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    modeSelector = AddComboBoxSetting(SettingsHandler::GetInstance()->GetSetting("O"), QRect(150, 100, 180, 40), QStringList() << "Mode" << "NO" << "NO2" << "NO and NO2");
    connect(modeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ModeForm::OnSelectNewMode);
}

void ModeForm::OnSelectNewMode(int index)
{
    dynamic_cast<ValueObject<uchar>*>(pageSettings[0].second)->setValue(index);
}

void ModeForm::UpdateUI()
{
    modeSelector->setCurrentIndex(dynamic_cast<ValueObject<uchar>*>(pageSettings[0].second)->getValue());
}
