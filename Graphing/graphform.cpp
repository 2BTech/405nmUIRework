#include "graphform.h"

GraphForm::GraphForm() : QWidget()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);

    BuildUIElements();
}

GraphForm::~GraphForm()
{
    if (homeButton)
    {
        homeButton->deleteLater();
        selectButton->deleteLater();
        rangeButton->deleteLater();
        zoomInButton->deleteLater();
        zoomOutButton->deleteLater();
        resetButton->deleteLater();

        while (!legendLabels.isEmpty())
        {
            legendLabels.takeFirst()->deleteLater();
        }
        bottomBar.clear();

        showBottomButton->deleteLater();
        hideBottomButton->deleteLater();
        showSideButton->deleteLater();
        hideSideButton->deleteLater();

        customPlot->deleteLater();

        for (ValueGraphSettings* obj : graphObjects)
        {
            obj->deleteLater();
        }
        graphObjects.clear();

        homeButton = selectButton = rangeButton = zoomInButton = zoomOutButton = resetButton = showBottomButton = hideBottomButton = showSideButton = hideSideButton = Q_NULLPTR;

        customPlot = Q_NULLPTR;
    }
}

void GraphForm::BuildUIElements()
{
    int tempX = 480 / 10;
    int tempY = 277 / 10;
    int graphOffset = tempY * 3 / 2;

    //Build graph sizes
    //Both bars open
    bothOpenSize   = QRect(0, 0, (tempX * 8) - (tempX / 2),  (tempY * 8) - (tempX / 2) - (graphOffset) + (tempY * 3 / 2));
    bothClosedSize = QRect(0, 0, (tempX * 10) - (tempX / 2), (tempY * 10) - (tempX / 2) - (graphOffset) + (tempY * 3 / 2));
    bottomOpenSize = QRect(0, 0, (tempX * 10) - (tempX / 2),  (tempY * 8) - (tempX / 2) - (graphOffset) + (tempY * 3 / 2));
    sideOpenSize   = QRect(0, 0, (tempX * 8) - (tempX / 2), (tempY * 10) - (tempX / 2) - (graphOffset) + (tempY * 3 / 2));

    int buttonWidth = 480 / 6;

    int missingX = 480 % 10;
    int missingY = 277 % 10;

    QFont font;
    font.setPointSize(12);

    //Home button
    homeButton = new QPushButton(this);
    homeButton->setGeometry(buttonWidth * 0, tempY * 8, buttonWidth, tempY * 2 + missingY);
    homeButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    homeButton->setText("Close");

    bottomBar.append(homeButton);
    connect(homeButton, &QPushButton::clicked, this, &GraphForm::OnCloseClicked);
    homeButton->setFont(font);

    //Select button
    selectButton = new QPushButton(this);
    selectButton->setGeometry(buttonWidth * 1, tempY * 8, buttonWidth, tempY * 2 + missingY);
    selectButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    selectButton->setText("Select");
    bottomBar.append(selectButton);
    connect(selectButton, &QPushButton::clicked, this, &GraphForm::OnSelectClicked);
    selectButton->setFont(font);

    //Range button
    rangeButton = new QPushButton(this);
    rangeButton->setGeometry(buttonWidth * 2, tempY * 8, buttonWidth, tempY * 2 + missingY);
    rangeButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    rangeButton->setText("Range");
    bottomBar.append(rangeButton);
    connect(rangeButton, &QPushButton::clicked, this, &GraphForm::OnRangeClicked);
    rangeButton->setFont(font);

    //Zoom in button
    zoomInButton = new QPushButton(this);
    zoomInButton->setGeometry(buttonWidth * 3, tempY * 8, buttonWidth, tempY * 2 + missingY);
    zoomInButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    zoomInButton->setText("Zoom\nIn");
    bottomBar.append(zoomInButton);
    connect(zoomInButton, &QPushButton::clicked, this, &GraphForm::OnZoomInClicked);
    zoomInButton->setFont(font);

    //Zoom out button
    zoomOutButton = new QPushButton(this);
    zoomOutButton->setGeometry(buttonWidth * 4, tempY * 8, buttonWidth, tempY * 2 + missingY);
    zoomOutButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    zoomOutButton->setText("Zoom\nOut");
    bottomBar.append(zoomOutButton);
    connect(zoomOutButton, &QPushButton::clicked, this, &GraphForm::OnZoomOutClicked);
    zoomOutButton->setFont(font);

    //Reset auto scale values
    resetButton = new QPushButton(this);
    resetButton->setGeometry(buttonWidth * 5, tempY * 8, buttonWidth + missingX, tempY * 2 + missingY);
    resetButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    resetButton->setText("Reset");
    bottomBar.append(resetButton);
    connect(resetButton, &QPushButton::clicked, this, &GraphForm::OnResetClicked);
    resetButton->setFont(font);

    //Create variable labels
    QLabel* legendLabel = Q_NULLPTR;
    for(int i = 0; i < 8; i++)
    {
        legendLabel = new QLabel(this);
        legendLabel->setGeometry(tempX * 8, tempY * i, (tempX * 2), tempY);
        legendLabels.append(legendLabel);
    }

    //Create show and hide buttons
    //Hide side bar
    hideSideButton = new QPushButton(this);
    hideSideButton->setGeometry((tempX * 8) - (tempX / 2), (277 / 2) - (tempX / 4), tempX / 2, tempX / 2);
    hideSideButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(hideSideButton, &QPushButton::clicked, this, &GraphForm::HideSide);

    //Show side bar
    showSideButton = new QPushButton(this);
    showSideButton->setGeometry((tempX * 10) - (tempX / 2), (277 / 2) - (tempX / 4), tempX / 2, tempX / 2);
    showSideButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    showSideButton->close();
    connect(showSideButton, &QPushButton::clicked, this, &GraphForm::ShowSide);

    //Hide bottom bar
    hideBottomButton = new QPushButton(this);
    hideBottomButton->setGeometry((480 / 2) - (tempX / 4), (tempY * 8) - (tempX / 2), tempX / 2, tempX / 2);
    hideBottomButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(hideBottomButton, &QPushButton::clicked, this, &GraphForm::HideBottom);

    //Show bottom bar
    showBottomButton = new QPushButton(this);
    showBottomButton->setGeometry((480 / 2) - (tempX / 4), (tempY * 10) - (tempX / 2), tempX / 2, tempX / 2);
    showBottomButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    showBottomButton->close();
    connect(showBottomButton, &QPushButton::clicked, this, &GraphForm::ShowBottom);

    SetUpGraph();

    rangeForm = new RangeForm();
    rangeForm->SetInformation(&minX, &maxX, &minY, &maxY, &autoMinX, &autoMaxX, &autoMinY, &autoMaxY);
    rangeForm->close();
    connect(rangeForm, &RangeForm::UpdateGraph, this, &GraphForm::RedrawGraph);
    connect(rangeForm, &RangeForm::CloseForm, this, &GraphForm::show);
    connect(rangeForm, &RangeForm::CloseForm, rangeForm, &RangeForm::hide);

    dataSelect = new DataSelectForm();
    dataSelect->close();
    connect(dataSelect, &DataSelectForm::UpdateGraph, this, &GraphForm::RedrawGraph);
    connect(dataSelect, &DataSelectForm::CloseForm, this, &GraphForm::show);
    connect(dataSelect, &DataSelectForm::CloseForm, dataSelect, &DataSelectForm::hide);

    UpdateDataLabels();
}

