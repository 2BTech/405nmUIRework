#ifndef DATAFILEMENUFORM_H
#define DATAFILEMENUFORM_H

#include <QThread>
#include <QDate>
#include <QProcess>

#include "../basepage.h"

#include "datafilemenu.h"

#include "SettingsPages/Files/savefilethread.h"
#include "SettingsPages/Files/deletefilethread.h"
#include "SettingsPages/blockingform.h"

class DataFileMenuForm : public BasePage
{
    Q_OBJECT
public:
    DataFileMenuForm();
    ~DataFileMenuForm();

    virtual void BuildUIElements() override;

signals:
    void SaveSingleFile(QString source, QString fileName, QString dest);
    void SaveCollection(QString source, QStringList fileNames, QString dest);

    void DeleteSingleFile(QString source, QString fileName);
    void DeleteCollection(QString source, QStringList fileNames);

private slots:
    void OnDeleteClicked();
    void OnSelectClicked();
    void OnSaveClicked();
    void OnRefreshClicked();

    void OnSelectFile(QString file);

    void OnSaveFileThreadFinish(bool sucess);
    void OnDeleteFileThreadFinish(bool sucess);

private:
    QLabel* currentFileLabel = Q_NULLPTR;
    QLabel* selectFileLabel = Q_NULLPTR;
    QLabel* usbLabel = Q_NULLPTR;

    QPushButton* refreshButton = Q_NULLPTR;
    QPushButton* deleteButton = Q_NULLPTR;
    QPushButton* selectButton = Q_NULLPTR;
    QPushButton* saveButton = Q_NULLPTR;

    DataFileMenu* menuForm = Q_NULLPTR;

    QString selectedFile = "Current";

    SaveFileThread* saveFile = Q_NULLPTR;
    DeleteFileThread* deleteFile = Q_NULLPTR;
    QThread* fileThread = Q_NULLPTR;

    BlockingForm* blockingForm = Q_NULLPTR;

    void UpdateUI();

    bool CheckIfUSBConnected();

    void showEvent(QShowEvent* event) override;
};

#endif // DATAFILEMENUFORM_H
