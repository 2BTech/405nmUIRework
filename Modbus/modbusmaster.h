#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include "modbusbase.h"

class ModbusMaster
{
public:
    ModbusMaster();

public slots:
    // Sends a request to the slaves to read output coils starting at startAddress for length.
    virtual void ReadOutputCoils(unsigned short startAddress, unsigned short length) = 0;
    // Sends a request to the slaves to read input coils starting at startAddress for length.
    virtual void ReadInputCoils(unsigned short address, unsigned short length) = 0;
    // Sends a request to the slaves to read output registers starting at startAddress for length.
    virtual void ReadOutputRegisters(unsigned short address, unsigned short length) = 0;
    // Sends a request to the slaves to read input registers starting at startAddress for length.
    virtual void ReadInputRegisters(unsigned short address, unsigned short length) = 0;
    // Sends a request to the slaves to set an output coil.
    virtual void WriteOutputCoil(unsigned short address) = 0;
    // Sends a request to the slaves to set an output register.
    virtual void WriteOutputRegister(unsigned short address) = 0;
    // Sends a request to the slaves to set a range of output coils on the slave.
    virtual void WriteMultipleOutputCoils(unsigned short address, unsigned short length) = 0;
    // Sends a request to the slaves to set a range of output registers on the slave.
    virtual void WriteMultipleOutputRegisters(unsigned short address, unsigned short length) = 0;

signals:

protected slots:

protected:
    // Number of coils to requent when pinging the slave
    int pingCoilsCount = 0;
    // ID for the message
    unsigned short messageID = 0;

    // This function is the first step in parsing a received message. It determines if the message is for this
    // device. If so, it sends the message to the proper parse function.
    virtual void HandleMessage(ModbusMessage* message) = 0;

    // Parses a response to requesting output coils. Updates the values in the modbus datatable.
    virtual void ParseReadOutputCoils(ModbusMessage* response) = 0;
    // Parses a response to requesting input coils. Updates the values in the modbus datatable.
    virtual void ParseReadInputCoils(ModbusMessage* response) = 0;
    // Parses a response to requesting output registers. Updates the values in the modbus datatable.
    virtual void ParseReadOutputRegisters(ModbusMessage* response) = 0;
    // Parses a response to requesting input registers. Updates the values in the modbus datatable.
    virtual void ParseReadInputRegisters(ModbusMessage* response) = 0;
    // Parses a response to requesting input registers.
    virtual void ParseWriteOutputCoil(ModbusMessage* response) = 0;
    // Parses a response to requesting input registers.
    virtual void ParseWriteOutputRegister(ModbusMessage* response) = 0;
    // Parses a response to requesting input registers.
    virtual void ParseWriteMultipleOutputCoils(ModbusMessage* response) = 0;
    // Parses a response to requesting input registers.
    virtual void ParseWriteMultipleOutputRegisters(ModbusMessage* response) = 0;
};

#endif // MODBUSMASTER_H
