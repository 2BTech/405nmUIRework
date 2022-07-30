#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "basesettingspage.h"

#include "averagetimeform.h"
#include "Settings/adaptivefilterform.h"
#include "Settings/modeform.h"
#include "Settings/datetimeform.h"
#include "Settings/nocalform.h"
#include "Settings/no2calform.h"
#include "Settings/flowcalform.h"
#include "Settings/datafilemenuform.h"
#include "Settings/modbusserverform.h"
#include "Settings/staticipform.h"
#include "Settings/systemsettingsform.h"
#include "Settings/aboutpage.h"

class SettingsMenu : public BasePage
{
    Q_OBJECT
public:
    SettingsMenu();
    ~SettingsMenu();

    void BuildUIElements() override;

signals:
    void CloseMenu();

private slots:
    void OnPageButtonClicked();

    void OnPageMoveLeft();
    void OnPageMoveRight();
    void OnPageClose();

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
    ModeForm* modeForm = Q_NULLPTR;
    AdaptiveFilterForm* adFilterForm = Q_NULLPTR;
    DateTimeForm* dateTimeForm = Q_NULLPTR;
    NOCalForm* noCalForm = Q_NULLPTR;
    NO2CalForm* no2CalForm = Q_NULLPTR;
    FlowCalForm* flowCalForm = Q_NULLPTR;
    DataFileMenuForm* dataFileForm = Q_NULLPTR;
    ModbusServerForm* modbusServerForm = Q_NULLPTR;
    StaticIPForm* staticIPForm = Q_NULLPTR;
    SystemSettingsForm* systemSettingsForm = Q_NULLPTR;
    AboutPage* aboutForm = Q_NULLPTR;

    QList<BasePage*> pages;
    QList<QPushButton*> pageButtons;

    int currentPageIndex = 0;

    void JumpToPage(int index);
};

#endif // SETTINGSMENU_H
