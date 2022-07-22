#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(480, 277);

    BuildUIElements();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BuildUIElements()
{
    // Build the lines
    statusLine = new QFrame(this);
    statusLine->setGeometry(130, 0, 20, 277);
    statusLine->setFrameShape(QFrame::VLine);
    statusLine->show();

    navigationLine = new QFrame(this);
    navigationLine->setGeometry(0, 210, 480, 20);
    navigationLine->setFrameShape(QFrame::HLine);
    navigationLine->show();

    navSepLine = new QFrame(this);
    navSepLine->setGeometry(300, 220, 20, 57);
    navSepLine->setFrameShape(QFrame::VLine);
    navSepLine->show();
}
