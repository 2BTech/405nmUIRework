#include "parametersform.h"

ParametersForm::ParametersForm() : QWidget()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);

    connect(DataHandler::GetInstance(), &DataHandler::ParsedDataLine, this, &ParametersForm::UpdateUI);
}

ParametersForm::~ParametersForm()
{
    if (leftButton)
    {
        leftButton->deleteLater();
        rightButton->deleteLater();
        closeButton->deleteLater();

        headerLabel->deleteLater();

        logLabel->deleteLater();
        noLabel->deleteLater();
        no2Label->deleteLater();
        noxLabel->deleteLater();
        cellTempLabel->deleteLater();

        cellPressLebal->deleteLater();
        cellFlowLabel->deleteLater();
        ozoneFlowLabel->deleteLater();
        pdv1Label->deleteLater();
        pdv2Label->deleteLater();

        leftButton = rightButton = closeButton = Q_NULLPTR;

        headerLabel = logLabel = noLabel = no2Label = noxLabel = cellTempLabel = cellPressLebal = cellFlowLabel = ozoneFlowLabel = pdv1Label = pdv2Label = Q_NULLPTR;
    }
}

void ParametersForm::BuildUIElements()
{
    QFont font;
    font.setPointSize(20);

    headerLabel = new QLabel(this);
    headerLabel->setGeometry(0, 10, 480, 50);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setText("Data 1");
    headerLabel->setFont(font);

    leftButton = new QPushButton(this);
    leftButton->setGeometry(10, 10, 50, 50);
    leftButton->setText("<");
    leftButton->setFont(font);
    leftButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a)");
    connect(leftButton, &QPushButton::clicked, this, &ParametersForm::OnMoveLeftClicked);

    rightButton = new QPushButton(this);
    rightButton->setGeometry(360, 10, 50, 50);
    rightButton->setText(">");
    rightButton->setFont(font);
    rightButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a)");
    connect(rightButton, &QPushButton::clicked, this, &ParametersForm::OnMoveRightClicked);

    font.setPointSize(11);
    closeButton = new QPushButton(this);
    closeButton->setGeometry(420, 10, 50, 50);
    closeButton->setText("Close");
    closeButton->setFont(font);
    closeButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a)");
    connect(closeButton, &QPushButton::clicked, this, &ParametersForm::CloseForm);

    topLine = new QFrame(this);
    topLine->setGeometry(0, 60, 480, 30);
    topLine->setFrameShape(QFrame::HLine);

    firstSep = new QFrame(this);
    firstSep->setGeometry(0, 100, 480, 30);
    firstSep->setFrameShape(QFrame::HLine);

    secSep = new QFrame(this);
    secSep->setGeometry(0, 140, 480, 30);
    secSep->setFrameShape(QFrame::HLine);

    thirdSep = new QFrame(this);
    thirdSep->setGeometry(0, 180, 480, 30);
    thirdSep->setFrameShape(QFrame::HLine);

    fourthSep = new QFrame(this);
    fourthSep->setGeometry(0, 220, 480, 30);
    fourthSep->setFrameShape(QFrame::HLine);

    middleLine = new QFrame(this);
    middleLine->setGeometry(230, 75, 20, 217);
    middleLine->setFrameShape(QFrame::VLine);

    font.setPointSize(12);

    logLabel = new QLabel(this);
    logLabel->setGeometry(10, 75, 230,40);
    logLabel->setFont(font);

    noLabel = new QLabel(this);
    noLabel->setGeometry(10, 115, 230,40);
    noLabel->setFont(font);

    no2Label = new QLabel(this);
    no2Label->setGeometry(10, 155, 230,40);
    no2Label->setFont(font);

    noxLabel = new QLabel(this);
    noxLabel->setGeometry(10, 195, 230,40);
    noxLabel->setFont(font);

    cellTempLabel = new QLabel(this);
    cellTempLabel->setGeometry(10, 235, 230,40);
    cellTempLabel->setFont(font);

    cellPressLebal = new QLabel(this);
    cellPressLebal->setGeometry(250, 75, 230,40);
    cellPressLebal->setFont(font);

    cellFlowLabel = new QLabel(this);
    cellFlowLabel->setGeometry(250, 115, 230,40);
    cellFlowLabel->setFont(font);

    ozoneFlowLabel = new QLabel(this);
    ozoneFlowLabel->setGeometry(250, 155, 230,40);
    ozoneFlowLabel->setFont(font);

    pdv1Label = new QLabel(this);
    pdv1Label->setGeometry(250, 195, 230,40);
    pdv1Label->setFont(font);

    pdv2Label = new QLabel(this);
    pdv2Label->setGeometry(250, 235, 230,40);
    pdv2Label->setFont(font);
}

void ParametersForm::OnMoveLeftClicked()
{
    isDisplayingFirst = !isDisplayingFirst;
    UpdateUI();
}

void ParametersForm::OnMoveRightClicked()
{
    isDisplayingFirst = !isDisplayingFirst;
    UpdateUI();
}

void ParametersForm::UpdateUI()
{
    if (isVisible())
    {
        if (isDisplayingFirst)
        {
            DisplayFirst();
        }
        else
        {
            DisplaySecond();
        }
    }
}

void ParametersForm::DisplayFirst()
{
    DataHandler* dataHandler = DataHandler::GetInstance();

    headerLabel->setText("Parameters 1");

    logLabel->setText("Log #: " + QString::number(dataHandler->GetLogNumber()->getValue()));
    noLabel->setText("NO: " + QString::number(dataHandler->GetNO()->getValue()) + " ppb");
    no2Label->setText("NO<sub>2</sub>: " + QString::number(dataHandler->GetNO2()->getValue()) + " ppb");
    noxLabel->setText("NO<sub>x</sub>: " + QString::number(dataHandler->GetNOx()->getValue()) + " ppb");
    cellTempLabel->setText("Cell Temp: " + QString::number(dataHandler->GetCellTemp()->getValue()) + " <sup>o</sup>C");

    cellPressLebal->setText("Cell Press: " + QString::number(dataHandler->GetCellPress()->getValue()) + " mbar");
    cellFlowLabel->setText("Cell Flow: " + QString::number(dataHandler->GetCellFlow()->getValue()) + " ccm");
    ozoneFlowLabel->setText("Ozone Flow: " + QString::number(dataHandler->GetOzoneFlow()->getValue()) + " ccm");
    pdv1Label->setText("PDV1: " + QString::number(dataHandler->GetPDV1()->getValue()) + " V");
    pdv2Label->setText("PDV2: " + QString::number(dataHandler->GetPDV2()->getValue()) + " V");
}

void ParametersForm::DisplaySecond()
{
    DataHandler* dataHandler = DataHandler::GetInstance();

    headerLabel->setText("Parameters 2");

    logLabel->setText("Scrubber Temp: " + QString::number(dataHandler->GetScrubberTemp()->getValue()) + " <sup>o</sup>C");
    noLabel->setText("Date: " + dataHandler->GetDate()->getValue());
    no2Label->setText("Mode: " + QString::number(dataHandler->GetMode()->getValue()));
    noxLabel->setText("");
    cellTempLabel->setText("");

    cellPressLebal->setText("Error Byte: " + QString::number(dataHandler->GetErrorByte()->getValue()));
    cellFlowLabel->setText("Time: " + dataHandler->GetTime()->getValue());
    ozoneFlowLabel->setText("");
    pdv1Label->setText("");
    pdv2Label->setText("");
}

void ParametersForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    UpdateUI();
}
