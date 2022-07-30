#ifndef SYSTEMSETTINGSFORM_H
#define SYSTEMSETTINGSFORM_H

#include "../basesettingspage.h"

class SystemSettingsForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    SystemSettingsForm();
    ~SystemSettingsForm();

    void BuildUIElements() override;

    void UpdateUI() override;

private slots:
    void OnSelectDateFormat(int index);

    void OnSerialNumberClicked();
    void ReceiveSerialNumber(QString val);

private:
    QComboBox* dateFormatSelector = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> serialNmPair;
};

#endif // SYSTEMSETTINGSFORM_H
