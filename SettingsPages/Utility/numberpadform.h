#ifndef NUMBERPADFORM_H
#define NUMBERPADFORM_H

#include <QGraphicsView>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QDebug>

#include "ValueHandlers/ValueObjects/basevalueobject.h"

enum class NumberPadMode {
    GET_INT,
    GET_FLOAT,
    GET_STRING,
    GET_IPADDRESS
};

class NumberPadForm : public QGraphicsView
{
    Q_OBJECT
public:
    static NumberPadForm* GetInstance();

    void Start(QWidget* opener, NumberPadMode mode, double min = 0, double max = 0);

signals:
    void Finished(QString result);

private slots:
    void OnClearClicked();
    void OnBackClicked();
    void OnNumberClicked();
    void OnToggleNegClicked();
    void OnDotClicked();
    void OnCancelClicked();
    void OnSaveClicked();

private:
    NumberPadForm();
    ~NumberPadForm();

    static NumberPadForm* instance;

    QTextEdit* textHolder = Q_NULLPTR;

    QLabel* rangeLabel = Q_NULLPTR;

    QPushButton* zeroButton = Q_NULLPTR;
    QPushButton* oneButton = Q_NULLPTR;
    QPushButton* twoButton = Q_NULLPTR;
    QPushButton* threeButton = Q_NULLPTR;
    QPushButton* fourButton = Q_NULLPTR;
    QPushButton* fiveButton = Q_NULLPTR;
    QPushButton* sixButton = Q_NULLPTR;
    QPushButton* sevenButton = Q_NULLPTR;
    QPushButton* eightButton = Q_NULLPTR;
    QPushButton* nineButton = Q_NULLPTR;

    QPushButton* clearButton = Q_NULLPTR;
    QPushButton* backButton = Q_NULLPTR;

    QPushButton* saveButton = Q_NULLPTR;
    QPushButton* cancelButton = Q_NULLPTR;

    QPushButton* toggleNegButton = Q_NULLPTR;
    QPushButton* dotButton = Q_NULLPTR;

    char textBuffer[50];
    int length = 0;

    QWidget* opener = Q_NULLPTR;
    double min;
    double max;

    NumberPadMode mode;

    void AddDigit(char text);

    void ShiftBufferForward();
    void ShiftBufferBack();

    bool Contains(char chr);
    int GetCount(char chr);

    void BuildUIElements();

};

#endif // NUMBERPADFORM_H
