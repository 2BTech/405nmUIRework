#include "valuegraphsettings.h"

ValueGraphSettings::ValueGraphSettings(BaseValueObject* value, bool show)
{
    val = value;
    SetVisible(show);
}

void ValueGraphSettings::SetGraphColor(Qt::GlobalColor color)
{
    graphColor = color;
    graphPen.setColor(QColor(color));
    if(graph != Q_NULLPTR)
    {
        graph->setPen(graphPen);
    }
}

void ValueGraphSettings::SetVisible()
{
    isVisible = true;
    graph->setVisible(isVisible);
}

void ValueGraphSettings::SetVisible(bool val)
{
    isVisible = val;
    //SetVisible();
    //graph->setVisible(val);
    UpdateDisplay();
}

bool ValueGraphSettings::IsVisible()
{
    return isVisible;
    //return graph->visible();
}

void ValueGraphSettings::SetActive(bool active)
{
    isVisible = active;
    //SetVisible();
    UpdateDisplay();
}

bool ValueGraphSettings::ShouldDisplay()
{
    return isVisible;
}

void ValueGraphSettings::UpdateDisplay()
{
    if(graph != Q_NULLPTR)
    {
        graph->setVisible(isVisible);
    }
}

void ValueGraphSettings::SetGraph(QCPGraph* _graph)
{
    graph = _graph;
    graph->setPen(graphPen);
}

double ValueGraphSettings::GetMinX()
{
    return minX;
}

double ValueGraphSettings::GetMaxX()
{
    return maxX;
}

double ValueGraphSettings::GetMinY()
{
    return minY;
}

double ValueGraphSettings::GetMaxY()
{
    return maxY;
}

BaseValueObject* ValueGraphSettings::GetDataObj()
{
    return val;
}

Qt::GlobalColor ValueGraphSettings::GetGraphColor()
{
    return graphColor;
}

QCPGraph * ValueGraphSettings::GetGraph()
{
    return graph;
}

void ValueGraphSettings::OnValueChange()
{
    double yVal;
    uint xVal = QDateTime::currentDateTime().toTime_t();

    switch (this->val->GetValueType())
    {
    case ValueType::UCHAR_VALUE:
        yVal = dynamic_cast<ValueObject<uchar>*>(this->val)->getValue();
        break;

    case ValueType::INT_VALUE:
        yVal = dynamic_cast<ValueObject<int>*>(this->val)->getValue();
        break;

    case ValueType::FLOAT_VALUE:
        yVal = dynamic_cast<ValueObject<float>*>(this->val)->getValue();
        break;

    case ValueType::STRING_VALUE:
    default:
        return;
    }

    keys.append(yVal);
    values.append(xVal);

    if(yVal < minY)
    {
        minY = yVal;
    }
    if(yVal > maxY)
    {
        maxY = yVal;
    }
    maxX = xVal;

    if(values.count() >= 100)
    {
        double rem = values.takeFirst();
        if(rem == minY)
        {
            minY = values.first();
            int count = values.count();
            for(int i = 1; i < count; i++)
            {
                if(values[i] < minY)
                {
                    minY = values[i];
                }
            }
        }
        else if (rem == maxY)
        {
            maxY = values.first();
            int count = values.count();
            for(int i = 1; i < count; i++)
            {
                if(values[i] > maxY)
                {
                    maxY = values[i];
                }
            }
        }
        keys.removeFirst();
    }

    //graph->addData(yVal, xVal);
    graph->setData(values, keys);

    //qDebug() << "X values: " << values;

    minX = values.first();
    maxX = values.last();

    //qDebug() << "X Range: " << minX << " -> " << maxX;

    //maxX = keys.last();

    //BaseLogger::Log("Min: " + QDateTime(QDate(1970, 1, 1), QTime(0,0,0)).addSecs(minX).toString());
    //BaseLogger::Log("Max: " + QDateTime(QDate(1970, 1, 1), QTime(0,0,0)).addSecs(maxX).toString());
}
