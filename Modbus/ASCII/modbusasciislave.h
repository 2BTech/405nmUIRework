#ifndef MODBUSASCIISLAVE_H
#define MODBUSASCIISLAVE_H

#include "modbusasciimessage.h"
#include "Modbus/modbusslave.h"
#include "modbusasciibase.h"

#define DEBUG_MODBUS_ASCII_SLAVE

// This class is used to provide the functionality of a modbus slave for the ASCII protocol. It handles sending, receiving, and parsing messages.
class ModbusASCIISlave : public ModbusASCIIBase , public ModbusSlave {
public:
    ModbusASCIISlave();
    ModbusASCIISlave(ModbusASCIISlave * other);

    // Gets unitAddress.
    unsigned char UnitAddress();
    // Sets unitAddress to the given value.
    void SetUnitAddress(unsigned char unitAddress);

    // Creates and sends a message to the master with the values of the coils starting at startAddress with length values.
    virtual void ReadOutputCoils(unsigned short startAddress, unsigned short length) override;
    // Creates and sends a message to the master with the values of the coils starting at startAddress with length values.
    virtual void ReadInputCoils(unsigned short address, unsigned short length) override;
    // Creates and sends a message to the master with the values of the coils starting at startAddress with length values.
    virtual void ReadOutputRegisters(unsigned short address, unsigned short length) override;
    // Creates and sends a message to the master with the values of the coils starting at startAddress with length values.
    virtual void ReadInputRegisters(unsigned short address, unsigned short length) override;
    // This function sets the coil at address with the value from the receved message. It will rebuild the
    // message to send back. It does not just echeo it back to make sure the state of the device actually changes.
    virtual void WriteOutputCoil(unsigned short address) override;
    // This function sets the regster at address with the value from the received message. It will
    // rebuild the message to send back. It does not just echeo it back to make sure the state of the device actually changes.
    virtual void WriteOutputRegister(unsigned short address) override;
    // This function sets a range of coils starting at address for length values. It will get the values from the bytes
    // in the received message. It does not just echeo it back to make sure the state of the device actually changes.
    virtual void WriteMultipleOutputCoils(unsigned short address, unsigned short length) override;
    // This function sets a range of registers starting at address for length values. It will get the values
    // from the bytes in the received message. It does not just echeo it back to make sure the state of the device actually changes.
    virtual void WriteMultipleOutputRegisters(unsigned short address, unsigned short length) override;

    // This function moves all of the child objects in the class into thread. After moving all child objects, this object is moved.
    virtual void MoveToThread(QThread* thread) override;

#if TEST_BUILD
    ModbusASCIIMessage * TestRequest()
    {
        return &response;
    }

    void TestHandleMessage(ModbusMessage * message)
    {
        HandleMessage(message);
    }
#endif

protected:
    // Function build reposnes into this instance of ModbusASCIIMessage.
    ModbusASCIIMessage response;

    // This function is the first step in handling a received message. It makes sure the received function is
    // intended for this device and sends the message to the proper function.
    virtual void HandleMessage(ModbusMessage* message) override;
};

#endif // MODBUSASCIISLAVE_H
