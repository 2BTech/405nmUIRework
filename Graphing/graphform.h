#ifndef GRAPHFORM_H
#define GRAPHFORM_H

#include <QWidget>
#include <QPen>
#include <QPushButton>
#include <QLabel>

#include <math.h>

#include "ValueHandlers/ValueObjects/valueobjects.h"
#include "ValueHandlers/datahandler.h"
#include "valuegraphsettings.h"

#include "dataselectform.h"
#include "rangeform.h"

#include "qcustomplot.h"

class GraphForm : public QWidget
{
    Q_OBJECT
public:
    GraphForm();
    ~GraphForm();

    void AddValueObjects(QList<BaseValueObject*> values);

public slots:
    // This function is triggered when a device reads a new data line. This handles
    // updating the values the graph is displaying. If the page is closed, the graph does not perform the update.
    void UpdateGraph();

signals:
    void CloseForm();

private slots:
    // This function sets all of the bottom buttons visible and resizes the graph
    void ShowBottom();
    // This function hides all of the bottom buttons and resizes the graph
    void HideBottom();
    // This function sets all of the legend labels visible and resizes the graph
    void ShowSide();
    // This function hides all of the legend labels and resizes the graph
    void HideSide();

    // Opens the main window and closes this page
    void OnCloseClicked();
    // Opens the data select form and closes this page
    void OnSelectClicked();
    // Opens the range form and closes this page
    void OnRangeClicked();
    // Decreases the range of the graph
    void OnZoomInClicked();
    // Increases the range of the graph
    void OnZoomOutClicked();
    // Resets the range of the graph and restarts the auto scaling
    void OnResetClicked();

    // Updates the range values for the graph and forces the plot to be redrawn
    void RedrawGraph();
    // Sets the contents of the legend labels
    void UpdateDataLabels();

    // Handles scrolling in or out based on the event.
    void HandleScrollEvent(QWheelEvent* event);
    // Handles moving the plot based on the event
    void HandleMouseMove(QMouseEvent* event);

private:
    QPushButton* homeButton = Q_NULLPTR;
    QPushButton* selectButton = Q_NULLPTR;
    QPushButton* rangeButton = Q_NULLPTR;
    QPushButton* zoomInButton = Q_NULLPTR;
    QPushButton* zoomOutButton = Q_NULLPTR;
    QPushButton* resetButton = Q_NULLPTR;

    // Lables used to display the legend
    QList<QLabel*> legendLabels;
    // Buttons that make up the bottom bar
    QList<QWidget*> bottomBar;

    // Button used to show the bottom buttons
    QPushButton* showBottomButton = Q_NULLPTR;
    // Button used to hide the bottom buttons
    QPushButton* hideBottomButton = Q_NULLPTR;
    // Button used to show the legend
    QPushButton* showSideButton = Q_NULLPTR;
    // Button used to hide the legend
    QPushButton* hideSideButton = Q_NULLPTR;

    // Geometry of the graph when both the bottom and legend are open.
    QRect bothOpenSize;
    // Geometry of the graph when both the bottom and legend are closed.
    QRect bothClosedSize;
    // Geometry of the graph when the bottom is open and the legend is closed.
    QRect bottomOpenSize;
    // Geometry of the graph when the legend is open and the bottom is closed.
    QRect sideOpenSize;

    // Is the bottom bar open
    bool showingBottom = true;
    // Is the legend open
    bool showingSide = true;

    // Reference to the QCustomPlot used to graph the data
    QCustomPlot* customPlot = Q_NULLPTR;
    // Formats the x axis label
    //QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker;

    // Min y displayed by the graph
    double minY = -10;
    // Max y displayed by the graph
    double maxY = 10;
    // Min x displayed by the graph
    //QDateTime minX;
    int minX;
    // Max x displayed by the graph
    //QDateTime maxX;
    int maxX;

    // Is the min x being auto scaled
    bool autoMinX = true;
    // Is the max x being auto scaled
    bool autoMaxX = true;
    // Is the min Y being auto scaled
    bool autoMinY = true;
    // Is the max y being auto scaled
    bool autoMaxY = true;

    // Names of the connectd devices
    QList<QString> deviceNames;
    // Index of the currently selectd device
    int selectedDevice = 0;

    // Helper forms
    // Form used to change the range and auto complete values
    RangeForm* rangeForm = Q_NULLPTR;
    // Form used to changes the values used to display data points
    DataSelectForm* dataSelect = Q_NULLPTR;

    // Empty date time object. Used with scrolling and scaling input.
    const QDateTime blankDT = QDateTime(QDate(1970, 1, 1), QTime(0,0,0));

    QMap<BaseValueObject*, ValueGraphSettings*> graphObjects;

    void BuildUIElements();

    // Updates the geometry of the plot based on if the legend and bottm buttons are visible
    void SetGraphSize();
    // This functons sets up the objects that handle graphing functionality. Sets the graph to autoscale by defualt.
    void SetUpGraph();

    // This function builds the graphing objects fo reach device
    void SetUpPlots();
    // Forces the graph to rescale.
    void FixScale();

    // Converts a color to a string representation
    QString GetColorNameFromColor(Qt::GlobalColor color);
    // Converts an index to the Qt::GlobalColor it refers to
    Qt::GlobalColor GetColorFromIndex(int index);

    // This function is triggered when the page is opened. Forces the graph to replot.
    void showEvent(QShowEvent * event) override;
};

#endif // GRAPHFORM_H
