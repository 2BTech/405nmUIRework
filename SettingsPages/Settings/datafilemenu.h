#ifndef DATAFILEMENU_H
#define DATAFILEMENU_H

#include <QWidget>
#include <QPushButton>

class DataFileMenu : public QWidget
{
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

private:
    QPushButton* currentButton = Q_NULLPTR;
    QPushButton* allButton = Q_NULLPTR;
    QPushButton* browseButton = Q_NULLPTR;
    QPushButton* cancelButton = Q_NULLPTR;
};

#endif // DATAFILEMENU_H
