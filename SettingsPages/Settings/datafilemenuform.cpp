#include "datafilemenuform.h"

DataFileMenuForm::DataFileMenuForm() : BasePage("Data Files")
{
    menuForm = new DataFileMenu();
    menuForm->BuildUIElements();
    menuForm->show();
}

DataFileMenuForm::~DataFileMenuForm()
{
    if (currentFileLabel)
    {
        currentFileLabel->deleteLater();
        selectFileLabel->deleteLater();
        usbLabel->deleteLater();

        refreshButton->deleteLater();
        deleteButton->deleteLater();
        selectButton->deleteLater();
        saveButton->deleteLater();

        currentFileLabel = selectFileLabel = usbLabel = Q_NULLPTR;

        refreshButton = deleteButton = selectButton = saveButton = Q_NULLPTR;
    }
}

void DataFileMenuForm::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont font;
    font.setPointSize(12);

    usbLabel = new QLabel(this);
    usbLabel->setGeometry(70, 70, 160, 40);
    usbLabel->setText("No usb connected");
    usbLabel->setFont(font);

    refreshButton = new QPushButton(this);
    refreshButton->setGeometry(300, 70, 100, 40);
    refreshButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    refreshButton->setText("Refresh");
    refreshButton->setFont(font);

    currentFileLabel = new QLabel(this);
    currentFileLabel->setGeometry(70, 120, 240, 40);
    currentFileLabel->setText("Current File: Not Set");
    currentFileLabel->setFont(font);

    selectFileLabel = new QLabel(this);
    selectFileLabel->setGeometry(70, 170, 240, 40);
    selectFileLabel->setText("Selected File: Not Set");
    selectFileLabel->setFont(font);

    deleteButton = new QPushButton(this);
    deleteButton->setGeometry(70, 230, 100, 40);
    deleteButton->setText("Delete");
    deleteButton->setFont(font);
    deleteButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(deleteButton, &QPushButton::clicked, this, &DataFileMenuForm::OnDeleteClicked);

    selectButton = new QPushButton(this);
    selectButton->setGeometry(190, 230, 100, 40);
    selectButton->setText("Select");
    selectButton->setFont(font);
    selectButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(selectButton, &QPushButton::clicked, this, &DataFileMenuForm::OnSelectClicked);

    saveButton = new QPushButton(this);
    saveButton->setGeometry(310, 230, 100, 40);
    saveButton->setText("Save");
    saveButton->setFont(font);
    saveButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(saveButton, &QPushButton::clicked, this, &DataFileMenuForm::OnSaveClicked);
}

void DataFileMenuForm::OnDeleteClicked()
{

}

void DataFileMenuForm::OnSelectClicked()
{

}

void DataFileMenuForm::OnSaveClicked()
{

}

void DataFileMenuForm::OnRefreshClicked()
{

}
