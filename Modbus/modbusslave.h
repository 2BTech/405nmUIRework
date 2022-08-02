#ifndef MODBUSSLAVE_H
#define MODBUSSLAVE_H

#include <QThread>

#include "Modbus/modbusmessage.h"

// This is a base class for all Modbus Slave classes. It defines the functions necessary to properly function.
class ModbusSlave
{
public:
    ModbusSlave();

    // This function handles responding to a request to read output coils.
    virtual void ReadOutputCoils(unsigned short startAddress, unsigned short length) = 0;
    // This function handles responding to a request to read input coils.
    virtual void ReadInputCoils(unsigned short address, unsigned short length) = 0;
    // This function handles responding to a request to read output registers.
    virtual void ReadOutputRegisters(unsigned short address, unsigned short length) = 0;
    // This function handles responding to a request to read input registers.
    virtual void ReadInputRegisters(unsigned short address, unsigned short length) = 0;
    // This function handles parsing and processing a message to write an output coil. It reads some values from currentMessage like the value to set the coil.
    virtual void WriteOutputCoil(unsigned short address) = 0;
    // This function handles parsing and processing a message to write an output register. It reads some values from currentMessage like the value to set the register.
    virtual void WriteOutputRegister(unsigned short address) = 0;
    // This function handles parsing and processing a message to write a range of output coils. The values to set the coils is contained in currentMessage;
    virtual void WriteMultipleOutputCoils(unsigned short address, unsigned short length) = 0;
    // This function handles parsing and processing a message to write a range of output coils. The values to set the coils is contained in currentMessage.
    virtual void WriteMultipleOutputRegisters(unsigned short address, unsigned short length) = 0;
    // This function is used to move all child objects that do not have their parent set to this object. After moving those objects, this object is then moved to the thread.
    virtual void MoveToThread(QThread * thread) = 0;

protected:
    // This function is used to handle the initial steps in parsing a received message.
    virtual void HandleMessage(ModbusMessage * message) = 0;
};

#endif // MODBUSSLAVE_H
