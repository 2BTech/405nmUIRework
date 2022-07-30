#include "systemsettingsform.h"

SystemSettingsForm::SystemSettingsForm() : BaseSettingsPage("System Settings")
{

}

SystemSettingsForm::~SystemSettingsForm()
{

}

void SystemSettingsForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    dateFormatSelector = AddComboBoxSetting(SettingsHandler::GetInstance()->GetSetting("*"), QRect(130, 100, 200, 40), QStringList() << "Date Format" << "DD/MM/YY" << "DD/MM/YYYY" << "YY/MM/DD" << "YYYY/MM/DD");

    QFont font;
    font.setPointSize(12);

    serialNmPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("R"), QRect(130, 150, 200, 40));
    serialNmPair.second->setText("Serial Number: 0000");
    serialNmPair.second->setFont(font);
}

void SystemSettingsForm::OnSelectDateFormat(int index)
{
    pageSettings[0].second->setValue_str(QString::number(index));
    UpdateUI();
}

void SystemSettingsForm::OnSerialNumberClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &SystemSettingsForm::ReceiveSerialNumber);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_INT, 1000, 9999);
}

void SystemSettingsForm::ReceiveSerialNumber(QString val)
{
    pageSettings[1].second->setValue_str(val);
    UpdateUI();
}

void SystemSettingsForm::UpdateUI()
{
    dateFormatSelector->setCurrentIndex(dynamic_cast<ValueObject<uchar>*>(pageSettings[0].second)->getValue());
    serialNmPair.second->setText(pageSettings[1].second->getName() + ": " + pageSettings[1].second->ToString());
}
