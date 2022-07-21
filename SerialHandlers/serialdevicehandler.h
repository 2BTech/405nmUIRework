#ifndef SERIALDEVICEHANDLER_H
#define SERIALDEVICEHANDLER_H

#include <QDebug>
#include <QSerialPort>
#include <QDate>
#include <QTime>

// This class is in charge of reading in and parsing data from the 405
class SerialDeviceHandler : public QObject
{
public:
    SerialDeviceHandler();

    // Opens the serial port conection with the provided port name
    void OpenSerialPort(QString portName);

    // Moves the object and child object to the thread
    void MoveToThread(QThread* thread);

private slots:
    void OnReadyRead();

private:
    // Used to read and write with the device
    QSerialPort* serialPort = Q_NULLPTR;

    // Holds the bytes that are read in from the serial port
    QByteArray received;

    // First step in the parsing process
    void ParseReceived();

    // Attempts to parse received as a dataline
    void ParseAsDataline();
};

#endif // SERIALDEVICEHANDLER_H
