#ifndef MODBUSRTUMASTER_H
#define MODBUSRTUMASTER_H

#include "modbusrtubase.h"
#include "modbusrtumessage.h"
#include "Modbus/modbusmaster.h"

// This class is used to provide the functionality of a modbus master for the RTU protocol. It handles sending, receiving, and parsing messages.
class ModbusRTUMaster : public ModbusRTUBase, public ModbusMaster {
public:
    ModbusRTUMaster();

    // Gets the unitAddress of the target device.
    unsigned char UnitAddress();

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

    // Moves all child objects to a thread and then moves this to a thread.
    virtual void MoveToThread(QThread * thread) override;

#if TEST_BUILD
    ModbusRTUMessage* TestRequest()
    {
        return &request;
    }
#endif

protected:
    ModbusRTUMessage request;

#if TEST_BUILD
    ModbusRTUMessage testRequest;
#endif

    // This function is the first step in the process of parsing a received message. It should make
    // sure the message is destined for this device and will then send it to the proper function.
    virtual void HandleMessage(ModbusMessage* message) override;

    // This function parses the received message and saves the value of the output coils.
    virtual void ParseReadOutputCoils(ModbusMessage* response) override;
    // This function parses the received message and saves the value of the input coils.
    virtual void ParseReadInputCoils(ModbusMessage* response) override;
    // This function parses the received message and saves the value of the output registers.
    virtual void ParseReadOutputRegisters(ModbusMessage* response) override;
    // This function parses the received message and saves the value of the input registers.
    virtual void ParseReadInputRegisters(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This
    // function checks if received matches what was sent.
    virtual void ParseWriteOutputCoil(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This
    // function checks if received matches what was sent.
    virtual void ParseWriteOutputRegister(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This
    // function checks if received matches what was sent.
    virtual void ParseWriteMultipleOutputCoils(ModbusMessage* response) override;
    // If the slave was able to properly handle the request, it echoes back the message. This
    // function checks if received matches what was sent.
    virtual void ParseWriteMultipleOutputRegisters(ModbusMessage* response) override;
};

#endif // MODBUSRTUMASTER_H
