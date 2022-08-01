#include "deletefilethread.h"

DeleteFileThread::DeleteFileThread()
{

}

void DeleteFileThread::DeleteSingleFile(QString source, QString file)
{
    QFile fileObj;
    if (fileObj.remove(source.append(file)))
    {
        errorString = "No Error";
        emit Finished(true);
    }
    else
    {
        errorString = fileObj.errorString();
        emit Finished(false);
    }
}

void DeleteFileThread::DeleteCollection(QString source, QStringList files)
{
    errorString = "";

    QFile fileObj;
    bool deletedAll = true;
    const QStringList toSave = files;
    for (const QString &file: toSave)
    {
        if (!fileObj.remove(source + file))
        {
            deletedAll = false;
            errorString += " " + fileObj.errorString();
        }
    }
    emit Finished(deletedAll);
}

QString DeleteFileThread::GetErrorString()
{
    return errorString;
}
