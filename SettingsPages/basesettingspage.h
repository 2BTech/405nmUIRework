#ifndef BASESETTINGSPAGE_H
#define BASESETTINGSPAGE_H

#include <QDebug>
// Used to notify the user about state changes
#include <QMessageBox>
// Displays settings
#include <QComboBox>

#include "basepage.h"
// Interacting with settings
#include "ValueHandlers/ValueObjects/valueobjects.h"
// Access settings
#include "ValueHandlers/settingshandler.h"
// Get user input
#include "Utility/numberpadform.h"

class BaseSettingsPage : public BasePage
{
    Q_OBJECT
public:
    BaseSettingsPage(QString pageName);
    ~BaseSettingsPage();

    virtual void BuildUIElements() override;

protected slots:
    virtual void OnMoveLeftClicked() override;
    virtual void OnMoveRightClikced() override;
    virtual void OnCloseClicked() override;

    virtual void OnSaveClicked();

protected:
    // Button used to start the save process
    QPushButton* saveButton = Q_NULLPTR;
    // Holds all created ui elements
    QList<QWidget*> settingUI;
    // Maps a button mask to the setting it represents. Used for handling button clicks
    QHash<QPushButton*,BaseValueObject*> buttonSettingMap;

    // List of all settings tracked by the page
    QList<QPair<BaseValueObject*,BaseValueObject*>> pageSettings;

    QPair<QPushButton*,QLabel*> AddLabelSetting(BaseValueObject* value, QRect geom);
    QComboBox* AddComboBoxSetting(BaseValueObject* value, QRect geom, QStringList content);

    bool isGettingSettings = false;

    bool CanExit();

    virtual void UpdateUI() = 0;

    void showEvent(QShowEvent* event) override;
};

#endif // BASESETTINGSPAGE_H
