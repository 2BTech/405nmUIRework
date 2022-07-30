#ifndef AVERAGETIMEFORM_H
#define AVERAGETIMEFORM_H

#include <QComboBox>

#include "basesettingspage.h"

class AverageTimeForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    AverageTimeForm();
    ~AverageTimeForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

    void UpdateUI() override;

private slots:
    void OnSelectNewAvg(int index);

private:
    // Used to get desired value from user
    QComboBox* avgTimeSelector = Q_NULLPTR;
};

#endif // AVERAGETIMEFORM_H
