#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class BasePage : public QWidget
{
public:
    BasePage(QString pageName);
    ~BasePage();

    virtual void BuildUIElements();

protected:
    QLabel* title = Q_NULLPTR;

    QPushButton* closeButton = Q_NULLPTR;
    QPushButton* leftMoveButton = Q_NULLPTR;
    QPushButton* rightMoveButton = Q_NULLPTR;

    QString pageName = "Unset page name";
};

#endif // BASEPAGE_H
