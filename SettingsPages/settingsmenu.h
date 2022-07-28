#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "basesettingspage.h"

#include "averagetimeform.h"

class SettingsMenu : public BasePage
{
public:
    SettingsMenu();
    ~SettingsMenu();

    void BuildUIElements() override;

private:
    QPushButton* avgTimeButton = Q_NULLPTR;
    QPushButton* modeButton = Q_NULLPTR;
    QPushButton* adFilterButton = Q_NULLPTR;
    QPushButton* dateTimeButton = Q_NULLPTR;
    QPushButton* noCalButton = Q_NULLPTR;
    QPushButton* no2CalButton = Q_NULLPTR;
    QPushButton* flowCalButton = Q_NULLPTR;
    QPushButton* dataFileButton = Q_NULLPTR;
    QPushButton* modbusButton = Q_NULLPTR;
    QPushButton* staticIPButton = Q_NULLPTR;
    QPushButton* sysSetButton = Q_NULLPTR;
    QPushButton* aboutButton = Q_NULLPTR;

    AverageTimeForm* avgForm = Q_NULLPTR;
};

#endif // SETTINGSMENU_H
