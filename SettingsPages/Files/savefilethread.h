#ifndef SAVEFILETHREAD_H
#define SAVEFILETHREAD_H

#include <QDir>
#include <QFile>
#include <QDebug>

class SaveFileThread : public QObject
{
    Q_OBJECT
public:
    SaveFileThread();

    QString ErrorString();

public slots:
    void SaveSingleFile(QString source, QString file, QString dest);
    void SaveCollection(QString sourceDir, QStringList files, QString destDir);

signals:
    void Finished(bool success);

private:
    QString errorString = "No error";
};

#endif // SAVEFILETHREAD_H
