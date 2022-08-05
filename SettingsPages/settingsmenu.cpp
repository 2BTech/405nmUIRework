#include "settingsmenu.h"

SettingsMenu::SettingsMenu() : BasePage("Settings Menu")
{
    connect(this, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(this, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(this, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Connected Setting Menu Movement";

    avgForm = new AverageTimeForm();
    avgForm->BuildUIElements();
    avgForm->hide();
    connect(avgForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(avgForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(avgForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created avg form";

    modeForm = new ModeForm();
    modeForm->BuildUIElements();
    modeForm->hide();
    connect(modeForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(modeForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(modeForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created Mode";

    adFilterForm = new AdaptiveFilterForm();
    adFilterForm->BuildUIElements();
    adFilterForm->hide();
    connect(adFilterForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(adFilterForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(adFilterForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created ad filter";

    dateTimeForm = new DateTimeForm();
    dateTimeForm->BuildUIElements();
    dateTimeForm->hide();
    connect(dateTimeForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(dateTimeForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(dateTimeForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created date time";

    noCalForm = new NOCalForm();
    noCalForm->BuildUIElements();
    noCalForm->hide();
    connect(noCalForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(noCalForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(noCalForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created no cal";

    no2CalForm = new NO2CalForm();
    no2CalForm->BuildUIElements();
    no2CalForm->hide();
    connect(no2CalForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(no2CalForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(no2CalForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created NO2";

    flowCalForm = new FlowCalForm();
    flowCalForm->BuildUIElements();
    flowCalForm->hide();
    connect(flowCalForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(flowCalForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(flowCalForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created flow cal";

    dataFileForm = new DataFileMenuForm();
    dataFileForm->BuildUIElements();
    dataFileForm->hide();
    connect(dataFileForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(dataFileForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(dataFileForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created data files form";

    modbusServerForm = new ModbusServerForm();
    modbusServerForm->BuildUIElements();
    modbusServerForm->hide();
    connect(modbusServerForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(modbusServerForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(modbusServerForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created modbus server form";

    staticIPForm = new StaticIPForm();
    staticIPForm->BuildUIElements();
    staticIPForm->hide();
    connect(staticIPForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(staticIPForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(staticIPForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created static IP form";

    systemSettingsForm = new SystemSettingsForm();
    systemSettingsForm->BuildUIElements();
    systemSettingsForm->hide();
    connect(systemSettingsForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(systemSettingsForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(systemSettingsForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created system settings form";

    aboutForm = new AboutPage();
    aboutForm->BuildUIElements();
    aboutForm->hide();
    connect(aboutForm, &BasePage::ClosePage, this, &SettingsMenu::OnPageClose);
    connect(aboutForm, &BasePage::MoveLeft, this, &SettingsMenu::OnPageMoveLeft);
    connect(aboutForm, &BasePage::MoveRight, this, &SettingsMenu::OnPageMoveRight);

    //qDebug() << "Created about form";

    pages << this << avgForm << modeForm << adFilterForm << dateTimeForm << noCalForm << no2CalForm << flowCalForm << dataFileForm << modbusServerForm << staticIPForm << systemSettingsForm << aboutForm;
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
    avgTimeButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    avgTimeButton->setFont(buttonFont);
    connect(avgTimeButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    adFilterButton = new QPushButton(this);
    adFilterButton->setGeometry(75, 100, 155, 30);
    adFilterButton->setText("Adaptive Filter");
    adFilterButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    adFilterButton->setFont(buttonFont);
    connect(adFilterButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    noCalButton = new QPushButton(this);
    noCalButton->setGeometry(75, 135, 155, 30);
    noCalButton->setText("NO Cal");
    noCalButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    noCalButton->setFont(buttonFont);
    connect(noCalButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    flowCalButton = new QPushButton(this);
    flowCalButton->setGeometry(75, 170, 155, 30);
    flowCalButton->setText("Flow Cal");
    flowCalButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    flowCalButton->setFont(buttonFont);
    connect(flowCalButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    modbusButton = new QPushButton(this);
    modbusButton->setGeometry(75, 205, 155, 30);
    modbusButton->setText("Modbus");
    modbusButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    modbusButton->setFont(buttonFont);
    connect(modbusButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    sysSetButton = new QPushButton(this);
    sysSetButton->setGeometry(75, 240, 155, 30);
    sysSetButton->setText("System");
    sysSetButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    sysSetButton->setFont(buttonFont);
    connect(sysSetButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    // Right side
    modeButton = new QPushButton(this);
    modeButton->setGeometry(250, 65, 155, 30);
    modeButton->setText("Mode");
    modeButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    modeButton->setFont(buttonFont);
    connect(modeButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    dateTimeButton = new QPushButton(this);
    dateTimeButton->setGeometry(250, 100, 155, 30);
    dateTimeButton->setText("Date Time");
    dateTimeButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    dateTimeButton->setFont(buttonFont);
    connect(dateTimeButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    no2CalButton = new QPushButton(this);
    no2CalButton->setGeometry(250, 135, 155, 30);
    no2CalButton->setText("NO2 Cal");
    no2CalButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    no2CalButton->setFont(buttonFont);
    connect(no2CalButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    dataFileButton = new QPushButton(this);
    dataFileButton->setGeometry(250, 170, 155, 30);
    dataFileButton->setText("Data files");
    dataFileButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    dataFileButton->setFont(buttonFont);
    connect(dataFileButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    staticIPButton = new QPushButton(this);
    staticIPButton->setGeometry(250, 205, 155, 30);
    staticIPButton->setText("Static IP");
    staticIPButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    staticIPButton->setFont(buttonFont);
    connect(staticIPButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    aboutButton = new QPushButton(this);
    aboutButton->setGeometry(250, 240, 155, 30);
    aboutButton->setText("About");
    aboutButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    aboutButton->setFont(buttonFont);
    connect(aboutButton, &QPushButton::clicked, this, &SettingsMenu::OnPageButtonClicked);

    pageButtons << avgTimeButton << modeButton << adFilterButton << dateTimeButton << noCalButton << no2CalButton << flowCalButton << dataFileButton << modbusButton << staticIPButton << sysSetButton << aboutButton;
}

void SettingsMenu::OnPageButtonClicked()
{
    int index = pageButtons.indexOf(dynamic_cast<QPushButton*>(sender()));

    //qDebug() << "Page index: " << index;
    if (index > -1 && index < pages.count())
    {
        JumpToPage(index + 1);
    }
}

void SettingsMenu::JumpToPage(int index)
{
    pages[currentPageIndex]->hide();
    pages[index]->show();
    currentPageIndex = index;
}

void SettingsMenu::OnPageMoveLeft()
{
    pages[currentPageIndex]->hide();
    currentPageIndex = currentPageIndex - 1 < 0 ? pages.count() - 1 : currentPageIndex - 1;
    pages[currentPageIndex]->show();
}

void SettingsMenu::OnPageMoveRight()
{
    pages[currentPageIndex]->hide();
    currentPageIndex = currentPageIndex + 1 < pages.count() ? currentPageIndex + 1 : 0;
    pages[currentPageIndex]->show();
}

void SettingsMenu::OnPageClose()
{
    if (currentPageIndex != 0)
    {
        JumpToPage(0);
    }
    else
    {
        emit CloseMenu();
    }
}
