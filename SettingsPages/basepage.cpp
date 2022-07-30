#include "basepage.h"

BasePage::BasePage(QString pageName) : QWidget()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);

    this->pageName = pageName;
}

BasePage::~BasePage()
{
    // Clean common ui elements
    if (title)
    {
        title->deleteLater();

        title = Q_NULLPTR;

        closeButton->deleteLater();
        leftMoveButton->deleteLater();
        rightMoveButton->deleteLater();

        closeButton = leftMoveButton = rightMoveButton = Q_NULLPTR;
    }
}

void BasePage::BuildUIElements()
{
    QFont titleFont;
    //titleFont.setBold(true);
    titleFont.setPointSize(25);

    title = new QLabel(this);
    title->setGeometry(0, 0, 480, 60);
    title->setText(pageName);
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->setStyleSheet("QLabel { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");

    closeButton = new QPushButton(this);
    closeButton->setGeometry(420, 5, 50, 50);
    closeButton->setText("Close");
    closeButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    QFont closeFont;
    closeFont.setPointSize(10);
    closeButton->setFont(closeFont);
    connect(closeButton, &QPushButton::clicked, this, &BasePage::OnCloseClicked);

    leftMoveButton = new QPushButton(this);
    leftMoveButton->setGeometry(10, 70, 50, 200);
    leftMoveButton->setText("<");
    leftMoveButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    leftMoveButton->setFont(titleFont);
    connect(leftMoveButton, &QPushButton::clicked, this, &BasePage::OnMoveLeftClicked);

    rightMoveButton = new QPushButton(this);
    rightMoveButton->setGeometry(420, 70, 50, 200);
    rightMoveButton->setText(">");
    rightMoveButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    rightMoveButton->setFont(titleFont);
    connect(rightMoveButton, &QPushButton::clicked, this, &BasePage::OnMoveRightClikced);
}

void BasePage::OnMoveLeftClicked()
{
    emit MoveLeft();
}

void BasePage::OnMoveRightClikced()
{
    emit MoveRight();
}

void BasePage::OnCloseClicked()
{
    emit ClosePage();
}
