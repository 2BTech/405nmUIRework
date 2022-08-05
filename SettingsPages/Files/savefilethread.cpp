#include "savefilethread.h"

SaveFileThread::SaveFileThread()
{

}

void SaveFileThread::SaveSingleFile(QString source, QString file, QString dest)
{
    qDebug() << "Started saving single file from " << source << " to " << dest;
    QDir destDir(dest);
    if (!destDir.exists() && !destDir.mkpath(dest))
    {
        qDebug() << "ERROR: Failed to create dest dir file";
        errorString = "Failed to create dest dir";
        emit Finished(false);
    }
    else
    {
        QFile fileObj;
        if (fileObj.copy(source.append(file), dest.append(file)))
        {
            qDebug() << "Successfully saved single file";
            errorString = "No error";
            emit Finished(true);
        }
        else
        {
            qDebug() << "Failed to save single file: " << errorString;
            errorString = fileObj.errorString();
            emit Finished(false);
        }
    }
}

void SaveFileThread::SaveCollection(QString source, QStringList files, QString dest)
{
    qDebug() << "Started saving collection from " << source << " to " << dest;
    QDir destDir(dest);
    if (!destDir.exists() && !destDir.mkpath(dest))
    {
        qDebug() << "ERROR: Failed to create dest dir file";
        errorString = "Failed to create dest dir";
        emit Finished(false);
    }
    else
    {
        QFile fileObj;
        bool savedAll = true;
        const QStringList toSave = files;
        for (const QString &file: toSave)
        {
            if (!fileObj.copy(source + file, dest + file))
            {
                qDebug() << "Failed to save file: " << file << " :  " << fileObj.errorString();
                errorString += fileObj.errorString() + " ";
                savedAll = false;
            }
        }
        qDebug() << "Finished saving collection. Saved all: " << savedAll;
        emit Finished(savedAll);
    }
}

QString SaveFileThread::ErrorString()
{
    return errorString;
}
