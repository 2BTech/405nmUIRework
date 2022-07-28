#ifndef BASESETTINGSPAGE_H
#define BASESETTINGSPAGE_H

#include <QDebug>

#include "basepage.h"

class BaseSettingsPage : public BasePage
{
    Q_OBJECT
public:
    BaseSettingsPage(QString pageName);
    ~BaseSettingsPage();

    virtual void BuildUIElements();

protected slots:
    void OnSaveClicked();

protected:
    QPushButton* saveButton = Q_NULLPTR;
};

#endif // BASESETTINGSPAGE_H
