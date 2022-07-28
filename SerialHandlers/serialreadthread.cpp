#include "serialreadthread.h"

SerialReadThread::SerialReadThread(int* pfd, QByteArray* pReadBuffer, QMutex* pReadMutex, int* pReadBufferLength, bool* pIsOpen)
{
    this->pfd = pfd;
    this->pReadBuffer = pReadBuffer;
    this->pReadMutex = pReadMutex;
    this->pReadBufferLength = pReadBufferLength;
    this->pIsOpen = pIsOpen;
}

void SerialReadThread::run()
{
    char readBuffer[256];
    memset(readBuffer, 0, 256);

    while (*pIsOpen)
    {
        int count = read(*pfd, &readBuffer[0], 255);

        if (count > 0)
        {
            readBuffer[count] = 0;
            qDebug() << ("Read in data");
            pReadMutex->lock();
            pReadBuffer->append(readBuffer);

            qDebug() << "Local read buffer: " << readBuffer;
            qDebug() << "Global read buffer: " << *pReadBuffer;

            // ToDo, replace with ring/circular buffer
            if (pReadBuffer->length() > *pReadBufferLength)
            {
                qDebug() << ("Shrunk read buffer");
                pReadBuffer->remove(0, pReadBuffer->length() - *pReadBufferLength);
            }

            pReadMutex->unlock();
            emit ReadyRead();
        }
    }
    //emit QThread::finished();
}
