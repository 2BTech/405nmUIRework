#ifndef FILEBROWSERFORM_H
#define FILEBROWSERFORM_H

#include <QWidget>
// Used to get files in a directory
#include <QDir>
// Used to display the found files
#include <QListWidget>
// Used to get use clicks
#include <QPushButton>

#include "GlobalDefinitions.h"

class FileBrowserForm : public QWidget
{
    Q_OBJECT
public:
    FileBrowserForm();
    ~FileBrowserForm();

    void BuildUIElements();

signals:
    void Finished(QString file);

private slots:
    void OnMoveUpClicked();
    void OnMoveDownClicked();

    void OnCancelClicked();
    void OnSelectClicked();

private:
    QListWidget* fileDisplay = Q_NULLPTR;
    QPushButton* cancelButton = Q_NULLPTR;
    QPushButton* selectButton = Q_NULLPTR;
    QPushButton* moveUpButton = Q_NULLPTR;
    QPushButton* moveDownButton = Q_NULLPTR;

    QStringList fileList;

    void showEvent(QShowEvent* event) override;
};

#endif // FILEBROWSERFORM_H