void GraphForm::AddValueObjects(QList<BaseValueObject*> values)
{
    int colorIndex = 0;
    for (BaseValueObject* val : values)
    {
        if (val->GetValueType() != ValueType::STRING_VALUE)
        {
            ValueGraphSettings* obj = new ValueGraphSettings(val, false);
            obj->SetGraph(customPlot->addGraph());
            obj->SetGraphColor(GetColorFromIndex(colorIndex++));
            // ToDo, add connect to update graph object internal buffer
            connect(val, &BaseValueObject::SetValue, obj, &ValueGraphSettings::OnValueChange);

            if (val->getName() == "NO" || val->getName() == "NO2" || val->getName() == "NOx")
            {
                //qDebug() << "Setting visible: " << val->getName();
                obj->SetVisible(true);
            }
            else
            {
                obj->SetVisible(false);
            }

            graphObjects.insert(val, obj);
            if(colorIndex >= 15)
            {
                colorIndex = 0;
            }
        }
    }
    dataSelect->BuildUIElements(graphObjects);
    RedrawGraph();
}

void GraphForm::UpdateGraph()
{
    //qDebug() << "Updating graph: " << isVisible();
    if (!isVisible())
    {
        return;
    }

    QList<ValueGraphSettings*> objs = graphObjects.values();

    bool grabbedInitial = false;

    int count = objs.count();
    for(int i = 1; i < count; i++)
    {
        if(objs[i]->ShouldDisplay())
        {
            if(!grabbedInitial)
            {
                grabbedInitial = true;
                if(autoMinX)
                {
                    //minX = QDateTime::fromSecsSinceEpoch(objs.at(i)->GetMinX());
                    minX = objs.at(i)->GetMinX();
                }
                if(autoMaxX)
                {
                    //maxX = QDateTime::fromSecsSinceEpoch(objs.at(i)->GetMaxX());
                    maxX = objs.at(i)->GetMaxX();
                }
                if(autoMinY)
                {
                    minY = objs.at(i)->GetMinY();
                }
                if(autoMaxY)
                {
                    maxY = objs.at(i)->GetMaxY();
                }
                continue;
            }

            if(autoMinX && minX > objs.at(i)->GetMinX())
            {
                /*minX = minX.toSecsSinceEpoch() < objs.at(i)->GetMinX()
                        ? minX : QDateTime::fromSecsSinceEpoch(objs.at(i)->GetMinX());*/
                minX = objs.at(i)->GetMinX();
            }
            if(autoMaxX && maxX < objs.at(i)->GetMaxX())
            {
//                maxX = maxX.toSecsSinceEpoch() > objs.at(i)->GetMaxX()
//                        ? maxX : QDateTime::fromSecsSinceEpoch(objs.at(i)->GetMaxX());
                maxX = objs.at(i)->GetMaxX();
            }
            if(autoMinY)
            {
                minY = minY < objs.at(i)->GetMinY()
                        ? minY : objs.at(i)->GetMinY();
            }
            if(autoMaxY)
            {
                maxY = maxY > objs.at(i)->GetMaxY()
                        ? maxY : objs.at(i)->GetMaxY();
            }
#if DEBUG_BUILD && DEBUG_GRAPH_FORM
            BaseLogger::Log("X: Min: " + QString::number(objs.at(i)->GetMinX()) + "Max: " + QString::number(objs.at(i)->GetMaxX()));
            BaseLogger::Log("Y: Min: " + QString::number(objs.at(i)->GetMinY()) + " Max: " + QString::number(objs.at(i)->GetMaxY()));
#endif
//            qDebug() << "X: " << objs.at(i)->GetMinX() << " to " << objs.at(i)->GetMaxX();
//            qDebug() << "Y: " << objs.at(i)->GetMinY() << " to " << objs.at(i)->GetMaxY();

//            qDebug() << "X: " << minX << " to " << maxX;
//            qDebug() << "Y: " << minY << " to " << maxY;
        }
    }
#if DEBUG_BUILD && DEBUG_GRAPH_FORM
    BaseLogger::Log("X: Min: " + QString::number(minX) + " Max: " + QString::number(maxX));
    BaseLogger::Log("Y: Min: " + QString::number(minY) + " Max: " + QString::number(maxY));
#endif

    //int xDiff = maxX.toSecsSinceEpoch() - minX.toSecsSinceEpoch();
    int xDiff = maxX - minX;
    if(xDiff == 0)
    {
        if(autoMaxX)
        {
            //maxX = maxX.addSecs(60);
            maxX += 60;
        }
        if(autoMinX)
        {
            //minX = minX.addSecs(-60);
            minX -= 60;
        }
    }
    else
    {
        xDiff /= 10;
        if(autoMaxX)
        {
            //maxX = maxX.addSecs(xDiff);
            maxX += xDiff;
        }
        if(autoMinX)
        {
            //minX = minX.addSecs(-xDiff);
            minX -= xDiff;
        }
    }

    if(minY == maxY)
    {
        minY -= 5;
        maxY += 5;
    }

    if(autoMinY || autoMaxY)
    {
        double diff = (maxY - minY) / 20;
        if(autoMinY)
        {
            minY -= diff;
        }
        if(autoMaxY)
        {
            maxY += diff;
        }
    }

    //customPlot->xAxis->setRange(minX.toSecsSinceEpoch(), maxX.toSecsSinceEpoch());
    customPlot->xAxis->setRange(minX, maxX);
    customPlot->yAxis->setRange(minY, maxY);

    //BaseLogger::Log("X range: " + minX.time().toString() + " to " + maxX.time().toString());
//    qDebug() << "X Range: " << minX << " to " << maxX;
//    qDebug() << "Y Range: " << minY << " to " << maxY;

    //BaseLogger::Log("X Range: " + QString::number(customPlot->xAxis->range().size()));
    //BaseLogger::Log("X: Min: " + QString::number(customPlot->xAxis->range().lower) + " Max: " + QString::number(customPlot->xAxis->range().upper));
    //BaseLogger::Log(minX == maxX ? "True" : "False");

#if DEBUG_BUILD && DEBUG_GRAPH_FORM
    BaseLogger::Log("X Range: " + QString::number(customPlot->xAxis->range().size()));
    BaseLogger::Log("X: Min: " + QString::number(customPlot->xAxis->range().lower) + " Max: " + QString::number(customPlot->xAxis->range().upper));
    BaseLogger::Log("Y Range: " + QString::number(customPlot->yAxis->range().size()));
    BaseLogger::Log("Y: Min: " + QString::number(customPlot->yAxis->range().lower) + " Max: " + QString::number(customPlot->yAxis->range().upper));
#endif

    customPlot->replot();
}

