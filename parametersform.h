#ifndef PARAMETERSFORM_H
#define PARAMETERSFORM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFrame>

#include "ValueHandlers/datahandler.h"

class ParametersForm : public QWidget
{
    Q_OBJECT
public:
    ParametersForm();
    ~ParametersForm();

    void BuildUIElements();

signals:
    void CloseForm();

private slots:
    void OnMoveLeftClicked();
    void OnMoveRightClicked();
    void UpdateUI();

private:
    QPushButton* leftButton = Q_NULLPTR;
    QPushButton* rightButton = Q_NULLPTR;
    QPushButton* closeButton = Q_NULLPTR;

    QLabel* headerLabel = Q_NULLPTR;

    QLabel* logLabel = Q_NULLPTR;
    QLabel* noLabel = Q_NULLPTR;
    QLabel* no2Label = Q_NULLPTR;
    QLabel* noxLabel = Q_NULLPTR;
    QLabel* cellTempLabel = Q_NULLPTR;

    QLabel* cellPressLebal = Q_NULLPTR;
    QLabel* cellFlowLabel = Q_NULLPTR;
    QLabel* ozoneFlowLabel = Q_NULLPTR;
    QLabel* pdv1Label = Q_NULLPTR;
    QLabel* pdv2Label = Q_NULLPTR;

    QFrame* topLine = Q_NULLPTR;
    QFrame* middleLine = Q_NULLPTR;
    QFrame* firstSep = Q_NULLPTR;
    QFrame* secSep = Q_NULLPTR;
    QFrame* thirdSep = Q_NULLPTR;
    QFrame* fourthSep = Q_NULLPTR;

    bool isDisplayingFirst = true;

    void DisplayFirst();
    void DisplaySecond();

    void showEvent(QShowEvent* event) override;
};

#endif // PARAMETERSFORM_H
