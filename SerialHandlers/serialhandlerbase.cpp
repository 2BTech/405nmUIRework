#include "serialhandlerbase.h"

SerialHandlerBase::SerialHandlerBase(QString name) : QObject()
{


#ifdef Q_OS_WIN
    serialPort = new QSerialPort();
    serialPort->setBaudRate(2400);
    connect(serialPort, &QSerialPort::readyRead, this, &SerialHandlerBase::OnReadyRead);
#else
    serialPort = new SerialPort();
    serialPort->setBaudRate(BaudRate::B_2400);
    connect(serialPort, &SerialPort::ReadyRead, this, &SerialHandlerBase::OnReadyRead);
#endif

    writeTimer.setInterval(200);
    connect(&writeTimer, &QTimer::timeout, this, &SerialHandlerBase::WriteNextMessage);
    writeTimer.setSingleShot(true);

    this->name = name;
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

#ifdef Q_OS_WIN
    if (serialPort->open(QIODevice::ReadWrite))
#else
    if (serialPort->openPort(QIODevice::ReadWrite))
#endif
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
#ifdef Q_OS_WIN
    serialPort->close();
#else
    serialPort->closePort();
#endif
}

//#define DEBUG_SER_WRITE
void SerialHandlerBase::WriteNextMessage()
{
    //qDebug() << "Writing message for " << name;
    if (!writeQueue.isEmpty())
    {
        //qDebug() << "Writing data for " << name;
#ifdef Q_OS_WIN
#ifdef DEBUG_SER_WRITE
        QByteArray message = writeQueue.dequeue();
        qDebug() << "Writing: " << message;
        int count = serialPort->write(message);
        if (count <= 0)
        {
            qDebug() << "Serial handler failed to write bytes";
        }
        else if (count != message.count())
        {
            qDebug() << "ERROR: Failed to write entire message: " << message;
        }
        else
        {
            bytesWritten += count;
        }

        if (!serialPort->waitForBytesWritten(-1))
        {
            qDebug() << "Failed to wait for bytes to write";
        }
        if (!serialPort->flush())
        {
            qDebug() << "Failed to flush bytes";
        }
        qDebug() << "Number of bytes written: " << bytesWritten;
#else
        serialPort->write(writeQueue.dequeue());
#endif
#else
        QByteArray message = writeQueue.dequeue();
        //qDebug() << name << " is writing: " << message;
        serialPort->writeData(message);
#endif
        writeTimer.start();
    }
    else
    {
        isSendingMessage = false;
    }
    //qDebug() << "Finished Writing message for " << name;
}

void SerialHandlerBase::OnReadyRead()
{
    //qDebug() << "Reading message for " << name;

    // Holds the most recent character
    char in;

#ifdef Q_OS_WIN
    // Continue reading bytes until all are handled
    while (serialPort->bytesAvailable())
    {
        // Read in each byte indiidually
        serialPort->read(&in, 1);
#else
    // Continue reading bytes until all are handled
    while (serialPort->available())
    {
        // Read in each byte indiidually
        serialPort->readData(&in, 1);
#endif

        // Check if this signals the end of a line
        if (in == '\r' || in == '\n')
        {
            // Parse data if some exists
            if (!received.isEmpty())
            {
                //qDebug() << "Data: " << received;
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
    //qDebug() << "Finished reading message for " << name;
}

void SerialHandlerBase::QueueMessage(QByteArray arr)
{
    writeQueue.append(arr);

    //qDebug() << arr;
    if (!isSendingMessage)
    {
        //qDebug() << "Starting write";
        isSendingMessage = true;
        WriteNextMessage();
    }

//    if (arr.length() > 40)
//    {
//        int length = arr.length() / 2;
//        QueueMessage(arr.left(length));
//        QueueMessage(arr.right(arr.length() - length));
//    }
//    else
//    {
//        writeQueue.append(arr);

//        //qDebug() << arr;
//        if (!isSendingMessage)
//        {
//            //qDebug() << "Starting write";
//            isSendingMessage = true;
//            writeTimer.start();
//        }
//    }
}
