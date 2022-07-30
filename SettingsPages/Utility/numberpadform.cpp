#include "numberpadform.h"

NumberPadForm::NumberPadForm() : QGraphicsView()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);

    BuildUIElements();
}

NumberPadForm::~NumberPadForm()
{

}

NumberPadForm* NumberPadForm::instance = Q_NULLPTR;
NumberPadForm* NumberPadForm::GetInstance()
{
    if (instance == Q_NULLPTR)
    {
        instance = new NumberPadForm();
    }
    return instance;
}

void NumberPadForm::BuildUIElements()
{
    int tempX = 480 / 20;
    int tempY = 277 / 10;

    QFont font;
    font.setPointSize(11);

    textHolder = new QTextEdit(this);
    textHolder->setGeometry(tempX / 2, tempY / 2, tempX * 6, tempY * 3 / 2);
    textHolder->setText("");
    textHolder->setFontPointSize(11);

    //Range label
    rangeLabel = new QLabel(this); //UIFactory::CreateRegLabel(QRect(tempX * 6 + (tempX / 2), tempY / 2, tempX * 8, tempY * 3 / 2), this);
    rangeLabel->setGeometry(tempX * 6 + (tempX / 2) + 10, tempY / 2, tempX * 8, tempY * 3 / 2);
    rangeLabel->setText("Range: 0 - 100");
    rangeLabel->setFont(font);

    //Clear button
    clearButton = new QPushButton(this);
    clearButton->setGeometry(tempX * 17 + (tempX / 2), tempY / 2, tempY * 3 / 2, tempY * 3 / 2);
    clearButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    clearButton->setText("Clear");
    connect(clearButton, &QPushButton::clicked, this, &NumberPadForm::OnClearClicked);

    //Back button
    backButton = new QPushButton(this);
    backButton->setGeometry(tempX * 15, tempY / 2, tempY * 3 / 2, tempY * 3 / 2);
    backButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    backButton->setText("Back");
    connect(backButton, &QPushButton::clicked, this, &NumberPadForm::OnBackClicked);

    tempX = 480 / 7;

    font.setPointSize(20);

    //0 button
    zeroButton = new QPushButton(this);
    zeroButton->setGeometry((tempX / 2) + (tempX * 0), tempY * 5 / 2, tempY * 2, tempY * 2);
    zeroButton->setText("0");
    zeroButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(zeroButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    zeroButton->setFont(font);

    //1 button
    oneButton = new QPushButton(this);
    oneButton->setGeometry((tempX / 2) + (tempX * 1), tempY * 5 / 2, tempY * 2, tempY * 2);
    oneButton->setText("1");
    oneButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(oneButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    oneButton->setFont(font);

    //2 button
    twoButton = new QPushButton(this);
    twoButton->setGeometry((tempX / 2) + (tempX * 2), tempY * 5 / 2, tempY * 2, tempY * 2);
    twoButton->setText("2");
    twoButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(twoButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    twoButton->setFont(font);

    //3 button
    threeButton = new QPushButton(this);
    threeButton->setGeometry((tempX / 2) + (tempX * 3), tempY * 5 / 2, tempY * 2, tempY * 2);
    threeButton->setText("3");
    threeButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(threeButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    threeButton->setFont(font);

    //4 button
    fourButton = new QPushButton(this);
    fourButton->setGeometry((tempX / 2) + (tempX * 4), tempY * 5 / 2, tempY * 2, tempY * 2);
    fourButton->setText("4");
    fourButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(fourButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    fourButton->setFont(font);

    //+/- button
    toggleNegButton = new QPushButton(this);
    toggleNegButton->setGeometry((tempX / 2) + (tempX * 5), tempY * 5 / 2, tempY * 2, tempY * 2);
    toggleNegButton->setText("+/-");
    toggleNegButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(toggleNegButton, &QPushButton::clicked, this, &NumberPadForm::OnToggleNegClicked);
    toggleNegButton->setFont(font);

    //5 button
    fiveButton = new QPushButton(this);
    fiveButton->setGeometry((tempX / 2) + (tempX * 0), (tempY * 5 / 2) + (tempY * 5 / 2), tempY * 2, tempY * 2);
    fiveButton->setText("5");
    fiveButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(fiveButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    fiveButton->setFont(font);

    //6 button
    sixButton = new QPushButton(this);
    sixButton->setGeometry((tempX / 2) + (tempX * 1), (tempY * 5 / 2) + (tempY * 5 / 2), tempY * 2, tempY * 2);
    sixButton->setText("6");
    sixButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(sixButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    sixButton->setFont(font);

    //7 button
    sevenButton = new QPushButton(this);
    sevenButton->setGeometry((tempX / 2) + (tempX * 2), (tempY * 5 / 2) + (tempY * 5 / 2), tempY * 2, tempY * 2);
    sevenButton->setText("7");
    sevenButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(sevenButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    sevenButton->setFont(font);

    //8 button
    eightButton = new QPushButton(this);
    eightButton->setGeometry((tempX / 2) + (tempX * 3), (tempY * 5 / 2) + (tempY * 5 / 2), tempY * 2, tempY * 2);
    eightButton->setText("8");
    eightButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(eightButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    eightButton->setFont(font);

    //9 Button
    nineButton = new QPushButton(this);
    nineButton->setGeometry((tempX / 2) + (tempX * 4), (tempY * 5 / 2) + (tempY * 5 / 2), tempY * 2, tempY * 2);
    nineButton->setText("9");
    nineButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(nineButton, &QPushButton::clicked, this, &NumberPadForm::OnNumberClicked);
    nineButton->setFont(font);

    // dot button
    dotButton = new QPushButton(this);
    dotButton->setGeometry((tempX / 2) + (tempX * 5), (tempY * 5 / 2) + (tempY * 5 / 2), tempY * 2, tempY * 2);
    dotButton->setText(".");
    dotButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(dotButton, &QPushButton::clicked, this, &NumberPadForm::OnDotClicked);
    dotButton->setFont(font);

    int centerX = 480 / 2;
    tempX = 480 / 10;

    font.setPointSize(15);

    //Cancel button
    cancelButton = new QPushButton(this);
    cancelButton->setGeometry(centerX - (tempX * 5 / 2), tempY * 7 + (tempY / 3), tempX * 2, tempY * 7 / 6);
    cancelButton->setText("Cancel");
    cancelButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(cancelButton, &QPushButton::clicked, this, &NumberPadForm::OnCancelClicked);
    cancelButton->setFont(font);

    //Apply button
    saveButton = new QPushButton(this);
    saveButton->setGeometry(centerX + (tempX / 2), tempY * 7 + (tempY / 3), tempX * 2, tempY * 7 / 6);
    saveButton->setText("Apply");
    saveButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(saveButton, &QPushButton::clicked, this, &NumberPadForm::OnSaveClicked);
    saveButton->setFont(font);

    //Set to readonly to prevent grabbing focus
    textHolder->setReadOnly(true);
}

void NumberPadForm::Start(QWidget* opener, NumberPadMode mode, double min, double max)
{
    this->opener = opener;

    this->mode = mode;
    this->min = min;
    this->max = max;

    memset(textBuffer, 0, 50);
    textHolder->setText(textBuffer);

    if (min != max)
    {
        QString temp  = "Range: ";

        sprintf(textBuffer, "%1.2f ", min);
        temp.append(textBuffer);
        sprintf(textBuffer, "%1.2f", max);
        temp.append(textBuffer);
        rangeLabel->setText(temp);
        memset(textBuffer, 0, 50);
    }
    else
    {
        rangeLabel->setText("");
    }

    length = 0;

    show();
    opener->hide();
}

void NumberPadForm::OnClearClicked()
{
    memset(textBuffer, 0, 50);
    length = 0;
    textHolder->setText(textBuffer);
}

void NumberPadForm::OnBackClicked()
{
    if (length <= 0)
    {
        return;
    }

    length--;
    textBuffer[length] = 0;
    textHolder->setText(textBuffer);
}

void NumberPadForm::OnNumberClicked()
{
    AddDigit(reinterpret_cast<QPushButton*>(sender())->text().at(0).toLatin1());
}

void NumberPadForm::OnToggleNegClicked()
{
    switch (mode)
    {
    case NumberPadMode::GET_STRING:
    case NumberPadMode::GET_INT:
    case NumberPadMode::GET_FLOAT:
        if (textBuffer[0] == '-')
        {
            ShiftBufferBack();
            length--;
        }
        else
        {
            ShiftBufferForward();
            textBuffer[0] = '-';
            length++;
        }
        break;

    default:
        // There is no reason to add negatives to ip address
        break;
    }
}

void NumberPadForm::OnDotClicked()
{
    // Do not overwrite the buffer
    if(length >= 49)
    {
        return;
    }

    switch (mode)
    {
    case NumberPadMode::GET_STRING:
        textBuffer[length] = '.';
        length++;
        textBuffer[length] = 0;
        break;

    case NumberPadMode::GET_FLOAT:
        if (!Contains('.'))
        {
            textBuffer[length] = '.';
            length++;
            textBuffer[length] = 0;
        }
        break;

    case NumberPadMode::GET_IPADDRESS:
        length++;
        if (GetCount('.') < 3 && QString(textBuffer).split('.').last().length() == 3)
        {
            textBuffer[length] = '.';
            length++;
        }
        textBuffer[length] = 0;
        break;

    case NumberPadMode::GET_INT:
        break;
    }
    textHolder->setText(textBuffer);
}

void NumberPadForm::OnCancelClicked()
{
    close();
    opener->show();
    disconnect(this, &NumberPadForm::Finished, Q_NULLPTR, Q_NULLPTR);
}

void NumberPadForm::OnSaveClicked()
{
    if (mode == NumberPadMode::GET_FLOAT || mode == NumberPadMode::GET_INT)
    {
        double val = QString(textBuffer).toFloat();
        if (val < min || val > max)
        {
            textHolder->setText(QString("<font color='red'>").append(textBuffer).append("</font>"));
            return;
        }
    }
    else if (mode == NumberPadMode::GET_IPADDRESS)
    {
        if (GetCount('.') != 3)
        {
            textHolder->setText(QString("<font color='red'>").append(textBuffer).append("</font>"));
            return;
        }
    }

    emit Finished(QString(textBuffer));
    close();
    opener->show();
    disconnect(this, &NumberPadForm::Finished, Q_NULLPTR, Q_NULLPTR);
}

void NumberPadForm::AddDigit(char text)
{
    // Do not overwrite the buffer
    if(length >= 49)
    {
        return;
    }

    switch (mode)
    {
    case NumberPadMode::GET_INT:
    case NumberPadMode::GET_FLOAT:
    case NumberPadMode::GET_STRING:
        textBuffer[length] = text;
        length++;
        textBuffer[length] = 0;
        break;

    case NumberPadMode::GET_IPADDRESS:
        textBuffer[length] = text;
        length++;
        if (QString(textBuffer).split('.').count() < 3 && QString(textBuffer).split('.').last().length() == 3)
        {
            textBuffer[length] = '.';
            length++;
        }
        textBuffer[length] = 0;
        break;
    }
    textHolder->setText(textBuffer);
}

void NumberPadForm::ShiftBufferForward()
{
    for (int i = 0; i < 49; i++)
    {
        textBuffer[i + 1] = textBuffer[i];
    }
}

void NumberPadForm::ShiftBufferBack()
{
    for (int i = 0; i < 49; i++)
    {
        textBuffer[i] = textBuffer[i + 1];
    }
}

bool NumberPadForm::Contains(char chr)
{
    for (int i = 0; i < length; i++)
    {
        if (textBuffer[i] == chr)
        {
            return true;
        }
    }
    return false;
}

int NumberPadForm::GetCount(char chr)
{
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        if (textBuffer[i] == chr)
        {
            count++;
        }
    }
    return count;
}
