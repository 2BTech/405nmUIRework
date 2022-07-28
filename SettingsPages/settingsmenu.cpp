#include "settingsmenu.h"

SettingsMenu::SettingsMenu() : BasePage("Settings Menu")
{
    avgForm = new AverageTimeForm();
    avgForm->BuildUIElements();
    avgForm->show();
}

SettingsMenu::~SettingsMenu()
{
    if (avgTimeButton)
    {
        avgTimeButton->deleteLater();
        modbusButton->deleteLater();
        adFilterButton->deleteLater();
        dateTimeButton->deleteLater();
        noCalButton->deleteLater();
        no2CalButton->deleteLater();
        flowCalButton->deleteLater();
        dataFileButton->deleteLater();
        modbusButton->deleteLater();
        staticIPButton->deleteLater();
        sysSetButton->deleteLater();
        aboutButton->deleteLater();
    }
}

void SettingsMenu::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont buttonFont;
    buttonFont.setPointSize(12);

    // Left side
    avgTimeButton = new QPushButton(this);
    avgTimeButton->setGeometry(75, 65, 155, 30);
    avgTimeButton->setText("Avg Time");
    avgTimeButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    avgTimeButton->setFont(buttonFont);

    adFilterButton = new QPushButton(this);
    adFilterButton->setGeometry(75, 100, 155, 30);
    adFilterButton->setText("Adaptive Filter");
    adFilterButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    adFilterButton->setFont(buttonFont);

    noCalButton = new QPushButton(this);
    noCalButton->setGeometry(75, 135, 155, 30);
    noCalButton->setText("NO Cal");
    noCalButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    noCalButton->setFont(buttonFont);

    flowCalButton = new QPushButton(this);
    flowCalButton->setGeometry(75, 170, 155, 30);
    flowCalButton->setText("Flow Cal");
    flowCalButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    flowCalButton->setFont(buttonFont);

    modbusButton = new QPushButton(this);
    modbusButton->setGeometry(75, 205, 155, 30);
    modbusButton->setText("Modbus");
    modbusButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    modbusButton->setFont(buttonFont);

    sysSetButton = new QPushButton(this);
    sysSetButton->setGeometry(75, 240, 155, 30);
    sysSetButton->setText("System");
    sysSetButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    sysSetButton->setFont(buttonFont);

    // Right side
    modeButton = new QPushButton(this);
    modeButton->setGeometry(250, 65, 155, 30);
    modeButton->setText("Mode");
    modeButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    modeButton->setFont(buttonFont);

    dateTimeButton = new QPushButton(this);
    dateTimeButton->setGeometry(250, 100, 155, 30);
    dateTimeButton->setText("Date Time");
    dateTimeButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    dateTimeButton->setFont(buttonFont);

    no2CalButton = new QPushButton(this);
    no2CalButton->setGeometry(250, 135, 155, 30);
    no2CalButton->setText("NO2 Cal");
    no2CalButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    no2CalButton->setFont(buttonFont);

    dataFileButton = new QPushButton(this);
    dataFileButton->setGeometry(250, 170, 155, 30);
    dataFileButton->setText("Data files");
    dataFileButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    dataFileButton->setFont(buttonFont);

    staticIPButton = new QPushButton(this);
    staticIPButton->setGeometry(250, 205, 155, 30);
    staticIPButton->setText("Static IP");
    staticIPButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    staticIPButton->setFont(buttonFont);

    aboutButton = new QPushButton(this);
    aboutButton->setGeometry(250, 240, 155, 30);
    aboutButton->setText("About");
    aboutButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    aboutButton->setFont(buttonFont);
}