void GraphForm::ShowBottom()
{
    for (QWidget* obj : bottomBar)
    {
        obj->show();
    }

    showingBottom = true;
    showBottomButton->close();
    hideBottomButton->show();
    SetGraphSize();
}

void GraphForm::HideBottom()
{
    for (QWidget* obj : bottomBar)
    {
        obj->hide();
    }

    showingBottom = false;
    showBottomButton->show();
    hideBottomButton->close();
    SetGraphSize();
}

void GraphForm::ShowSide()
{
    for (QWidget* obj : legendLabels)
    {
        obj->show();
    }
    showingSide = true;
    showSideButton->close();
    hideSideButton->show();
    SetGraphSize();
}

void GraphForm::HideSide()
{
    for (QWidget* obj : legendLabels)
    {
        obj->hide();
    }
    showingSide = false;
    showSideButton->show();
    hideSideButton->close();
    SetGraphSize();
}

void GraphForm::OnCloseClicked()
{
    emit CloseForm();
}

void GraphForm::OnSelectClicked()
{
    close();
    dataSelect->show();
}

void GraphForm::OnRangeClicked()
{
    close();
    rangeForm->show();
}

void GraphForm::OnZoomInClicked()
{
    autoMaxX = false;
    autoMinX = false;
    autoMaxY = false;
    autoMinY = false;

//    double xLength = (maxX.toSecsSinceEpoch() - minX.toSecsSinceEpoch()) / 40;
    double xLength = (maxX - minX) / 40;
    double yLength = (maxY - minY) / 40;

//    minX = minX.addSecs(-xLength);
//    maxX = maxX.addSecs(xLength);
    minX -= xLength;
    maxX += xLength;

    minY += yLength;
    maxY -= yLength;

//    customPlot->xAxis->setRange(minX.toSecsSinceEpoch(), maxX.toSecsSinceEpoch());
    customPlot->xAxis->setRange(minX, maxX);
    customPlot->yAxis->setRange(minY, maxY);

    FixScale();
    customPlot->replot();
}

