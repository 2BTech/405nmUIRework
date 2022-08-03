#include "dataselectform.h"

DataSelectForm::DataSelectForm() : QWidget()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);
}

DataSelectForm::~DataSelectForm()
{
    if (title)
    {
        title->deleteLater();

        variableSelect->deleteLater();
        colorSelect->deleteLater();
        displayCheck->deleteLater();

        saveButton->deleteLater();
        cancelButton->deleteLater();

        title = Q_NULLPTR;

        variableSelect = colorSelect = Q_NULLPTR;

        displayCheck = Q_NULLPTR;

        saveButton = cancelButton = Q_NULLPTR;
    }
}

void DataSelectForm::BuildUIElements(QMap<BaseValueObject*, ValueGraphSettings*> valueObjects)
{
    this->valueObjects = valueObjects;

    int tempX = 480 / 20;
    int tempY = 277 / 20;

    QFont titleFont;
    titleFont.setPointSize(25);

    //Title
    title = new QLabel(this);
    title->setGeometry(0, 0, 480, 60);
    title->setText("Graph Range");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->setStyleSheet("QLabel { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");

    //Variable selector
    QStringList variables = QStringList() << "Variables";
    QString longest;

    for (BaseValueObject* val : valueObjects.keys())
    {
        variables << val->getName();
    }
    variableSelect = new QComboBox(this);
    variableSelect->setGeometry((480 / 2) - (tempX * 4), tempY * 8, tempX * 8, tempY * 2);
    QFont tempFont;
    tempFont.setPointSize(12);
    variableSelect->addItems(variables);
    variableSelect->setFont(tempFont);
    variableSelect->show();
    connect(variableSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(OnVariablesIndexChange(int)));

    //Color selector
    QStringList colors = QStringList() << "Colors";
    colors << "Black";
    colors << "Gray";
    colors << "Red";
    colors << "Green";
    colors << "Blue";
    colors << "Cyan";
    colors << "Magenta";
    colors << "Yellow";
    colors << "Dark Red";
    colors << "Dark Green";
    colors << "Dark Blue";
    colors << "Dark Cyan";
    colors << "Dark Magenta";
    colorSelect = new QComboBox(this);
    colorSelect->setGeometry((480 / 2) - (tempX * 4), tempY * 11, tempX * 8, tempY * 2);
    colorSelect->addItems(colors);
    colorSelect->setFont(tempFont);
    colorSelect->show();

    displayCheck = new QCheckBox(this);
    displayCheck->setGeometry((480 / 2) - (tempX * 3), tempY * 14, tempX * 6, tempY * 2);
    displayCheck->setText("Show on graph");
    displayCheck->setFont(tempFont);
    displayCheck->show();

    int buttonWidth = tempX * 4;

    //Apply button
    saveButton = new QPushButton(this);
    saveButton->setGeometry((480 / 2) - (buttonWidth) - (buttonWidth / 4), tempY * 17, tempX * 4, tempY * 2);
    saveButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    saveButton->setText("Apply");
    connect(saveButton, &QPushButton::clicked, this, &DataSelectForm::OnApplyClicked);

    //Close Button
    cancelButton = new QPushButton(this);
    cancelButton->setGeometry((480 / 2) + (buttonWidth / 4), tempY * 17, tempX * 4, tempY * 2);
    cancelButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    cancelButton->setText("Close");
    connect(cancelButton, &QPushButton::clicked, this, &DataSelectForm::OnCloseClicked);
}

void DataSelectForm::OnVariablesIndexChange(int index)
{
    //Select default value
    if(index == 0)
    {
        colorSelect->setCurrentIndex(0);
        displayCheck->setChecked(false);
    }
    //Index set when combo box is cleared, nothing to do so just return
    else if(index == -1)
    {
        return;
    }
    //Update other fields with newly selected objects values
    else
    {
        ValueGraphSettings * graphObj = valueObjects[valueObjects.keys()[index - 1]];
        colorSelect->setCurrentIndex(GetIndexFromColor(graphObj->GetGraphColor()) + 1);
        displayCheck->setChecked(graphObj->GetGraph()->visible());
    }
}

void DataSelectForm::OnApplyClicked()
{
    // Make sure the default item is not selected
    if(variableSelect->currentIndex() > 0)
    {
        //Get the graph object the user is editting
        ValueGraphSettings* current = valueObjects[valueObjects.keys()[variableSelect->currentIndex() - 1]];
        //Change the color
        current->SetGraphColor(GetColorFromIndex(colorSelect->currentIndex() - 1));
        //Set visiblility
        current->SetVisible(displayCheck->checkState() == Qt::CheckState::Checked);

        QMessageBox msg;
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("Applied settings");
        msg.setStyleSheet("QMessageBox { border: 1px}");
        msg.exec();

        //BaseLogger::Log("Setting " + current->GetDataObj()->GetName() + " visibility to " + (displayCheck->checkState() == Qt::CheckState::Checked ? "true" : "false"));
    }
    else
    {
        QMessageBox msg;
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("No changes to apply");
        msg.setStyleSheet("QMessageBox { border: 1px}");
        msg.exec();
    }
}

void DataSelectForm::OnCloseClicked()
{
    emit UpdateGraph();
    emit CloseForm();
}

int DataSelectForm::GetIndexFromColor(Qt::GlobalColor color)
{
    switch (color)
    {
    case Qt::GlobalColor::black:
        return 0;

    case Qt::GlobalColor::gray:
        return 1;

    case Qt::GlobalColor::red:
        return 2;

    case Qt::GlobalColor::green:
        return 3;

    case Qt::GlobalColor::blue:
        return 4;

    case Qt::GlobalColor::cyan:
        return 5;

    case Qt::GlobalColor::magenta:
        return 6;

    case Qt::GlobalColor::yellow:
        return 7;

    case Qt::GlobalColor::darkRed:
        return 8;

    case Qt::GlobalColor::darkGreen:
        return 9;

    case Qt::GlobalColor::darkBlue:
        return 10;

    case Qt::GlobalColor::darkCyan:
        return 11;

    case Qt::GlobalColor::darkMagenta:
        return 12;

    default:
        qDebug() << ("Received bad qt global color in get index from color: " + QString::number(color));
        return -1;
    }
}

Qt::GlobalColor DataSelectForm::GetColorFromIndex(int index)
{
    switch (index)
    {
    case 0:
        return Qt::GlobalColor::black;

    case 1:
        return Qt::GlobalColor::gray;

    case 2:
        return Qt::GlobalColor::red;

    case 3:
        return Qt::GlobalColor::green;

    case 4:
        return Qt::GlobalColor::blue;

    case 5:
        return Qt::GlobalColor::cyan;

    case 6:
        return Qt::GlobalColor::magenta;

    case 7:
        return Qt::GlobalColor::yellow;

    case 8:
        return Qt::GlobalColor::darkRed;

    case 9:
        return Qt::GlobalColor::darkGreen;

    case 10:
        return Qt::GlobalColor::darkBlue;

    case 11:
        return Qt::GlobalColor::darkCyan;

    case 12:
        return Qt::GlobalColor::darkMagenta;

    default:
        qDebug() << ("Received bad qt global color in get index from color: " + QString::number(index));
        return Qt::GlobalColor::transparent;
    }
}

void DataSelectForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    variableSelect->setCurrentIndex(0);
    colorSelect->setCurrentIndex(0);
    displayCheck->setChecked(false);
}
