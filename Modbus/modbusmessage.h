#ifndef MODBUSMESSAGE_H
#define MODBUSMESSAGE_H

#include <string.h>

#include "modbusdefs.h"

// Abstract class for the ModbusMessage classes
class ModbusMessage
{
public:
    ModbusMessage();
    virtual ~ModbusMessage();

    // Clears all values stored in the ModbusMessage
    virtual void Reset() = 0;

    // Adds a byte to the internal message buffer
    virtual void AddByte(unsigned char byte);

    // Adds a collection of bytes in the message buffer.
    virtual void AddBytes(unsigned char* bytes, int count);

    // Parses the internal buffer and populates values.
    virtual unsigned char ParseMessage() = 0;

    // Getter function for the unit address value
    virtual unsigned char UnitAddress() = 0;
    // Sets the unit address value in the message.
    virtual void SetUnitAddress(unsigned char unitAddress) = 0;

    // Getter for the function code
    virtual unsigned char FunctionCode() = 0;
    // Sets the function code in the message.
    virtual void SetFunctionCode(unsigned char functionCode) = 0;

    // Gets a value from the interal buffer by combining 2 bytes
    // at the offset into an unsigned short value
    virtual unsigned short GetValue(int offset) = 0;
    // Inserts an unsigned short into the internal message buffer
    virtual void SetValue(int offset, unsigned short value) = 0;

    // Gets the byte value at index
    virtual unsigned char GetByte(int index) = 0;
    // Sets the byte value at index with byte
    virtual void SetByte(int index, unsigned char byte) = 0;

    // Returns a reference to the internal message buffer
    unsigned char* Buffer();
    // Returns the length of the internal message buffer
    int Length();

    // Returns the PDU portion of the message buffer
    virtual unsigned char * PDU() = 0;
    // Length of the PDU section
    virtual int PDULength() = 0;

    // Updates the checksum value in the buffer
    virtual void UpdateChecksum(bool hasChecksum) = 0;

    // Gets the address value from the buffer
    virtual unsigned short Address() = 0;
    // Gets the quantity value from the buffer
    virtual unsigned short ValueCount() = 0;

    // Updates the message length value.
    virtual void UpdateLength(int nCount);

protected:
    // Internal message buffer
    unsigned char buffer[512];
    // Length of buffer
    int length = 0;
};

#endif // MODBUSMESSAGE_H
