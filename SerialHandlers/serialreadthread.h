#ifndef SERIALREADTHREAD_H
#define SERIALREADTHREAD_H

#include <QThread>
#include <QMutex>
#include <QByteArray>
#include <QDebug>

// UNIX standard function definitions, close, read, write
#include <unistd.h>

class SerialReadThread : public QThread
{
    Q_OBJECT
public:
    SerialReadThread(int* pfd, QByteArray* pReadBuffer, QMutex* pReadMutex, int* pReadBufferLength, bool* pIsOpen);

signals:
    void ReadyRead();

protected:
    void run() override;

private:
    int* pfd = Q_NULLPTR;
    QByteArray* pReadBuffer = Q_NULLPTR;
    QMutex* pReadMutex = Q_NULLPTR;
    int* pReadBufferLength = Q_NULLPTR;
    bool* pIsOpen = Q_NULLPTR;
};

#endif // SERIALREADTHREAD_H
