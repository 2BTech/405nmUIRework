#include "savefilethread.h"

SaveFileThread::SaveFileThread()
{

}

void SaveFileThread::SaveSingleFile(QString source, QString file, QString dest)
{
    QDir destDir(dest);
    if (!destDir.exists() && !destDir.mkpath(dest))
    {
        qDebug() << "ERROR: Failed to create dest dir file";
        emit Finished(false);
    }
    else
    {
        emit Finished(QFile::copy(source.append(file), dest.append(file)));
    }
}

void SaveFileThread::SaveCollection(QString source, QStringList files, QString dest)
{
    QDir destDir(dest);
    if (!destDir.exists() && !destDir.mkpath(dest))
    {
        qDebug() << "ERROR: Failed to create dest dir file";
        emit Finished(false);
    }
    else
    {
        bool savedAll = true;
        const QStringList toSave = files;
        for (const QString &file: toSave)
        {
            if (!QFile::copy(source + file, dest + file))
            {
                savedAll = false;
            }
        }
        emit Finished(savedAll);
    }
}
