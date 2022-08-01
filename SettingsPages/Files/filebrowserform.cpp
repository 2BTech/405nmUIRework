#include "filebrowserform.h"

FileBrowserForm::FileBrowserForm() : QWidget()
{
    //Set the background to white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(480, 277);
}

FileBrowserForm::~FileBrowserForm()
{
    if (fileDisplay)
    {
        fileDisplay->deleteLater();

        cancelButton->deleteLater();
        selectButton->deleteLater();
        moveDownButton->deleteLater();
        moveUpButton->deleteLater();

        fileDisplay = Q_NULLPTR;

        cancelButton = selectButton = moveDownButton = moveUpButton = Q_NULLPTR;
    }
}

void FileBrowserForm::BuildUIElements()
{
    int tempX = 480 / 10;
    int tempY = 277 / 10;

    fileDisplay = new QListWidget(this);
    fileDisplay->setGeometry(tempX / 2, tempY * 5 / 2, tempX * 8, tempY * 11 / 2);
    fileList = QStringList() << "00_00_00.csv" << "01_01_01.csv" << "02_02_02.csv" << "03_03_03.csv";
    fileDisplay->addItems(fileList);
    fileDisplay->show();

    //Up Button
    moveUpButton = new QPushButton(this);
    moveUpButton->setGeometry(420, 90, 40, 40);
    moveUpButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(moveUpButton, &QPushButton::clicked, this, &FileBrowserForm::OnMoveUpClicked);

    // Down button
    moveDownButton = new QPushButton(this);
    moveDownButton->setGeometry(420, 140, 40, 40);
    moveDownButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(moveDownButton, &QPushButton::clicked, this, &FileBrowserForm::OnMoveDownClicked);

    QFont font;
    font.setPointSize(12);

    //Close button
    cancelButton = new QPushButton(this);
    cancelButton->setGeometry(110, 230, 100, 40);
    cancelButton->setText("Cancel");
    cancelButton->setFont(font);
    cancelButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(cancelButton, &QPushButton::clicked, this, &FileBrowserForm::OnCancelClicked);

    //Select button
    selectButton = new QPushButton(this);
    selectButton->setGeometry(250, 230, 100, 40);
    selectButton->setText("Select");
    selectButton->setFont(font);
    selectButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a);");
    connect(selectButton, &QPushButton::clicked, this, &FileBrowserForm::OnSelectClicked);
}

void FileBrowserForm::OnMoveUpClicked()
{
    //Get previous index
    int currentIndex = fileDisplay->currentRow();
    //If there are no items, there is nothing to do
    if(fileList.count() > 0)
    {
        //Decrease the index by 1 to go up the list. If at beginning of list, move to bottom
        if(currentIndex != -1)
        {
            currentIndex = currentIndex - 1 >= 0 ? currentIndex - 1 : fileList.count() - 1;
        }
        //If there is no selected index, set index to first item
        else
        {
            currentIndex = 0;
        }
        //Update list widget index
        fileDisplay->setCurrentRow(currentIndex);
    }
}

void FileBrowserForm::OnMoveDownClicked()
{
    //Get previous index
    int currentIndex = fileDisplay->currentRow();
    //If there are no items, there is nothing to do
    if(fileList.count() > 0)
    {
        //Increase the index by 1 to go down the list. If at end of list, move back to top
        if(currentIndex != -1)
        {
            currentIndex = currentIndex + 1 < fileList.count() ? currentIndex + 1 : 0;
        }
        //If there is no selected index, set index to first item
        else
        {
            currentIndex = 0;
        }
        //Update list widget index
        fileDisplay->setCurrentRow(currentIndex);
    }
}

void FileBrowserForm::OnCancelClicked()
{
    emit Finished("");
}

void FileBrowserForm::OnSelectClicked()
{
    //Make sure there are items to select
    if(fileList.count() > 0 &&
        //Make sure an item is actuall selected
        fileDisplay->currentRow() != -1 &&
            //Make sure the selected item is inbounds of the list
            fileDisplay->count() > fileDisplay->currentRow())
    {
        emit Finished(fileList.at(fileDisplay->currentRow()));
    }
}

void FileBrowserForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    //Set the file list widget's content to be all of the files in the selected directory
    QDir dir;
    //Data logs are stored in a directory where the its the device name with Logs added to it
    //IE: For the 405nm device, the data logs folder would be 405nmLogs
    dir.setPath(QString(WORKING_DIR).append('/').append("datafiles/"));
    //dir.setPath(QDir::currentPath().append("/").append(selectedDeviceName).append("Logs"));
    //Clear previous items
    fileList.clear();
    fileDisplay->clear();
    //Get only data files sorted by their last edit time
    fileList = dir.entryList(QDir::Filter::Files, QDir::SortFlag::Time);
    //Add new items to list widget
    fileDisplay->addItems(fileList);
}