void GraphForm::OnZoomOutClicked()
{
    autoMaxX = false;
    autoMinX = false;
    autoMaxY = false;
    autoMinY = false;

    //double xLength = (maxX.toSecsSinceEpoch() - minX.toSecsSinceEpoch()) / 40;
    double xLength = (maxX - minX) / 40;
    double yLength = (maxY - minY) / 40;

//    minX = minX.addSecs(-xLength);
//    maxX = maxX.addSecs(xLength);
    minX -= xLength;
    maxX += xLength;

    minY -= yLength;
    maxY += yLength;

    //customPlot->xAxis->setRange(minX.toSecsSinceEpoch(), maxX.toSecsSinceEpoch());
    customPlot->xAxis->setRange(minX, maxX);
    customPlot->yAxis->setRange(minY, maxY);

    FixScale();
    customPlot->replot();
}

void GraphForm::OnResetClicked()
{
    if(customPlot == Q_NULLPTR)
    {
        return;
    }

    autoMinX = true;
    autoMaxX = true;
    autoMinY = true;
    autoMaxY = true;

    UpdateGraph();
}

void GraphForm::RedrawGraph()
{
    //customPlot->xAxis->setRange(minX.toSecsSinceEpoch(), maxX.toSecsSinceEpoch());
    customPlot->xAxis->setRange(minX, maxX);
    customPlot->yAxis->setRange(minY, maxY);
    customPlot->replot();

    //qDebug() << "Redrawing graph";

    UpdateDataLabels();
}

