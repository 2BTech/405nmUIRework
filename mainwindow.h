#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Refernce to UI objects
    QFrame* statusLine;
    QFrame* navigationLine;
    QFrame* navSepLine;

    // Handles the process of building UI elements for the page
    void BuildUIElements();

    // Build the status bar UI


    // Build the navigation bar

    // Build the data dislay section

};
#endif // MAINWINDOW_H
