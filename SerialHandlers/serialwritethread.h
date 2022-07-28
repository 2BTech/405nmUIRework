#ifndef SERIALWRITETHREAD_H
#define SERIALWRITETHREAD_H

#include <QThread>
#include <QMutex>
#include <QByteArray>
#include <QDebug>

// UNIX standard function definitions, close, read, write
#include <unistd.h>

class SerialWriteThread : public QThread
{
    Q_OBJECT
public:
    SerialWriteThread(int* pfd, QMutex* pWriteMutex, QByteArray* pWriteBuffer, bool* pIsOpen);

signals:
    void FinishedWriting();

protected:
    void run() override;

private:
    int* pfd = Q_NULLPTR;
    QMutex* pWriteMutex = Q_NULLPTR;
    QByteArray* pWriteBuffer = Q_NULLPTR;
    bool* pIsOpen = Q_NULLPTR;
};

#endif // SERIALWRITETHREAD_H