void GraphForm::UpdateDataLabels()
{
    int currentLabel = 0;
    int keysCount = legendLabels.count();

    if(graphObjects.count() == 0)
    {
        //qDebug() << ("No graph objects");
        return;
    }

    for (ValueGraphSettings* current : graphObjects)
    {
        //Exit if all key labels have been populated
        if(currentLabel == keysCount)
        {
            return;
        }
        //If graph object is visible, update data labels
        if(current->ShouldDisplay())
        {
            legendLabels.at(currentLabel)->setText(QString("<font color='").append(GetColorNameFromColor(current->GetGraphColor())).append("'>").append(current->GetDataObj()->getName()).append("</font>"));
            currentLabel++;
        }
    }

    for(; currentLabel < keysCount; currentLabel++)
    {
        legendLabels.at(currentLabel)->setText("");
    }
}

//Need to update user mins and maxs
void GraphForm::HandleScrollEvent(QWheelEvent * event)
{
    Q_UNUSED(event)

    autoMaxX = false;
    autoMinX = false;
    autoMaxY = false;
    autoMinY = false;

//    minX = blankDT.addSecs(customPlot->xAxis->range().lower);
//    maxX = blankDT.addSecs(customPlot->xAxis->range().upper);
//    qDebug() << ("MinX: " + QString::number(customPlot->xAxis->range().lower));
//    qDebug() << ("MaxX: " + QString::number(customPlot->xAxis->range().upper));
//    qDebug() << ("MinX: " + blankDT.addSecs(customPlot->xAxis->range().lower).toString());
//    qDebug() << ("MaxX: " + blankDT.addSecs(customPlot->xAxis->range().upper).toString());

    minX = customPlot->xAxis->range().lower;
    maxX = customPlot->xAxis->range().upper;

    minY = customPlot->yAxis->range().lower;
    maxY = customPlot->yAxis->range().upper;
}

void GraphForm::HandleMouseMove(QMouseEvent *event)
{
    Q_UNUSED(event)

    autoMaxX = false;
    autoMinX = false;
    autoMaxY = false;
    autoMinY = false;

//    minX = blankDT.addSecs(customPlot->xAxis->range().lower);
//    maxX = blankDT.addSecs(customPlot->xAxis->range().upper);
    minX = customPlot->xAxis->range().lower;
    maxX = customPlot->xAxis->range().upper;

    minY = customPlot->yAxis->range().lower;
    maxY = customPlot->yAxis->range().upper;
}

void GraphForm::SetGraphSize()
{
    if(showingBottom)
    {
        if(showingSide)
        {
            customPlot->setGeometry(bothOpenSize);
        }
        else
        {
            customPlot->setGeometry(bottomOpenSize);
        }
    }
    else
    {
        if(showingSide)
        {
            customPlot->setGeometry(sideOpenSize);
        }
        else
        {
            customPlot->setGeometry(bothClosedSize);
        }
    }
    customPlot->replot();
}

