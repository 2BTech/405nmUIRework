#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QTimer>

#include "SettingsPages/settingsmenu.h"

#include "ValueHandlers/datahandler.h"
#include "parametersform.h"
#include "Graphing/graphform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private slots:
    void OnAvgMaskClicked();

    void OnModeMaskClicked();

    void OnErrMaskClicked();

    void OnSettingsClicked();
    void OnCloseMenu();

    void OnParametersClicked();

    void OnGraphClicked();

    void UpdateUI();

private:
    Ui::MainWindow *ui;

    // Refernce to UI objects
    QFrame* statusLine = Q_NULLPTR;
    QFrame* navigationLine = Q_NULLPTR;
    QFrame* navSepLine = Q_NULLPTR;

    // Status bar objects
    QLabel* statusLabel = Q_NULLPTR;
    QLabel* loggingLabel = Q_NULLPTR;
    QPushButton* modeMask = Q_NULLPTR;
    QLabel* logLabel = Q_NULLPTR;
    QLabel* avgStateLabel = Q_NULLPTR;
    QPushButton* avgStateMask = Q_NULLPTR;
    QLabel* errLabel = Q_NULLPTR;
    QPushButton* errMask = Q_NULLPTR;

    // Nav bar objects
    QPushButton* settingsButton = Q_NULLPTR;
    QPushButton* parametersButton = Q_NULLPTR;
    QPushButton* graphButton = Q_NULLPTR;

    // Data display objects
    QLabel* noLabel = Q_NULLPTR;
    QLabel* no2Label = Q_NULLPTR;
    QLabel* noxLabel = Q_NULLPTR;
    QLabel* dateTimeLabel = Q_NULLPTR;

    // Settings menu
    SettingsMenu* settingsMenu = Q_NULLPTR;

    QTimer updateTimer;

    ParametersForm* parameters = Q_NULLPTR;

    GraphForm* graphForm = Q_NULLPTR;

    // Handles the process of building UI elements for the page
    void BuildUIElements();

    // Build the status bar UI
    void BuildStatusBar();

    // Build the navigation bar
    void BuildNavBar();

    // Build the data dislay section
    void BuildDataLabels();

    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

};
#endif // MAINWINDOW_H
