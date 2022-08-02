#ifndef MODBUSASCIIBASE_H
#define MODBUSASCIIBASE_H

// Used to establish a connection
#include <QSerialPort>

// Inheritance
#include "../modbusbase.h"
// Modbus message that is used by the ASCII Protocol
#include "modbusasciimessage.h"

// This class extends ModbusBases by implementing the common functions for the ASCII protocol. It also defines serialPort and currentMessage.
class ModbusASCIIBase : public ModbusBase
{
public:
    ModbusASCIIBase();

    // If the serial port is not connected, the provided values are set in the serial port. It does not establish a connection.
    void SetSerialPortInformation(QString portname, int baudrate);
    // If there is not an existing connection, the serial port attempts to open a connection. Returns true if
    // the serial port has an active connection.
    virtual bool OpenConnection() override;
    // Closes an existing serial connection if one exists.
    virtual void CloseConnection() override;
    // Returns true if the serial port has an existing connection.
    bool IsConnected() override;

    // Gets the port name form the serial poirt.
    QString PortName();
    // Gets the baud rate from the serial port.
    int BaudRate();

    // Moves all child objects to a thread and then moves this to a thread.
    void MoveToThread(QThread * thread) override;

protected slots:
    // Connects to the serial ports signal of the same name. Reads in
    // bytes from the serial port
    void ReadyRead() override;

protected:
    // Serialport used for establishing and handling a connection
    QSerialPort serialPort;
    // The current modbus message. Messages are read into this object.
    ModbusASCIIMessage currentMessage;

    // This function is the first step in the process of parsing a received message. It should make sure the
    // message is destined for this device and will then send it to the proper function.
    virtual void HandleMessage(ModbusMessage* message) = 0;
};

#endif // MODBUSASCIIBASE_H
