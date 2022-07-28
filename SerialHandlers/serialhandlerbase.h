#ifndef SERIALHANDLERBASE_H
#define SERIALHANDLERBASE_H

#include <QDebug>
#include <QQueue>
#include <QTimer>

//#include "qserialport.h"

#ifdef Q_OS_WIN
#include <QSerialPort>
#else
#include "serialport.h"
#endif

// This class serves as a base for both the serial munu and device handlers
class SerialHandlerBase : public QObject
{
public:
    SerialHandlerBase(QString name);

    // Opens the serial port conection with the provided port name
    virtual void OpenSerialPort(QString portName);
    virtual void Disconnect();

    // Moves the object and child object to the thread
    virtual void MoveToThread(QThread* thread);

protected slots:
    // Reads in each byte individually. If a new line character ('\r' or '\n') is received
    // then ParseReceived is called
    virtual void OnReadyRead();

    // Writes the next message is write queue
    void WriteNextMessage();

protected:
    // Used to read and write with the device
#ifdef Q_OS_WIN
    QSerialPort* serialPort = Q_NULLPTR;
#else
    SerialPort* serialPort = Q_NULLPTR;
#endif

    // Holds the bytes that are read in from the serial port
    QByteArray received;

    // Is the device in the process of sending messages
    bool isSendingMessage = false;

    // Queue of messages to write
    QQueue<QByteArray> writeQueue;

    // Timer used to spread out writing messages
    QTimer writeTimer;

    int bytesWritten = 0;

    QString name;

    // First step in the parsing process
    virtual void ParseReceived() = 0;

    virtual void QueueMessage(QByteArray arr);
};

#endif // SERIALHANDLERBASE_H
