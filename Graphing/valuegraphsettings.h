#ifndef VALUEGRAPHSETTINGS_H
#define VALUEGRAPHSETTINGS_H

#include <QVector>

#include "qcustomplot.h"
#include "ValueHandlers/ValueObjects/valueobjects.h"

class ValueGraphSettings : public QObject
{
    Q_OBJECT
public:
    ValueGraphSettings(BaseValueObject* val, bool show);

    // Sets graph. The object that displays the values.
    void SetGraph(QCPGraph * graph);

    // Sets the color of the plot
    void SetGraphColor(Qt::GlobalColor color);

    // Sets the visibile boolean to true
    void SetVisible();
    // Sets the visible boolean to to val
    void SetVisible(bool val);
    // Returns true if the graph is visible
    bool IsVisible();
    // Sets isActive to active
    void SetActive(bool active);
    // Returns true if both is isActive and isVisible are true
    bool ShouldDisplay();

    // Sets the graph to be either visible or hidden
    void UpdateDisplay();

    // Getters for the graph bounds
    double GetMinY();
    double GetMaxY();
    double GetMinX();
    double GetMaxX();

    // Returns the data object that this represents
    BaseValueObject* GetDataObj();
    // Returns the color of the graph
    Qt::GlobalColor GetGraphColor();
    // Returns the graph object that this populates
    QCPGraph* GetGraph();

public slots:
    void OnValueChange();

private:
    BaseValueObject* val = Q_NULLPTR;

    QPen graphPen = QPen(Qt::GlobalColor::blue);
    Qt::GlobalColor graphColor = Qt::GlobalColor::blue;

    QCPGraph* graph = Q_NULLPTR;
    QVector<double> keys;
    QVector<double> values;

    // If true, show graph when is active
    bool isVisible = false;

    double minY = 10000000, maxY = -10000000, minX = 10000000, maxX = -10000000;
};

#endif // VALUEGRAPHSETTINGS_H
