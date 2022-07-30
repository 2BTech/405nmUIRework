#include "datetimeform.h"

DateTimeForm::DateTimeForm() : BaseSettingsPage("Date Time")
{

}

DateTimeForm::~DateTimeForm()
{

}

void DateTimeForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    QFont font;
    font.setPointSize(15);

    dateFormatLabel = new QLabel(this);
    dateFormatLabel->setGeometry(70, 70, 340, 40);
    dateFormatLabel->setText("(DD/MM/YY)");
    dateFormatLabel->setAlignment(Qt::AlignCenter);

    dayPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));
    dayPair.second->setText("DD");
    dayPair.second->setGeometry(150, 100, 50, 50);
    dayPair.second->setFont(font);

    dayPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    dayPair.first->setGeometry(150, 100, 50, 50);
    connect(dayPair.first, &QPushButton::clicked, this, &DateTimeForm::OnDayClicked);

    dayMonthDivider = new QLabel(this);
    dayMonthDivider->setText("/");
    dayMonthDivider->setGeometry(200, 100, 10, 50);
    dayMonthDivider->setFont(font);

    monthPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));
    monthPair.second->setText("MM");
    monthPair.second->setGeometry(220, 100, 50, 50);
    monthPair.second->setFont(font);

    monthPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    monthPair.first->setGeometry(220, 100, 50, 50);
    connect(monthPair.first, &QPushButton::clicked, this, &DateTimeForm::OnMonthClicked);

    monthYearDivider = new QLabel(this);
    monthYearDivider->setText("/");
    monthYearDivider->setGeometry(270, 100, 10, 50);
    monthYearDivider->setFont(font);

    yearPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));
    yearPair.second->setText("YY");
    yearPair.second->setGeometry(290, 100, 50, 50);
    yearPair.second->setFont(font);

    yearPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    yearPair.first->setGeometry(290, 100, 50, 50);
    connect(yearPair.first, &QPushButton::clicked, this, &DateTimeForm::OnYearClicked);

    timeFormatLabel = new QLabel(this);
    timeFormatLabel->setGeometry(70, 140, 340, 40);
    timeFormatLabel->setText("(HH/MM/SS)");
    timeFormatLabel->setAlignment(Qt::AlignCenter);

    hourPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));
    hourPair.second->setText("HH");
    hourPair.second->setGeometry(150, 170, 50, 50);
    hourPair.second->setFont(font);

    hourPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    hourPair.first->setGeometry(150, 170, 50, 50);
    connect(hourPair.first, &QPushButton::clicked, this, &DateTimeForm::OnHourClicked);

    hourMinDivider = new QLabel(this);
    hourMinDivider->setText(":");
    hourMinDivider->setGeometry(200, 170, 10, 50);
    hourMinDivider->setFont(font);

    minPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));
    minPair.second->setText("MM");
    minPair.second->setGeometry(220, 170, 50, 50);
    minPair.second->setFont(font);

    minPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    minPair.first->setGeometry(220, 170, 50, 50);
    connect(minPair.first, &QPushButton::clicked, this, &DateTimeForm::OnMinClicked);

    minSecDivider = new QLabel(this);
    minSecDivider->setText(":");
    minSecDivider->setGeometry(270, 170, 10, 50);
    minSecDivider->setFont(font);

    secPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));
    secPair.second->setText("SS");
    secPair.second->setGeometry(290, 170, 50, 50);
    secPair.second->setFont(font);

    secPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    secPair.first->setGeometry(290, 170, 50, 50);
    connect(secPair.first, &QPushButton::clicked, this, &DateTimeForm::OnSecClicked);

    increaseButton = new QPushButton(this);
    increaseButton->setGeometry(355, 105, 40, 40);
    increaseButton->setText("+");
    increaseButton->setFont(font);
    increaseButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(increaseButton, &QPushButton::clicked, this, &DateTimeForm::OnIncreaseClicked);

    decreaseButton = new QPushButton(this);
    decreaseButton->setGeometry(355, 175, 40, 40);
    decreaseButton->setText("-");
    decreaseButton->setFont(font);
    decreaseButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(decreaseButton, &QPushButton::clicked, this, &DateTimeForm::OnDecreaseClicked);
}

void DateTimeForm::OnDayClicked()
{
    editIndex = 0;
    UpdateUI();
}

void DateTimeForm::OnMonthClicked()
{
    editIndex = 1;
    UpdateUI();
}

void DateTimeForm::OnYearClicked()
{
    editIndex = 2;
    UpdateUI();
}

void DateTimeForm::OnHourClicked()
{
    editIndex = 3;
    UpdateUI();
}

void DateTimeForm::OnMinClicked()
{
    editIndex = 4;
    UpdateUI();
}

void DateTimeForm::OnSecClicked()
{
    editIndex = 5;
    UpdateUI();
}

void DateTimeForm::OnIncreaseClicked()
{
    switch (editIndex)
    {
    case 0:
        dateTime = dateTime.addDays(1);
        break;

    case 1:
        dateTime = dateTime.addMonths(1);
        break;

    case 2:
        dateTime = dateTime.addYears(1);
        break;

    case 3:
        dateTime = dateTime.addSecs(3600);
        break;

    case 4:
        dateTime = dateTime.addSecs(60);
        break;

    case 5:
        dateTime = dateTime.addSecs(1);
        break;

    default:
        break;
    }
    UpdateUI();
}

void DateTimeForm::OnDecreaseClicked()
{
    switch (editIndex)
    {
    case 0:
        dateTime = dateTime.addDays(-1);
        break;

    case 1:
        dateTime = dateTime.addMonths(-1);
        break;

    case 2:
        dateTime = dateTime.addYears(-1);
        break;

    case 3:
        dateTime = dateTime.addSecs(-3600);
        break;

    case 4:
        dateTime = dateTime.addSecs(-60);
        break;

    case 5:
        dateTime = dateTime.addSecs(-1);
        break;

    default:
        break;
    }
    UpdateUI();
}

void DateTimeForm::UpdateUI()
{
    dayPair.second->setText(dateTime.toString("dd"));
    monthPair.second->setText(dateTime.toString("MM"));
    yearPair.second->setText(dateTime.toString("yy"));

    hourPair.second->setText(dateTime.toString("hh"));
    minPair.second->setText(dateTime.toString("mm"));
    secPair.second->setText(dateTime.toString("ss"));

    switch (editIndex)
    {
    case 0:
        dayPair.second->setText(QString("<font color='blue'>").append(dayPair.second->text()).append("</font"));
        break;

    case 1:
        monthPair.second->setText(QString("<font color='blue'>").append(monthPair.second->text()).append("</font"));
        break;

    case 2:
        yearPair.second->setText(QString("<font color='blue'>").append(yearPair.second->text()).append("</font"));
        break;

    case 3:
        hourPair.second->setText(QString("<font color='blue'>").append(hourPair.second->text()).append("</font"));
        break;

    case 4:
        minPair.second->setText(QString("<font color='blue'>").append(minPair.second->text()).append("</font"));
        break;

    case 5:
        secPair.second->setText(QString("<font color='blue'>").append(secPair.second->text()).append("</font"));
        break;

    default:
        break;
    }
}

void DateTimeForm::showEvent(QShowEvent* event)
{
   BasePage::showEvent(event);

   dateTime = QDateTime::currentDateTime();
   editIndex = -1;
   UpdateUI();
}
