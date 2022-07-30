#ifndef DATAFILEMENUFORM_H
#define DATAFILEMENUFORM_H

#include "../basepage.h"

#include "datafilemenu.h"

class DataFileMenuForm : public BasePage
{
    Q_OBJECT
public:
    DataFileMenuForm();
    ~DataFileMenuForm();

    virtual void BuildUIElements();

private slots:
    void OnDeleteClicked();
    void OnSelectClicked();
    void OnSaveClicked();
    void OnRefreshClicked();

private:
    QLabel* currentFileLabel = Q_NULLPTR;
    QLabel* selectFileLabel = Q_NULLPTR;
    QLabel* usbLabel = Q_NULLPTR;

    QPushButton* refreshButton = Q_NULLPTR;
    QPushButton* deleteButton = Q_NULLPTR;
    QPushButton* selectButton = Q_NULLPTR;
    QPushButton* saveButton = Q_NULLPTR;

    DataFileMenu* menuForm = Q_NULLPTR;
};

#endif // DATAFILEMENUFORM_H
