#include "rangeform.h"

RangeForm::RangeForm() : QWidget()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);

    BuildUIElements();
}

RangeForm::~RangeForm()
{
    if (title)
    {
        title->deleteLater();
        minXLabel->deleteLater();
        maxXLabel->deleteLater();
        minYLabel->deleteLater();
        maxYLabel->deleteLater();

        minXMask->deleteLater();
        maxXMask->deleteLater();
        minYMask->deleteLater();
        maxYMask->deleteLater();
        saveButton->deleteLater();
        cancelButton->deleteLater();

        autoMinX->deleteLater();
        autoMaxX->deleteLater();
        autoMinY->deleteLater();
        autoMaxY->deleteLater();
        autoAll->deleteLater();

        title = minXLabel = maxXLabel = minYLabel = Q_NULLPTR;

        minXMask = maxXMask = minYMask = maxYMask = saveButton = cancelButton = Q_NULLPTR;

        autoMinX = autoMaxX = autoMinY = autoMaxY = autoAll = Q_NULLPTR;
    }
}

void RangeForm::BuildUIElements()
{
    int tempX = 480 / 20;
    int tempY = 277 / 20;

    QFont tempFont;
    QFont checkFont;

    QFont titleFont;
    titleFont.setPointSize(25);

    //Title
    title = new QLabel(this);
    title->setGeometry(0, 0, 480, 60);
    title->setText("Graph Range");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->setStyleSheet("QLabel { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");

    QFont font;
    font.setPointSize(12);

    // Min X Label
    minXLabel = new QLabel(this);
    minXLabel->setGeometry(tempX, tempY * 7, tempX * 8, tempY * 4);
    minXLabel->setFont(font);
    minXLabel->setText("Min X: 10.00");

    // Min X mask
    minXMask = new QPushButton(this);
    minXMask->setGeometry(tempX, tempY * 7, tempX * 8, tempY * 4);
    minXMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(minXMask, &QPushButton::clicked, this, &RangeForm::OnMinXClicked);

    // Auto Min X
    autoMinX = new QCheckBox(this);
    autoMinX->setGeometry(tempX * 3, tempY * 5, tempX * 6, tempY * 2);
    autoMinX->setText("Auto Min X");
    autoMinX->show();
    checkFont.setPointSize(10);
    autoMinX->setFont(checkFont);
    toggles.append(autoMinX);
    connect(autoMinX, &QCheckBox::clicked, this, &RangeForm::OnToggleClicked);

    // Max X Label
    maxXLabel = new QLabel(this);
    maxXLabel->setGeometry(tempX * 11, tempY * 7, tempX * 8, tempY * 4);
    maxXLabel->setFont(font);
    maxXLabel->setText("Max X: 10.00");

    // Max X Mask
    maxXMask = new QPushButton(this);
    maxXMask->setGeometry(tempX * 11, tempY * 7, tempX * 8, tempY * 4);
    maxXMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(maxXMask, &QPushButton::clicked, this, &RangeForm::OnMaxXClicked);

    //Auto Max X
    autoMaxX = new QCheckBox(this);
    autoMaxX->setGeometry(tempX * 13, tempY * 5, tempX * 6, tempY * 2);
    autoMaxX->setText("Auto Max X");
    autoMaxX->show();
    autoMaxX->setFont(checkFont);
    toggles.append(autoMaxX);
    connect(autoMaxX, &QCheckBox::clicked, this, &RangeForm::OnToggleClicked);

    //Min Y Label
    minYLabel = new QLabel(this);
    minYLabel->setGeometry(tempX, tempY * 13, tempX * 8, tempY * 4);
    minYLabel->setFont(font);
    minYLabel->setText("Min Y: 10.00");

    // Min Y Mask
    minYMask = new QPushButton(this);
    minYMask->setGeometry(tempX, tempY * 13, tempX * 8, tempY * 4);
    minYMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(minYMask, &QPushButton::clicked, this, &RangeForm::OnMinYClicked);

    //Auto Min Y
    autoMinY = new QCheckBox(this);
    autoMinY->setGeometry(tempX * 3, tempY * 11, tempX * 6, tempY * 2);
    autoMinY->setText("Auto Min Y");
    autoMinY->show();
    autoMinY->setFont(checkFont);
    toggles.append(autoMinY);
    connect(autoMinY, &QCheckBox::clicked, this, &RangeForm::OnToggleClicked);

    // Max Y Label
    maxYLabel = new QLabel(this);
    maxYLabel->setGeometry(tempX * 11, tempY * 13, tempX * 8, tempY * 4);
    maxYLabel->setFont(font);
    maxYLabel->setText("Min Y: 10.00");

    // Max Y Mask
    maxYMask = new QPushButton(this);
    maxYMask->setGeometry(tempX * 11, tempY * 13, tempX * 8, tempY * 4);
    maxYMask->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(minYMask, &QPushButton::clicked, this, &RangeForm::OnMaxYClicked);

    //Auto Max Y
    autoMaxY = new QCheckBox(this);
    autoMaxY->setGeometry(tempX * 13, tempY * 11, tempX * 6, tempY * 2);
    autoMaxY->setText("Auto Max Y");
    autoMaxY->show();
    autoMaxY->setFont(checkFont);
    toggles.append(autoMaxY);
    connect(autoMaxY, &QCheckBox::clicked, this, &RangeForm::OnToggleClicked);

    //Toggle all checkbox
    autoAll = new QCheckBox(this);
    autoAll->setGeometry((tempX * 10) - (tempX * 2), tempY * 5, tempX * 4, tempY * 2);
    autoAll->setText("Toggle All");
    autoAll->setChecked(true);
    autoAll->show();
    autoAll->setFont(checkFont);
    toggles.append(autoAll);
    connect(autoAll, &QCheckBox::clicked, this, &RangeForm::OnToggleAllClicked);

    //Save button
    saveButton = new QPushButton(this);
    saveButton->setGeometry((tempX * 10) + (tempX * 1), tempY * 17, tempX * 4, tempY * 2);
    saveButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    saveButton->setText("Save");
    connect(saveButton, &QPushButton::clicked, this, &RangeForm::OnSaveClicked);

    //Cancel button
    cancelButton = new QPushButton(this);
    cancelButton->setGeometry((tempX * 10) - (tempX * 5), tempY * 17, tempX * 4, tempY * 2);
    cancelButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    cancelButton->setText("Cancel");
    connect(cancelButton, &QPushButton::clicked, this, &RangeForm::OnCancelClicked);
}

