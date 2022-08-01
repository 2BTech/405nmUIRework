#include "datafilemenuform.h"

DataFileMenuForm::DataFileMenuForm() : BasePage("Data Files")
{
    menuForm = new DataFileMenu();
    menuForm->BuildUIElements();
    menuForm->hide();
    connect(menuForm, &DataFileMenu::Finished, this, &DataFileMenuForm::OnSelectFile);

    saveFile = new SaveFileThread();
    connect(this, &DataFileMenuForm::SaveSingleFile, saveFile, &SaveFileThread::SaveSingleFile);
    connect(this, &DataFileMenuForm::SaveCollection, saveFile, &SaveFileThread::SaveCollection);
    connect(saveFile, &SaveFileThread::Finished, this, &DataFileMenuForm::OnSaveFileThreadFinish);

    deleteFile = new DeleteFileThread();
    connect(this, &DataFileMenuForm::DeleteSingleFile, deleteFile, &DeleteFileThread::DeleteSingleFile);
    connect(this, &DataFileMenuForm::DeleteCollection, deleteFile, &DeleteFileThread::DeleteCollection);
    connect(deleteFile, &DeleteFileThread::Finished, this, &DataFileMenuForm::OnDeleteFileThreadFinish);

    blockingForm = new BlockingForm();
    blockingForm->BuildUIElements();
    blockingForm->show();

    fileThread = new QThread(this);
    saveFile->moveToThread(fileThread);
    fileThread->start();
}

DataFileMenuForm::~DataFileMenuForm()
{
    if (currentFileLabel)
    {
        currentFileLabel->deleteLater();
        selectFileLabel->deleteLater();
        usbLabel->deleteLater();

        refreshButton->deleteLater();
        deleteButton->deleteLater();
        selectButton->deleteLater();
        saveButton->deleteLater();

        currentFileLabel = selectFileLabel = usbLabel = Q_NULLPTR;

        refreshButton = deleteButton = selectButton = saveButton = Q_NULLPTR;
    }
}

void DataFileMenuForm::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont font;
    font.setPointSize(12);

    usbLabel = new QLabel(this);
    usbLabel->setGeometry(70, 70, 160, 40);
    usbLabel->setText("No usb connected");
    usbLabel->setFont(font);

    refreshButton = new QPushButton(this);
    refreshButton->setGeometry(300, 70, 100, 40);
    refreshButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    refreshButton->setText("Refresh");
    refreshButton->setFont(font);

    currentFileLabel = new QLabel(this);
    currentFileLabel->setGeometry(70, 120, 240, 40);
    currentFileLabel->setText("Current File: Not Set");
    currentFileLabel->setFont(font);
    currentFileLabel->hide();

    selectFileLabel = new QLabel(this);
    selectFileLabel->setGeometry(70, 135, 240, 40);
    selectFileLabel->setText("Selected File: Not Set");
    selectFileLabel->setFont(font);

    deleteButton = new QPushButton(this);
    deleteButton->setGeometry(70, 230, 100, 40);
    deleteButton->setText("Delete");
    deleteButton->setFont(font);
    deleteButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(deleteButton, &QPushButton::clicked, this, &DataFileMenuForm::OnDeleteClicked);

    selectButton = new QPushButton(this);
    selectButton->setGeometry(190, 230, 100, 40);
    selectButton->setText("Select");
    selectButton->setFont(font);
    selectButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(selectButton, &QPushButton::clicked, this, &DataFileMenuForm::OnSelectClicked);

    saveButton = new QPushButton(this);
    saveButton->setGeometry(310, 230, 100, 40);
    saveButton->setText("Save");
    saveButton->setFont(font);
    saveButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(saveButton, &QPushButton::clicked, this, &DataFileMenuForm::OnSaveClicked);
}

void DataFileMenuForm::OnDeleteClicked()
{
    blockingForm->show();
    if (selectedFile == "Current")
    {
        emit DeleteSingleFile(QString(WORKING_DIR).append("datafiles/"), QString("405nm_").append(QDate::currentDate().toString("dd_MM_yy")).append(".csv"));
    }
    else if (selectedFile == "All")
    {
        emit DeleteCollection(QString(WORKING_DIR).append("datafiles/"), QDir(QString(WORKING_DIR).append("datafiles/")).entryList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot));
    }
    else
    {
        emit DeleteSingleFile(QString(WORKING_DIR).append("datafiles/"), selectedFile);
    }
}

void DataFileMenuForm::OnSelectClicked()
{
    menuForm->show();
}

void DataFileMenuForm::OnSaveClicked()
{
    blockingForm->show();
    if (selectedFile == "Current")
    {
        emit SaveSingleFile(QString(WORKING_DIR).append("datafiles/"), QString("405nm_").append(QDate::currentDate().toString("dd_MM_yy")).append(".csv"), "/media/usb/");
    }
    else if (selectedFile == "All")
    {
        emit SaveCollection(QString(WORKING_DIR).append("datafiles/"), QDir(QString(WORKING_DIR).append("datafiles/")).entryList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot), "/media/usb/");
    }
    else
    {
        emit SaveSingleFile(QString(WORKING_DIR).append("datafiles/"), selectedFile, "/media/usb/");
    }
}

void DataFileMenuForm::OnRefreshClicked()
{
    UpdateUI();
}

void DataFileMenuForm::OnSelectFile(QString file)
{
    if (!file.isEmpty())
    {
        selectedFile = file;
    }
    menuForm->close();
    show();
    UpdateUI();
}

void DataFileMenuForm::UpdateUI()
{
    if (isVisible())
    {
        if (CheckIfUSBConnected())
        {
            usbLabel->setText("<font color='green'>USB is connected</font>");
        }
        else
        {
            usbLabel->setText("<font color='red'>No USB is connected</font>");
        }
        selectFileLabel->setText("Selected File: " + selectedFile);
    }
}

bool DataFileMenuForm::CheckIfUSBConnected()
{
//    QDir mediaDir;
//    mediaDir.setPath("/media/");
//    const QStringList entryList = mediaDir.entryList(QDir::Filter::AllDirs | QDir::Filter::NoDotAndDotDot);
//    QStringList dirs;
//    for(const QString &entry : entryList)
//    {
//        dirs.append("/media/" + entry + '/');
//        //BaseLogger::Log(entry);
//    }
//    return dirs.count() > 0;

    QString prog = "/bin/bash";
    QStringList arguments = {"-c", "mount | grep media"};
    QProcess process;

    process.start(prog, arguments);
    process.waitForFinished();

    QString mountStatus = process.readAll();
    if(mountStatus.contains("usb") || mountStatus.contains("/dev/sd"))
    {
        QStringList stringList = mountStatus.split(" ");
        for(int i = 0; i < stringList.size(); i++)
        {
            //If the usb stick is on a linux pc
            if(stringList[i].contains("usb0"))
            {
                return true;
            }
            //If the usb stick is in the armadillo board
            else if(stringList[i].contains("/media"))
            {
                return true;
            }
        }
    }
    return false;
}

void DataFileMenuForm::OnSaveFileThreadFinish(bool success)
{
    blockingForm->hide();
    qDebug() << "Finished saving. Was success: " << success;
}

void DataFileMenuForm::OnDeleteFileThreadFinish(bool success)
{
    blockingForm->hide();
    qDebug() << "Finished deleting. Was success: " << success;
}

void DataFileMenuForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    UpdateUI();
}
