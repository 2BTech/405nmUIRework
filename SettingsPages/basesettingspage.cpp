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
}

void BaseSettingsPage::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont font;
    font.setPointSize(20);

    saveButton = new QPushButton(this);
    saveButton->setGeometry(170, 230, 140, 40);
    saveButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    saveButton->setText("Save");
    saveButton->setFont(font);
    connect(saveButton, &QPushButton::clicked, this, &BaseSettingsPage::OnSaveClicked);
}

void BaseSettingsPage::OnSaveClicked()
{
    qDebug() << "Clicked on save in " << pageName;
}
