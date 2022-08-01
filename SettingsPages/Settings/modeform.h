#ifndef MODEFORM_H
#define MODEFORM_H

#include <QComboBox>

#include "../basesettingspage.h"

class ModeForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    ModeForm();
    ~ModeForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

private slots:
    void OnSelectNewMode(int index);

private:
    // Used to get desired value from user
    QComboBox* modeSelector = Q_NULLPTR;

    void UpdateUI() override;
};

#endif // MODEFORM_H