void GraphForm::SetUpGraph()
{
    //Create custom plot instance and set up internal functions and graph size
    customPlot = new QCustomPlot(this);
    customPlot->setInteractions(QCP::Interaction::iRangeDrag | QCP::Interaction::iRangeZoom | QCP::Interaction::iSelectAxes);
    //customPlot->setParent(graphHolder);
    customPlot->setGeometry(bothClosedSize);

//    dateTimeTicker = QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime);
//    customPlot->xAxis->setTicker(dateTimeTicker);
//    dateTimeTicker->setDateTimeFormat("hh:mm:ss");

    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setTickLabelRotation(30);
    customPlot->xAxis->setAutoTickStep(true);

    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");

    customPlot->setMouseTracking(false);

    connect(customPlot, &QCustomPlot::mouseWheel, this, &GraphForm::HandleScrollEvent);
    connect(customPlot, &QCustomPlot::mouseMove, this, &GraphForm::HandleMouseMove);

    //Set UI values
    customPlot->xAxis->setTickLabelFont(QFont("Cabin", 8));
    customPlot->yAxis->setTickLabelFont(QFont("Cabin", 8));

    //customPlot->xAxis->setRange(minX.toSecsSinceEpoch(), maxX.toSecsSinceEpoch());
    customPlot->xAxis->setRange(minX, maxX);
    customPlot->yAxis->setRange(minY, maxY);

    SetUpPlots();

    FixScale();

    SetGraphSize();

    customPlot->replot();
    customPlot->lower();

    //Defaut to autoscale the graph
    autoMaxX = autoMaxY = autoMinX = autoMinY = true;

    connect(DataHandler::GetInstance(), &DataHandler::ParsedDataLine, this, &GraphForm::UpdateGraph);
}

void GraphForm::SetUpPlots()
{

}

void GraphForm::FixScale()
{
    /*QCPRange plotRange = customPlot->xAxis->range();
    double range = plotRange.upper - plotRange.lower;

    if(range < 120)
    {
        customPlot->xAxis->setDateTimeFormat("mm:ss");
        customPlot->xAxis->setTickStep(10);
    }
    else if((range > 120) && (range < 3600)) //60 * 60
    {
        customPlot->xAxis->setDateTimeFormat("hh:mm");
        customPlot->xAxis->setTickStep(60);
    }
    else if((range > 3600) && (range < 86400)) //24 * 60 * 60
    {
        customPlot->xAxis->setDateTimeFormat("ddd hh");
        customPlot->xAxis->setTickStep(60*60);
    }
    else if(range > 86400) //24 * 60 * 60
    {
        customPlot->xAxis->setDateTimeFormat("mm-dd-yy");
        customPlot->xAxis->setTickStep(86400);
    }*/
    customPlot->xAxis->rescale();
    customPlot->yAxis->rescale();
}

QString GraphForm::GetColorNameFromColor(Qt::GlobalColor color)
{
    switch (color)
    {
    case Qt::GlobalColor::black:
        return "black";

    case Qt::GlobalColor::gray:
        return "gray";

    case Qt::GlobalColor::red:
        return "red";

    case Qt::GlobalColor::green:
        return "green";

    case Qt::GlobalColor::blue:
        return "blue";

    case Qt::GlobalColor::cyan:
        return "cyan";

    case Qt::GlobalColor::magenta:
        return "magenta";

    case Qt::GlobalColor::darkRed:
        return "darkRed";

    case Qt::GlobalColor::darkGreen:
        return "darkGreen";

    case Qt::GlobalColor::darkBlue:
        return "darkBlue";

    case Qt::GlobalColor::darkCyan:
        return "darkCyan";

    case Qt::GlobalColor::darkMagenta:
        return "darkMagenta";

    default:
        return "black";
    }
}

Qt::GlobalColor GraphForm::GetColorFromIndex(int index)
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
        return Qt::GlobalColor::darkRed;

    case 8:
        return Qt::GlobalColor::darkGreen;

    case 9:
        return Qt::GlobalColor::darkBlue;

    case 10:
        return Qt::GlobalColor::darkCyan;

    case 11:
        return Qt::GlobalColor::darkMagenta;

    default:
        return Qt::GlobalColor::blue;
    }
}

void GraphForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    UpdateGraph();
}
