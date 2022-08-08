#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(480, 277);

    BuildUIElements();

    //qDebug() << "Built Main Window UI";

    settingsMenu = new SettingsMenu();
    settingsMenu->BuildUIElements();
    connect(settingsMenu, &SettingsMenu::CloseMenu, this, &MainWindow::show);
    connect(settingsMenu, &SettingsMenu::CloseMenu, settingsMenu, &SettingsMenu::hide);

    //qDebug() << "Built Settings Menu";

    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    //qDebug() << "Set Main Window background";

    parameters = new ParametersForm();
    parameters->BuildUIElements();
    connect(parameters, &ParametersForm::CloseForm, this, &MainWindow::show);
    connect(parameters, &ParametersForm::CloseForm, parameters, &ParametersForm::close);

    //qDebug() << "Built Parameters";

    graphForm = new GraphForm();
    connect(graphForm, &GraphForm::CloseForm, this, &MainWindow::show);
    connect(graphForm, &GraphForm::CloseForm, graphForm, &QWidget::close);
    graphForm->AddValueObjects(DataHandler::GetInstance()->GetAllValues());

    //qDebug() << "Built Graph";
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
        modeMask->deleteLater();

        avgStateMask = errMask = modeMask = Q_NULLPTR;
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

    updateTimer.setSingleShot(false);
    updateTimer.setInterval(500);
    connect(&updateTimer, &QTimer::timeout, this, &MainWindow::UpdateUI);
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

    modeMask = new QPushButton(this);
    modeMask->setGeometry(5, 35, 130, 30);
    modeMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(modeMask, &QPushButton::clicked, this, &MainWindow::OnModeMaskClicked);

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
    avgStateMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(avgStateMask, &QPushButton::clicked, this, &MainWindow::OnAvgMaskClicked);

    errLabel = new QLabel(this);
    errLabel->setGeometry(5, 180, 130, 30);
    errLabel->setFont(font);
    errLabel->setText("No Errors");

    errMask = new QPushButton(this);
    errMask->setGeometry(5, 180, 130, 30);
    errMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
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
    settingsMenu->OpenPage(1);
    hide();
}

void MainWindow::OnModeMaskClicked()
{
    settingsMenu->OpenPage(2);
    hide();
}

void MainWindow::OnErrMaskClicked()
{
    if (DataHandler::GetInstance()->GetErrorByte()->getValue())
    {
        QMessageBox::warning(this, "Error", DataHandler::GetInstance()->GetErrorString());
    }
    else
    {
        QMessageBox::information(this, "No Error", "No Errors");
    }
}

void MainWindow::OnSettingsClicked()
{
    settingsMenu->show();
    hide();
}

void MainWindow::OnCloseMenu()
{
    settingsMenu->hide();
    show();
}

void MainWindow::OnParametersClicked()
{
    parameters->show();
    close();
}

void MainWindow::OnGraphClicked()
{
    graphForm->show();
    close();
}

void MainWindow::UpdateUI()
{
    DataHandler* dataHander = DataHandler::GetInstance();
    switch (dataHander->GetMode()->getValue())
    {
    case 0:
        loggingLabel->setText("Warming Up");
        break;

    case 1:
        loggingLabel->setText("NO");
        break;

    case 2:
        loggingLabel->setText("NO2");
        break;

    case 3:
        loggingLabel->setText("NO and NO2");
        break;

    default:
        loggingLabel->setText("ERROR: " + QString::number(dataHander->GetMode()->getValue()));
        break;
    }

    logLabel->setText("Log #: " + dataHander->GetLogNumber()->ToString());

    switch (dynamic_cast<ValueObject<uchar>*>(SettingsHandler::GetInstance()->GetSetting("A"))->getValue())
    {
    case 1:
        avgStateLabel->setText("Avg: 5 secs");
        break;

    case 2:
        avgStateLabel->setText("Avg: 1 min");
        break;

    case 3:
        avgStateLabel->setText("Avg: 5 mins");
        break;

    case 4:
        avgStateLabel->setText("Avg: 1 hour");
        break;

    default:
        avgStateLabel->setText("ERROR");
        break;
    }

    if (dataHander->GetErrorByte()->getValue())
    {
        errLabel->setText("<font color='red'>ERROR</font>");
    }
    else
    {
        errLabel->setText("<font color='green'>No Errors</font>");
    }

    noLabel->setText("NO: " + dataHander->GetNO()->ToString() + " ppb");
    no2Label->setText("NO2: " + dataHander->GetNO2()->ToString() + " ppb");
    noxLabel->setText("NOx: " + dataHander->GetNOx()->ToString() + " ppb");

    dateTimeLabel->setText(QDateTime::currentDateTime().toString("dd/MM/yy hh:mm:ss"));
}

void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    updateTimer.start();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);
    updateTimer.stop();
}
