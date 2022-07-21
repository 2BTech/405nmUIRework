#include "serialhandlerbase.h"

SerialHandlerBase::SerialHandlerBase() : QObject()
{
    serialPort = new QSerialPort();
    serialPort->setBaudRate(2400);
    connect(serialPort, &QSerialPort::readyRead, this, &SerialHandlerBase::OnReadyRead);

    writeTimer.setInterval(200);
    connect(&writeTimer, &QTimer::timeout, this, &SerialHandlerBase::WriteNextMessage);
}

void SerialHandlerBase::MoveToThread(QThread* thread)
{
    serialPort->moveToThread(thread);
    writeTimer.moveToThread(thread);
    moveToThread(thread);
}

void SerialHandlerBase::OpenSerialPort(QString portName)
{
    serialPort->setPortName(portName);

    if (serialPort->open(QIODevice::ReadWrite))
    {
        qDebug() << "Opened serial device handler at " << portName;
    }
    else
    {
        qDebug() << "Failed to open serial device handler at " << portName;
    }
}

void SerialHandlerBase::Disconnect()
{
    serialPort->close();
}

#define DEBUG_SER_WRITE
void SerialHandlerBase::WriteNextMessage()
{
    if (!writeQueue.isEmpty())
    {
#ifdef DEBUG_SER_WRITE
        QByteArray message = writeQueue.dequeue();
        serialPort->write(message);
#else
        serialPort.write(writeQueue.dequeue());
#endif
        writeTimer.start();
    }
    else
    {
        isSendingMessage = false;
    }
}

void SerialHandlerBase::OnReadyRead()
{
    // Holds the most recent character
    char in;

    // Continue reading bytes until all are handled
    while (serialPort->bytesAvailable())
    {
        // Read in each byte indiidually
        serialPort->read(&in, 1);

        // Check if this signals the end of a line
        if (in == '\r' || in == '\n')
        {
            // Parse data if some exists
            if (!received.isEmpty())
            {
                ParseReceived();
                received.clear();
            }
        }
        else
        {
            received.append(in);
        }
    }
    // Prevent an overflow condition if data is not being properly received. Max line size is around 100
    if (received.count() > 1000)
    {
        received.clear();
    }
}

void SerialHandlerBase::QueueMessage(QByteArray arr)
{
    if (arr.length() > 40)
    {
        int length = arr.length() / 2;
        QueueMessage(arr.left(length));
        QueueMessage(arr.right(arr.length() - length));
    }
    else
    {
        writeQueue.append(arr);

        qDebug() << arr;
        if (!isSendingMessage)
        {
            isSendingMessage = true;
            WriteNextMessage();
        }
    }
}