void RangeForm::SetInformation(QDateTime* pMinX, QDateTime* pMaxX, double *pMinY, double* pMaxY, bool* pAutoMinX, bool* pAutoMaxX, bool* pAutoMinY, bool* pAutoMaxY)
{
    this->pMinX = pMinX;
    this->pMaxX = pMaxX;
    this->pMinY = pMinY;
    this->pMaxY = pMaxY;

    this->pAutoMinX = pAutoMinX;
    this->pAutoMaxX = pAutoMaxX;
    this->pAutoMinY = pAutoMinY;
    this->pAutoMaxY = pAutoMaxY;
}

void RangeForm::OnMinXClicked()
{
    isGettingSetting = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &RangeForm::ReceiveMinX);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_STRING);
}

void RangeForm::ReceiveMinX(QString val)
{
    if (val.length() == 6)
    {
        tempMinX.setTime(QTime(val.left(2).toInt(), val.left(4).right(2).toInt(), val.right(2).toInt()));
    }
}

void RangeForm::OnMaxXClicked()
{
    isGettingSetting = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &RangeForm::ReceiveMaxX);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_STRING);
}

void RangeForm::ReceiveMaxX(QString val)
{
    if (!val.isEmpty())
    {
        tempMaxX.setTime(QTime(val.left(2).toInt(), val.left(4).right(2).toInt(), val.right(2).toInt()));
        UpdateUI();
    }
}

void RangeForm::OnMinYClicked()
{
    isGettingSetting = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &RangeForm::ReceiveMinY);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, -100000, 100000);
}

void RangeForm::ReceiveMinY(QString val)
{
    if (!val.isEmpty())
    {
        tempMinY = val.toFloat();
        UpdateUI();
    }
}

void RangeForm::OnMaxYClicked()
{
    isGettingSetting = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &RangeForm::ReceiveMaxY);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, -100000, 100000);
}

void RangeForm::ReceiveMaxY(QString val)
{
    if (!val.isEmpty())
    {
        tempMaxY = val.toFloat();
        UpdateUI();
    }
}

void RangeForm::OnToggleAllClicked()
{
    //Get current toggle value
    bool newVal = toggles.at(toggles.count() - 1)->checkState() == Qt::CheckState::Checked;
    //Set all toggles to this value
    autoMinX->setChecked(newVal);
    autoMaxX->setChecked(newVal);
    autoMinY->setChecked(newVal);
    autoMaxY->setChecked(newVal);
    autoAll->setChecked(newVal);
}

void RangeForm::OnToggleClicked()
{
    autoAll->setChecked(*pAutoMinX && *pAutoMaxX && *pAutoMinY && *pAutoMaxY);
}

void RangeForm::OnSaveClicked()
{
    qDebug() << ("Set MinX to " + tempMinX.time().toString());
    qDebug() << ("Set MaxX to " + tempMaxX.time().toString());
    *pMinX = tempMinX;
    *pMaxX = tempMaxX;
    *pMinY = tempMinY;
    *pMaxY = tempMaxY;

    *pAutoMinX = toggles.at(0)->checkState() == Qt::CheckState::Checked;
    *pAutoMaxX = toggles.at(1)->checkState() == Qt::CheckState::Checked;
    *pAutoMinY = toggles.at(2)->checkState() == Qt::CheckState::Checked;
    *pAutoMaxY = toggles.at(3)->checkState() == Qt::CheckState::Checked;

    emit UpdateGraph();

    QMessageBox::information(this, "Applying", "Applied new range");

    emit CloseForm();
}

void RangeForm::OnCancelClicked()
{
    emit CloseForm();
}

void RangeForm::UpdateUI()
{
    minXLabel->setText("Min X: " + tempMinX.toString("hh:mm:ss"));
    maxXLabel->setText("Max X: " + tempMaxX.toString("hh:mm:ss"));
    minYLabel->setText("Min Y: " + QString::number(tempMinY));
    maxYLabel->setText("Max Y: " + QString::number(tempMaxY));
}

void RangeForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (!isGettingSetting)
    {
        tempMinX = *pMinX;
        tempMaxX = *pMaxX;
        tempMinY = *pMinY;
        tempMaxY = *pMaxY;
        autoMinX->setChecked(*pAutoMinX);
        autoMaxX->setChecked(*pAutoMaxX);
        autoMinY->setChecked(*pAutoMinY);
        autoMaxY->setChecked(*pAutoMaxY);

        autoAll->setChecked(*pAutoMinX && *pAutoMaxX && *pAutoMinY && *pAutoMaxY);

        UpdateUI();
    }
    else
    {
        isGettingSetting = false;
    }
}
