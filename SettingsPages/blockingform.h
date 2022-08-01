#ifndef BLOCKINGFORM_H
#define BLOCKINGFORM_H

#include <QWidget>
#include <QMovie>
#include <QPushButton>
#include <QLabel>

class BlockingForm : public QWidget
{
    Q_OBJECT
public:
    BlockingForm();
    ~BlockingForm();

    void BuildUIElements();

signals:
    void CancelClicked();

private:
    QMovie loadGif;

    QPushButton* cancelButton = Q_NULLPTR;
    QLabel* waitLabel = Q_NULLPTR;
    QLabel* gifLabel = Q_NULLPTR;

    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
};

#endif // BLOCKINGFORM_H
