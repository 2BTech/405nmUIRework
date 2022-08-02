#ifndef MODBUSRTUMESSAGE_H
#define MODBUSRTUMESSAGE_H

#include <QDebug>

// Inheritance, defines functions and variables that all modbus messages should have
#include "Modbus/modbusmessage.h"

// This class is in charge of parsing through a
// RTU message
// |Unit Address(1 byte)|Message(N bytes)|CRC(2 bytes)|

// This class extends ModbusMessage by implementing all abstract functions and adding rtu protocol
// specific values and functions. The main addition is the LRC checksum function.
class ModbusRTUMessage : public ModbusMessage
{
public:
    ModbusRTUMessage();
    ModbusRTUMessage(ModbusRTUMessage * other);
    ModbusRTUMessage(unsigned char * message, int count);

    ~ModbusRTUMessage();

    // This function resets the values in the class. It clears the buffer and clears the checksum.
    void Reset() override;

    // Adds a byte at the end of the message.
    void AddByte(unsigned char byte) override;

    // Parses the internal buffer and returns true if the message is complete and valid.
    unsigned char ParseMessage() override;
    // Since the differences between the requests and responses is too different, have to make a check function for both
    // Parses the intrenal buffer as a request and returns a byte describing the validity of the message
    unsigned char IsRequestComplete();
    // Parses the intrenal buffer as a request and returns a byte describing the validity of the message
    unsigned char IsResponseComplete();

    // Adds the unitAddress to the proper location in the message. Will increase the length value for
    // the message if it does not inlcude the added byte.
    void SetUnitAddress(unsigned char unitAddress) override;

    // Sets the bytes in the message that represent the function code with functionCode.
    void SetFunctionCode(unsigned char functionCode) override;

    // This function returns the 16 bit value starting at offset.
    unsigned short GetValue(int offset) override;
    // This function sets the bytes in the message starting at offset with value. Value is
    // converted into its two bytes and is then copied into the message buffer. Increases
    // the length value of the message if it does not include the added values.
    void SetValue(int offset, unsigned short value) override;

    // Gets the value of the byte in the message starting at index.
    unsigned char GetByte(int index) override;
    // This function ets the bytes in the message starting at offset with value.
    // The length value of the message is increased to include the added bytes if necessary.
    void SetByte(int index, unsigned char value) override;

    // Gets a reference to the modbus message buffer starting at the beginning of the PDU portion of the message.
    unsigned char* PDU() override;
    // Gets the length of the PDU data section.
    int PDULength() override;

    // This function calculates a checksum for the message using the CRC algorithm. If the message already
    // has a checksum, the checksum is calculated up to but not including the current checksum bytes and
    // replaces the current checksum. If there is no checksum, the checksum is made up of all bytes and
    // appeneded to the end of the message.
    void UpdateChecksum(bool hasCRC) override;

    // Gets the address value from the message buffer.
    unsigned short Address() override;
    // Gets the quantity value from the message buffer.
    unsigned short ValueCount() override;
    // Gets the unit address value from the message buffer.
    unsigned char UnitAddress() override;
    // Gets the function code value from the message buffer.
    unsigned char FunctionCode() override;

private:
    // The checksum for the message
    short crcCehecksum = 0;

    // Calculates the checksum using the CRC algorithm based off of message.
    unsigned short CRC(unsigned char* message, int length);

    static unsigned short* crcTable;
    void GenerateCRCTable();
};

#endif // MODBUSRTUMESSAGE_H
