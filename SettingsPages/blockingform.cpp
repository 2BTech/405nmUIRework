#include "blockingform.h"

BlockingForm::BlockingForm() : QWidget()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);

    loadGif.setFileName(":/loader.gif");
    loadGif.stop();
}

BlockingForm::~BlockingForm()
{
    if (cancelButton)
    {
        cancelButton->deleteLater();

        waitLabel->deleteLater();
        gifLabel->deleteLater();

        cancelButton = Q_NULLPTR;

        waitLabel = gifLabel = Q_NULLPTR;
    }
}

void BlockingForm::BuildUIElements()
{
    QFont font;

    waitLabel = new QLabel(this);
    waitLabel->setText("Please Wait");
    waitLabel->setGeometry(0, 60, 480, 40);
    waitLabel->setAlignment(Qt::AlignCenter);
    font.setPointSize(20);
    waitLabel->setFont(font);

    gifLabel = new QLabel(this);
    gifLabel->setGeometry(215, 110, 70, 70);
    gifLabel->setMovie(&loadGif);

    cancelButton = new QPushButton(this);
    cancelButton->setGeometry(170, 200, 140, 40);
    cancelButton->setText("Cancel");
    cancelButton->setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    font.setPointSize(12);
    cancelButton->setFont(font);
    connect(cancelButton, &QPushButton::clicked, this, &BlockingForm::CancelClicked);
}

void BlockingForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    loadGif.start();
}

void BlockingForm::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);
    loadGif.stop();
}
