#include "serialwritethread.h"

SerialWriteThread::SerialWriteThread(int* pfd, QMutex* pWriteMutex, QByteArray* pWriteBuffer, bool* pIsOpen)
{
    this->pfd = pfd;
    this->pWriteMutex = pWriteMutex;
    this->pWriteBuffer = pWriteBuffer;
    this->pIsOpen = pIsOpen;
}

void SerialWriteThread::run()
{
    //qDebug() << "Started write thread";
    while(!pWriteBuffer->isEmpty() && *pIsOpen)
    {
        pWriteMutex->lock();
        QByteArray toWrite = pWriteBuffer->left(200);
        //qDebug() << "Before write";
        pWriteBuffer->remove(0, toWrite.length());
        //qDebug() << "After write";
        int writeResult = write(*pfd, toWrite.data(), toWrite.length());
        pWriteMutex->unlock();

        if (writeResult < 0)
        {
            qInfo("ERROR: Failed to write bytes to serial port");
        }
//        else
//        {
//            qDebug() << "Write result: " << writeResult;
//        }
    }
    emit FinishedWriting();
}
