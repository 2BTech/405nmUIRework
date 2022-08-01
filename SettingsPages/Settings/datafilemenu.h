#ifndef DATAFILEMENU_H
#define DATAFILEMENU_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>

#include "SettingsPages/Files/filebrowserform.h"

class DataFileMenu : public QWidget
{
    Q_OBJECT
public:
    DataFileMenu();
    ~DataFileMenu();

    void BuildUIElements();

signals:
    void Finished(QString selection);

protected slots:
    void OnSelectCurrentClicked();
    void OnSelectAllClicked();
    void OnBrowseClicked();
    void OnCancelClicked();

    void OnFileBrowserFinish(QString file);

private:
    QPushButton* currentButton = Q_NULLPTR;
    QPushButton* allButton = Q_NULLPTR;
    QPushButton* browseButton = Q_NULLPTR;
    QPushButton* cancelButton = Q_NULLPTR;

    FileBrowserForm* fileBrowser = Q_NULLPTR;
};

#endif // DATAFILEMENU_H
