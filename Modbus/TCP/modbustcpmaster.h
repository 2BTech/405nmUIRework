#ifndef MODBUSTCPMASTER_H
#define MODBUSTCPMASTER_H

// Used to establish a connection with the target device.
#include <QTcpSocket>

// Inheritance, defines functions and values that modbus TCP must implement
#include "modbustcpbase.h"
// Inheritance, defines functions that modbus masters must implement
#include "Modbus/modbusmaster.h"

// This class is used to provide the functionality of a modbus master for the RTU protocol. It handles sending, receiving, and parsing messages.
class ModbusTCPMaster : public ModbusTCPBase, public ModbusMaster
{
public:
    ModbusTCPMaster();

public slots:
    // Sends a request to the slaves to read output coils starting at startAddress for length.
    virtual void ReadOutputCoils(unsigned short startAddress, unsigned short length) override;
    // Sends a request to the slaves to read input coils starting at startAddress for length.
    virtual void ReadInputCoils(unsigned short address, unsigned short length) override;
    // Sends a request to the slaves to read output registers starting at startAddress for length.
    virtual void ReadOutputRegisters(unsigned short address, unsigned short length) override;
    // Sends a request to the slaves to read input registers starting at startAddress for length.
    virtual void ReadInputRegisters(unsigned short address, unsigned short length) override;
    // Sends a request to the slaves to set an output coil.
    virtual void WriteOutputCoil(unsigned short address) override;
    // Sends a request to the slaves to set an output register.
    virtual void WriteOutputRegister(unsigned short address) override;
    // Sends a request to the slaves to set a range of output coils on the slave.
    virtual void WriteMultipleOutputCoils(unsigned short address, unsigned short length) override;
    // Sends a request to the slaves to set a range of output registers on the slave.
    virtual void WriteMultipleOutputRegisters(unsigned short address, unsigned short length) override;

    // If there is not an established connection, the values are saved in the class. This does not open a connection.
    void SetConnectionInformation(QString targetIP, unsigned short port) override;
    // Returns true if there is an active existing connection.
    bool IsConnected() override;
    // This function attempts to open a connection with the values saved in the class. If there is an existing connection, this immediately returns false. Else, returns true if the connction was successfully made.
    bool OpenConnection() override;
    // If there is an exsting connection, it is closed.
    void CloseConnection() override;

    // Moves all child objects to the thread and then moves itself.
    void MoveToThread(QThread* thread) override;

#if TEST_BUILD
    ModbusTCPMessage * TestRequest()
    {
        return &request;
    }

    void SetTransactionID(unsigned short nTID)
    {
        transactionID = nTID;
    }

    void TestHandleMessage(ModbusTCPMessage * message)
    {
        HandleMessage(message);
    }
#endif

protected slots:
    // This function is triggered when the socket receives any data. It reads in the data from the socket and adds it to the current message. If the message is complete, it is passed to HandleMessage.
    void ReadyRead() override;

protected:
    // Messages that are about to be sent are built into this. This allows for proper handling of responses.
    ModbusTCPMessage request;
    // Id for the message
    unsigned short transactionID = 0;
    // Referece to the tcp socket that is used for the current connection.
    QTcpSocket* tcpSocket = Q_NULLPTR;

    // This function attempts to convert message into a ModbusTCPMessage. If successful, it is passed to the next HandleMessage function.
    virtual void HandleMessage(ModbusMessage* message) override;
    //  This function is the first step in the process of parsing a received message. It should make sure the message is destined for this device and will then send it to the proper function.
    virtual void HandleMessage(ModbusTCPMessage* message) override;

    // This function parses the received message and saves the value of the output coils.
    virtual void ParseReadOutputCoils(ModbusMessage* response) override;
    // This function parses the received message and saves the value of the input coils.
    virtual void ParseReadInputCoils(ModbusMessage* response) override;
    // This function parses the received message and saves the value of the output registers.
    virtual void ParseReadOutputRegisters(ModbusMessage* response) override;
    // This function parses the received message and saves the value of the input registers.
    virtual void ParseReadInputRegisters(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This function checks if received matches what was sent.
    virtual void ParseWriteOutputCoil(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This function checks if received matches what was sent.
    virtual void ParseWriteOutputRegister(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This function checks if received matches what was sent.
    virtual void ParseWriteMultipleOutputCoils(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This function checks if received matches what was sent.
    virtual void ParseWriteMultipleOutputRegisters(ModbusMessage* response) override;
};

#endif // MODBUSTCPMASTER_H
