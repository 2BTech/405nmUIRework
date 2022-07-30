#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class BasePage : public QWidget
{
    Q_OBJECT
public:
    BasePage(QString pageName);
    ~BasePage();

    virtual void BuildUIElements();

signals:
    void ClosePage();
    void MoveLeft();
    void MoveRight();

protected slots:
    virtual void OnMoveLeftClicked();
    virtual void OnMoveRightClikced();
    virtual void OnCloseClicked();

protected:
    QLabel* title = Q_NULLPTR;

    QPushButton* closeButton = Q_NULLPTR;
    QPushButton* leftMoveButton = Q_NULLPTR;
    QPushButton* rightMoveButton = Q_NULLPTR;

    QString pageName = "Unset page name";
};

#endif // BASEPAGE_H
