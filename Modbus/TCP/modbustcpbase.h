#ifndef MODBUSTCPBASE_H
#define MODBUSTCPBASE_H

// Inheritance
#include "../modbusbase.h"
// Modbus message that is used by the TCP Protocol
#include "modbustcpmessage.h"

// This class extends ModbusBase by implementing the common functions for the TCP protocol. It defines peerIP, port, targetIP, and currentMessage.
class ModbusTCPBase  : public ModbusBase
{
public:
    ModbusTCPBase();

    // If the serial port is not connected, the provided values are set in the tcp server or socket. It
    // does not establish a connection.
    Q_INVOKABLE virtual void SetConnectionInformation(QString targetIP, unsigned short port) = 0;
    // Returns true if there is an existing connection.
    virtual bool IsConnected() = 0;
    // If there is not an existing connection, the tcp server or socket attempts to open a connection. Returns
    // true if the tcp server or socket has an active connection.
    virtual bool OpenConnection() = 0;
    // Closes an existing connection if one exists.
    virtual void CloseConnection() = 0;

    // Gets peerIP.
    QString PeerIP();
    // Gets port.
    unsigned short Port();
    // Gets targetIP.
    QString TargetIP();

protected:
    // IP address of the peer connection
    QString peerIP = "127.0.0.1";
    // Port the connection is over
    unsigned short port = 50200;
    // Target port for the connection
    QString targetIP = "127.0.0.1";
    // Current message being handled by the class
    ModbusTCPMessage currentMessage;

    // This function is the first step in the process of parsing a received message. It should make sure the message
    // is destined for this device and will then send it to the proper function.
    virtual void HandleMessage(ModbusTCPMessage * message) = 0;
};

#endif // MODBUSTCPBASE_H
