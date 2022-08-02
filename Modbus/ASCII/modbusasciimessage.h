#ifndef MODBUSASCIIMESSAGE_H
#define MODBUSASCIIMESSAGE_H

#include <QString>
#include <QDebug>

#include "Modbus/modbusmessage.h"

// This class extends ModbusMessage by implementing all abstract functions and adding ascii protocol
// specific values and functions. The main addition is the LRC checksum function.
class ModbusASCIIMessage  : public ModbusMessage {
public:
    ModbusASCIIMessage();
    ModbusASCIIMessage(ModbusASCIIMessage * other);
    ModbusASCIIMessage(unsigned char * message, int count);

    ~ModbusASCIIMessage();

    // This function resets the values in the class. It clears the buffer, clears lrc, and re-adds the message start byte (':').
    void Reset() override;

    // Sets the first character in the message to ':'. This character signals the start of a message.
    void SetStart();

    // Adds a byte at the end of the message. It converts the byte into a 2 character hexadecimal string.
    void AddByte(unsigned char byte) override;

    // Gets the device ID value from the message.
    int DeviceID();

    // Parses the internal buffer and populates the unit address and lrc.
    unsigned char ParseMessage() override;

    unsigned char IsRequestComplete();
    unsigned char IsResponseComplete();

    // Adds the unitAddress to the proper location in the message. Will increase the length value
    // for the message if it does not inlcude the added bytes.
    void SetUnitAddress(unsigned char unitAddress) override;

    // Sets the bytes in the message that represent the function code with functionCode.
    void SetFunctionCode(unsigned char functionCode) override;

    // This function returns the 16 bit value at offset. Converts the hex strings back into the 16 bit string.
    unsigned short GetValue(int offset) override;
    // This function sets the bytes in the message starting at offset with value. Value is converted into a 4 byte hex
    // string and is then copied into the message buffer. Increases the length value of the message if it does not include the added values.
    void SetValue(int offset, unsigned short value) override;

    // Gets the value of the byte in the message starting at index. Converts the 2 byte hex string into the 8 bit value it represents.
    unsigned char GetByte(int index) override;
    // This function sets the bytes in the message starting at the offset with value. Value is converted into a 2 byte hex
    // string that is copied into the message buffer. The length value of the message is increased to include the added bytes if necessary.
    void SetByte(int index, unsigned char value) override;

    // Gets a reference to the modbus message buffer starting at the beginning of the PDU portion of the message.
    unsigned char* PDU() override;
    // Gets the length of the PDU data section.
    int PDULength() override;

    // This function calculates a checksum for the message using the LRC algorithm. If the message already has a checksum, the checksum
    // is calculated up to but not including the current checksum bytes and replaces the current checksum. If there is no checksum, the
    // checksum is made up of all bytes and appeneded to the end of the message..
    void UpdateChecksum(bool hasChecksum) override;

    // Gets the address value from the message buffer.
    unsigned short Address() override;
    // Gets the quantity value from the message buffer.
    unsigned short ValueCount() override;

    // Gets the unit address value from the message buffer.
    unsigned char UnitAddress() override;
    // Gets the function code value from the message buffer.
    unsigned char FunctionCode() override;

    // Gets the checksum value from the message buffer.
    unsigned int Checksum();

    // Add the end of line characters to the message buffer.
    void AddEnd();

private:
    // The checksum for the message
    //unsigned short lrc = 0;

    // Calculates the checksum using the LRC algorithm based off of message.
    unsigned char LRC(unsigned char* message, int length);
};

#endif // MODBUSASCIIMESSAGE_H
