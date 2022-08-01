#include "deletefilethread.h"

DeleteFileThread::DeleteFileThread()
{

}

void DeleteFileThread::DeleteSingleFile(QString source, QString file)
{
    emit Finished(QFile::remove(source.append(file)));
}

void DeleteFileThread::DeleteCollection(QString source, QStringList files)
{
    bool deletedAll = true;
    const QStringList toSave = files;
    for (const QString &file: toSave)
    {
        if (!QFile::remove(source + file))
        {
            deletedAll = false;
        }
    }
    emit Finished(deletedAll);
}
