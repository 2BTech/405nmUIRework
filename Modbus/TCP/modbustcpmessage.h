#ifndef MODBUSTCPMESSAGE_H
#define MODBUSTCPMESSAGE_H

#include <string.h>
#include <QDebug>

// Inheritance, defines functions and variables that all modbus messages should have
#include "Modbus/modbusmessage.h"

// This class extends ModbusMessage by implementing all abstract functions and adding tcp protocol specific values and functions.
class ModbusTCPMessage : public ModbusMessage
{
public:
    ModbusTCPMessage();
    ModbusTCPMessage(ModbusTCPMessage* other);
    ModbusTCPMessage(unsigned char* message, int count);

    ~ModbusTCPMessage();

    // This function resets all class values.
    void Reset() override;

    // Adds bytes to the end of the message.
    void AddByte(unsigned char byte) override;

    // Parses the internal buffer. Returns true if the length
    // of the message matches the expected length.
    unsigned char ParseMessage() override;

    // Adds the unitAddress to the proper location in the message. Will increase
    // the length value for the message if it does not inlcude the added bytes.
    void SetUnitAddress(unsigned char unitAddress) override;
   // Sets the bytes in the message that represent the function code with functionCode.
    void SetFunctionCode(unsigned char functionCode) override;

    // This function returns the 16 bit value at offset.
    unsigned short GetValue(int offset) override;
    // This function sets the bytes in the message starting at offset with value. Increases
    // the length value of the message if it does not include the added values.
    void SetValue(int offset, unsigned short value) override;

    // Gets the value of the byte in the message starting at index.
    unsigned char GetByte(int index) override;
    // This function sets the bytes in the message starting at the offset with value. The
    // length value of the message is increased to include the added bytes if necessary.
    void SetByte(int index, unsigned char value) override;

    // Gets a reference to the modbus message buffer starting at the beginning of the PDU portion of the message.
    unsigned char* PDU() override;
    // Gets the length of the PDU data section.
    int PDULength() override;

    // Gets the address value from the message buffer.
    unsigned short Address() override;
    // Gets the quantity value from the message buffer.
    unsigned short ValueCount() override;

    // Header values
    // Sets the transaction id bytes on the message. Will increase the length value if it does not include the added bytes.
    unsigned short TransactionID();
    // Sets the transaction id bytes on the message. Will increase the length value if it does not include the added bytes.
    void SetTransactionID(unsigned short id);

    // Gets the protocol value from the message buffer.
    unsigned short Protocol();
    // Sets the protocol value bytes in the message. Will incrase the length value if it does not include the added value.
    void SetProtocol(unsigned short protocol);

    // Gets the package length value from the message buffer.
    unsigned short PackageLength();
    // Sets the package length value in the message buffer. Will increase the length value if it does not include the added value.
    void SetPackageLength(unsigned short len);
    // Udpates the package length value to match the size of the message.
    void UpdatePackageLength();

    // The TCP protocol does not use a checksum
    virtual void UpdateChecksum(bool) override;

    // Gets the unit address value from the message buffer
    unsigned char UnitAddress() override;
    // Gets the function code value from the buffer
    unsigned char FunctionCode() override;
};

#endif // MODBUSTCPMESSAGE_H
