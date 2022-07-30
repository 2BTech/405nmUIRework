#ifndef DATETIMEFORM_H
#define DATETIMEFORM_H

#include <QDateTime>

#include "../basesettingspage.h"

class DateTimeForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    DateTimeForm();
    ~DateTimeForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

private slots:
    void OnDayClicked();
    void OnMonthClicked();
    void OnYearClicked();

    void OnHourClicked();
    void OnMinClicked();
    void OnSecClicked();

    void OnDecreaseClicked();
    void OnIncreaseClicked();

private:
    QLabel* dateFormatLabel = Q_NULLPTR;
    QLabel* timeFormatLabel = Q_NULLPTR;

    QPair<QPushButton*,QLabel*> dayPair;
    QLabel* dayMonthDivider = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> monthPair;
    QLabel* monthYearDivider = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> yearPair;

    QPair<QPushButton*,QLabel*> hourPair;
    QLabel* hourMinDivider = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> minPair;
    QLabel* minSecDivider = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> secPair;

    QPushButton* increaseButton = Q_NULLPTR;
    QPushButton* decreaseButton = Q_NULLPTR;

    QDateTime dateTime;

    int editIndex = -1;

    void UpdateUI() override;

    void showEvent(QShowEvent* event) override;
};

#endif // DATETIMEFORM_H
