#ifndef DELETEFILETHREAD_H
#define DELETEFILETHREAD_H

#include <QDir>
#include <QFile>
#include <QDebug>

class DeleteFileThread : public QObject
{
    Q_OBJECT
public:
    DeleteFileThread();

public slots:
    void DeleteSingleFile(QString source, QString file);
    void DeleteCollection(QString sourceDir, QStringList files);

signals:
    void Finished(bool success);
};

#endif // DELETEFILETHREAD_H
