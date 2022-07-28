#ifndef AVERAGETIMEFORM_H
#define AVERAGETIMEFORM_H

#include <QComboBox>

#include "basesettingspage.h"

class AverageTimeForm : public BaseSettingsPage
{
public:
    AverageTimeForm();
    ~AverageTimeForm();

    void BuildUIElements() override;

private:
    QComboBox* avgTimeSelector = Q_NULLPTR;
};

#endif // AVERAGETIMEFORM_H
