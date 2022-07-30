#include "basesettingspage.h"

BaseSettingsPage::BaseSettingsPage(QString pageName) : BasePage(pageName)
{

}

BaseSettingsPage::~BaseSettingsPage()
{
    // Clean up save button
    if (saveButton)
    {
        saveButton->deleteLater();

        saveButton = Q_NULLPTR;
    }

    while (!pageSettings.isEmpty())
    {
        QPair<BaseValueObject*,BaseValueObject*> pair = pageSettings.takeFirst();
        //pair.first->deleteLater();
        pair.second->deleteLater();
    }

    while (!settingUI.isEmpty())
    {
        settingUI.takeFirst()->deleteLater();
    }
}

void BaseSettingsPage::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont font;
    font.setPointSize(20);

    saveButton = new QPushButton(this);
    saveButton->setGeometry(170, 230, 140, 40);
    saveButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    saveButton->setText("Save");
    saveButton->setFont(font);
    connect(saveButton, &QPushButton::clicked, this, &BaseSettingsPage::OnSaveClicked);
}

void BaseSettingsPage::OnSaveClicked()
{
    // Need to notify the user about the outcome of the save action. This tracks what happens
    bool valueChanged = false;
    // Check each page setting pair to see if the value has changed
    const QList<QPair<BaseValueObject*,BaseValueObject*>> settings = pageSettings;
    for (const QPair<BaseValueObject*,BaseValueObject*> &pair : settings)
    {
        if (!pair.first->ValueEquals(pair.second))
        {
            valueChanged = true;
            pair.first->setValue(pair.second);
        }
    }

    if (valueChanged)
    {
        QMessageBox::information(this, "Saved Setting", "Changes have been applied", QMessageBox::StandardButton::Ok);
    }
    else
    {
        QMessageBox::information(this, "No changes", "There were no changes to apply", QMessageBox::StandardButton::Ok);
    }
}

QPair<QPushButton*,QLabel*> BaseSettingsPage::AddLabelSetting(BaseValueObject* value, QRect geom)
{
    QLabel* setLabel = new QLabel(this);
    setLabel->setText(value->getName() + ": " + value->ToString() + " " + value->getUnits());
    setLabel->setGeometry(geom);
    QPushButton* buttonMask = new QPushButton(this);
    buttonMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    buttonMask->setGeometry(geom);

    settingUI.append(setLabel);
    settingUI.append(buttonMask);

    pageSettings.append(QPair<BaseValueObject*,BaseValueObject*>(value, value->MakeCopy()));

    return QPair<QPushButton*,QLabel*>(buttonMask, setLabel);
}

QComboBox* BaseSettingsPage::AddComboBoxSetting(BaseValueObject* value, QRect geom, QStringList content)
{
    QComboBox* box = new QComboBox(this);
    box->setGeometry(geom);
    box->addItems(content);

    settingUI.append(box);

    pageSettings.append(QPair<BaseValueObject*,BaseValueObject*>(value, value->MakeCopy()));

    return box;
}

void BaseSettingsPage::OnMoveLeftClicked()
{
    if (CanExit())
    {
        emit MoveLeft();
    }
}

void BaseSettingsPage::OnMoveRightClikced()
{
    if (CanExit())
    {
        emit MoveRight();
    }
}

void BaseSettingsPage::OnCloseClicked()
{
    if (CanExit())
    {
        emit ClosePage();
    }
}

bool BaseSettingsPage::CanExit()
{
    bool hasUnsaved = false;
    const QList<QPair<BaseValueObject*,BaseValueObject*>> settings = pageSettings;
    for (const QPair<BaseValueObject*,BaseValueObject*> &pair : settings)
    {
        if (!pair.first->ValueEquals(pair.second))
        {
            hasUnsaved = true;
            break;
        }
    }

    if (hasUnsaved)
    {
        QMessageBox msg;
        msg.setText("Save unsaved settings?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msg.setStyleSheet("QMessageBox { border: 1px}");

        switch(msg.exec())
        {
        case QMessageBox::Yes:
            OnSaveClicked();
            return true;
            break;

        case QMessageBox::No:
            // Reset all local copies
            for (const QPair<BaseValueObject*,BaseValueObject*> &pair : settings)
            {
                pair.second->setValue(pair.first);
            }
            return true;
            break;

        default:
            return false;
            break;
        }
    }
    else
    {
        return true;
    }
}

void BaseSettingsPage::showEvent(QShowEvent* event)
{
    BasePage::showEvent(event);

    if (!isGettingSettings)
    {
        // Reset all local copies
        const QList<QPair<BaseValueObject*,BaseValueObject*>> settings = pageSettings;
        for (const QPair<BaseValueObject*,BaseValueObject*> &pair : settings)
        {
            pair.second->setValue(pair.first);
        }
        UpdateUI();
    }
    else
    {
        isGettingSettings = false;
    }
}
