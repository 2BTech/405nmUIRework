#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(480, 277);

    BuildUIElements();

    settingsMenu = new SettingsMenu();
    settingsMenu->BuildUIElements();

    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
}

MainWindow::~MainWindow()
{
    delete ui;

    // Clean up lines
    if (statusLine)
    {
        statusLine->deleteLater();
        navSepLine->deleteLater();
        navSepLine->deleteLater();

        statusLine = navSepLine = navigationLine = Q_NULLPTR;
    }

    // Clean up status bar objects
    if (statusLabel)
    {
        statusLabel->deleteLater();
        loggingLabel->deleteLater();
        logLabel->deleteLater();
        avgStateLabel->deleteLater();
        errLabel->deleteLater();

        statusLabel = loggingLabel = logLabel = avgStateLabel = errLabel = Q_NULLPTR;

        avgStateMask->deleteLater();
        errMask->deleteLater();

        avgStateMask = errMask = Q_NULLPTR;
    }

    // Clean up nav bar objects
    if (settingsButton)
    {
        settingsButton->deleteLater();
        parametersButton->deleteLater();
        graphButton->deleteLater();

        settingsButton = parametersButton = graphButton = Q_NULLPTR;
    }

    // Clean up data display objects
    if (noLabel)
    {
        noLabel->deleteLater();
        no2Label->deleteLater();
        noxLabel->deleteLater();
        dateTimeLabel->deleteLater();

        noLabel = no2Label = noxLabel = dateTimeLabel = Q_NULLPTR;
    }
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

    BuildStatusBar();
    BuildNavBar();
    BuildDataLabels();
}

void MainWindow::BuildStatusBar()
{
    QFont font;
    font.setPointSize(15);

    statusLabel = new QLabel(this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setGeometry(0, 5, 130, 30);
    statusLabel->setFont(font);
    statusLabel->setText("Status");

    font.setPointSize(9);

    loggingLabel = new QLabel(this);
    loggingLabel->setGeometry(5, 35, 130, 30);
    loggingLabel->setFont(font);
    loggingLabel->setText("Logging");

    logLabel = new QLabel(this);
    logLabel->setGeometry(5, 65, 130, 30);
    logLabel->setFont(font);
    logLabel->setText("Log: 123");

    avgStateLabel = new QLabel(this);
    avgStateLabel->setGeometry(5, 95, 130, 30);
    avgStateLabel->setFont(font);
    avgStateLabel->setText("Avg State: 5 Secs");

    avgStateMask = new QPushButton(this);
    avgStateMask->setGeometry(5, 95, 130, 30);
    avgStateMask->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");
    connect(avgStateMask, &QPushButton::clicked, this, &MainWindow::OnAvgMaskClicked);

    errLabel = new QLabel(this);
    errLabel->setGeometry(5, 180, 130, 30);
    errLabel->setFont(font);
    errLabel->setText("No Errors");

    errMask = new QPushButton(this);
    errMask->setGeometry(5, 180, 130, 30);
    errMask->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");
    connect(errMask, &QPushButton::clicked, this, &MainWindow::OnErrMaskClicked);
}

void MainWindow::BuildNavBar()
{
    QFont font;
    font.setBold(true);
    font.setPointSize(12);

    settingsButton = new QPushButton(this);
    settingsButton->setGeometry(5, 225, 130, 47);
    settingsButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    settingsButton->setText("Settings");
    settingsButton->setFont(font);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::OnSettingsClicked);

    parametersButton = new QPushButton(this);
    parametersButton->setGeometry(145, 225, 160, 47);
    parametersButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    parametersButton->setText("Parameters");
    parametersButton->setFont(font);
    connect(parametersButton, &QPushButton::clicked, this, &MainWindow::OnParametersClicked);

    graphButton = new QPushButton(this);
    graphButton->setGeometry(315, 225, 160, 47);
    graphButton->setStyleSheet(QString("background-color: ").append("qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);"));
    graphButton->setText("Graph");
    graphButton->setFont(font);
    connect(graphButton, &QPushButton::clicked, this, &MainWindow::OnGraphClicked);
}

void MainWindow::BuildDataLabels()
{
    QFont font;
    font.setPointSize(23);

    noLabel = new QLabel(this);
    noLabel->setGeometry(160, 10, 320, 50);
    noLabel->setText("NO: 0.0 ppb");
    noLabel->setFont(font);

    no2Label = new QLabel(this);
    no2Label->setGeometry(160, 60, 320, 50);
    no2Label->setText("NO2: 0.0 ppb");
    no2Label->setFont(font);

    noxLabel = new QLabel(this);
    noxLabel->setGeometry(160, 110, 320, 50);
    noxLabel->setText("NOx: 0.0 ppb");
    noxLabel->setFont(font);

    font.setPointSize(12);

    dateTimeLabel = new QLabel(this);
    dateTimeLabel->setGeometry(160, 170, 320, 50);
    dateTimeLabel->setAlignment(Qt::AlignCenter);
    dateTimeLabel->setText("dd/MM/yy hh:mm:ss");
    dateTimeLabel->setFont(font);
}

void MainWindow::OnAvgMaskClicked()
{
    qDebug() << "Clicked on avg state mask";
}

void MainWindow::OnErrMaskClicked()
{
    qDebug() << "Clicked on err mask";
}

void MainWindow::OnSettingsClicked()
{
    settingsMenu->show();
}

void MainWindow::OnParametersClicked()
{
    qDebug() << "Clicked on parameters";
}

void MainWindow::OnGraphClicked()
{
    qDebug() << "Clicked on graph";
}
