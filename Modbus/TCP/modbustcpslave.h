#ifndef MODBUSTCPSLAVE_H
#define MODBUSTCPSLAVE_H

// Used for communicating with the master
#include <QTcpSocket>
// Used to listen for incomming connections.
#include <QTcpServer>

// Inheritance, Base class for all modbus tcp classes
#include "modbustcpbase.h"
// Inheritance, Bsae class for all modbus slaves
#include "Modbus/modbusslave.h"
// Message class for the TCP modbus protocol
#include "modbustcpmessage.h"
// Collection of useful modbus functions
#include "Modbus/modbushelpers.h"

//#define DEBUG_MODBUS_TCP_SLAVE

// This class is used to provide the functionality of a modbus slave for the TCP protocol. It handles
// sending, receiving, and parsing messages. Only supports 1 connection at a time.
class ModbusTCPSlave  : public ModbusTCPBase, public ModbusSlave
{
    Q_OBJECT
public:
    ModbusTCPSlave();

    // Gets unitAddress.
    unsigned char UnitAddress();
    // Sets unitAddress to the given value.
    void SetUnitAddress(unsigned char unitAddress);

    // Creates and sends a message to the master with the values of the coils starting at startAddress with length values.
    virtual void ReadOutputCoils(unsigned short startAddress, unsigned short length) override;
    // Creates and sends a message to the master with the values of the coils starting at startAddress with length values.
    virtual void ReadInputCoils(unsigned short address, unsigned short length) override;
    // Creates and sends a message to the master with the values of the registers starting at startAddress with length values.
    virtual void ReadOutputRegisters(unsigned short address, unsigned short length) override;
    // Creates and sends a message to the master with the values of the registers starting at startAddress with length values.
    virtual void ReadInputRegisters(unsigned short address, unsigned short length) override;
    // This function sets the coil at address with the value from the received message. It will rebuild the message to send
    // back. It does not just echeo it back to make sure the state of the device actually changes.
    virtual void WriteOutputCoil(unsigned short address) override;
    // This function sets the regster at address with the value from the received message. It will rebuild the message
    // to send back. It does not just echeo it back to make sure the state of the device actually changes.
    virtual void WriteOutputRegister(unsigned short address) override;
    // This function sets a range of coils starting at address for length values. It will get the values from the bytes in the
    // received message. It does not just echeo it back to make sure the state of the device actually changes.
    virtual void WriteMultipleOutputCoils(unsigned short address, unsigned short length) override;
    // Ths function sets a reage a registers at address for length to the connected slave. It will get the register values
    // from currentMessage. It rebuils the message instead of echoing back currentMessage to make sure the write was successful.
    virtual void WriteMultipleOutputRegisters(unsigned short address, unsigned short length) override;

    // This function moves all child objects to thread and then moves itself.
    void MoveToThread(QThread* thread) override;

#if TEST_BUILD
    ModbusTCPMessage * TestRequest()
    {
        return &response;
    }

    void TestHandleMessage(ModbusTCPMessage * message)
    {
        HandleMessage(message);
    }
#endif

    // This function sets the connection information for the tcp server. If the server is already listening,
    // then it will sop the server and attempt to re-start it.
    Q_INVOKABLE void SetConnectionInformation(QString targetIP, unsigned short port) override;
    // // Returns true if the tcp server exitst and is listening.
    bool IsConnected() override;
    // If the tcp server is not listening, it attempts to open a connection with the values stores in the
    // class. This function returns true if the trcp server has an active connection.
    virtual bool OpenConnection() override;
    // Stops the server if it is listening.
    void CloseConnection() override;

signals:
    // This signal is triggered when the server accepts a new cnnection. It includes the ip address of the connected devivce.
    void NewConnection(QString peerIP);
    // This signal is triggered when a conection is lost/closed. This refers to the tcp socket connection and not the tcp server.
    void ClosedConnection();

protected slots:
    // This function is triggered when the tcp server receives an incomming connection. If there is already an
    // established connection, the incomming one is rejected. If there is not an existing connection, then it
    // takes the incomming connection and sets it up to work with the class.
    void OnPeerConnect();
    // This function is triffered when the current connection encounters an error. Logs the encoutered error.
    void OnPeerSocketError(QTcpSocket::SocketError);
    // This function is triggered when the current connection disconnects. This handles disposing the socket and allowing
    // the server to start accepting connections.
    void OnPeerDisconnect();

    // This function handles reading data from the current connection. It adds the received bytes to the current message. Once the message is complete, it is sent to HandleMessage.
    void ReadyRead() override;

protected:
    // Function build reposnes into this instance of ModbusRTUMessage.
    ModbusTCPMessage response;

    // TCP server that is used to listen for incomming connections.
    QTcpServer* tcpServer = Q_NULLPTR;
    // Holds the current connection.
    QTcpSocket* curConn = Q_NULLPTR;

    // This function tries to convert a generic modbus message into the specific tcp version. If
    // successful, it is sent to the next handle message functon.
    virtual void HandleMessage(ModbusMessage* message) override;
    // This function is the first step in processing a received messsage. It makes sure that
    // the message should be processed by this device. If so, it is sent to the proper handling function.
    virtual void HandleMessage(ModbusTCPMessage* message) override;

    // Used to build and write an error resposne
    void WriteError(unsigned char functionCode, unsigned char exceptionCode);
};

#endif // MODBUSTCPSLAVE_H
