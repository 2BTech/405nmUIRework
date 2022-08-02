#include "modbustcpmessage.h"

ModbusTCPMessage::ModbusTCPMessage() : ModbusMessage()
{

}

ModbusTCPMessage::ModbusTCPMessage(ModbusTCPMessage * other) : ModbusMessage()
{
    this->length = other->length;

    int i;
    for(i = 0; i < other->length; i++)
    {
        buffer[i] = other->buffer[i];
    }
    for(; i < 256; i++)
    {
        buffer[i] = 0;
    }
    ModbusTCPMessage::ParseMessage();
}

ModbusTCPMessage::ModbusTCPMessage(unsigned char * message, int count) : ModbusMessage()
{
    memcpy(buffer, message, count);
    memset(buffer + count, 0, 256 - count);
    length = count;
    ModbusTCPMessage::ParseMessage();
}

ModbusTCPMessage::~ModbusTCPMessage()
{

}

void ModbusTCPMessage::Reset()
{
    memset(buffer, 0, 260);
    length = 0;
}

void ModbusTCPMessage::AddByte(unsigned char byte)
{
    if(length < 260)
    {
        buffer[length++] = byte;

        //ParseMessage();
    }
    else
    {
        qDebug() << ("ModbusRTUMessage overrun");
        //BaseLogger::LogCritical("ModbusRTUMessage overrun");
    }
}

unsigned char ModbusTCPMessage::ParseMessage()
{
    if(length < 7)
    {
        return 0;
    }

    // Package length - 2 bytes
    int packageLength = (buffer[4] << 8) | buffer[5];

    //BaseLogger::Log("Checking if tcp message is complete: Package length: " + QString::number(packageLength + 6) + " actual: " + QString::number(length));
    //BaseLogger::Log("Message bytes: " + BaseLogger::CollectionToString(buffer, length));

    if ((packageLength + 6) == length)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

void ModbusTCPMessage::SetUnitAddress(unsigned char unitAddress)
{
    buffer[6] = unitAddress;

    if(length < 7)
    {
        length = 7;
    }
}

void ModbusTCPMessage::SetFunctionCode(unsigned char functionCode)
{
    buffer[7] = functionCode;

    if(length < 8)
    {
        length = 8;
    }
}

unsigned short ModbusTCPMessage::GetValue(int offset)
{
    if(offset >= 0 && offset + 1 < length)
    {
        return (buffer[offset] << 8) | buffer[offset + 1];
    }
    else
    {
        //throw new std::out_of_range("offset");
        throw new std::exception();
    }
}

void ModbusTCPMessage::SetValue(int offset, unsigned short value)
{
    buffer[offset] = (value & 0xFF00) >> 8;
    buffer[offset + 1] = value & 0x00FF;

    if(length < offset + 2)
    {
        length = offset + 2;
    }
}

unsigned char ModbusTCPMessage::GetByte(int index)
{
    if(index >= 0 && index < length)
    {
        return buffer[index];
    }
    else
    {
        //throw new std::out_of_range("index");
        throw new std::exception();
    }
}

void ModbusTCPMessage::SetByte(int index, unsigned char value)
{
    buffer[index] = value;

    // If appending byte to message, then increase length
    if(length <= index)
    {
        length = index + 1;
    }
}

unsigned char* ModbusTCPMessage::PDU()
{
    return buffer + 7;
}

int ModbusTCPMessage::PDULength()
{
    return length - 7;
}

unsigned short ModbusTCPMessage::Address()
{
    return ((buffer[8]) << 8) | (buffer[9]);
}

unsigned short ModbusTCPMessage::ValueCount()
{
    return ((buffer[10]) << 8) | (buffer[11]);
}

unsigned short ModbusTCPMessage::TransactionID()
{
    return ((buffer[0]) << 8) | (buffer[1]);
}

void ModbusTCPMessage::SetTransactionID(unsigned short id)
{
    buffer[0] = (id & 0xFF00) >> 8;
    buffer[1] = (id & 0x00FF);

    if(length < 2)
    {
        length = 2;
    }
}

unsigned short ModbusTCPMessage::Protocol()
{
    return ((buffer[2] & 0xFF00) >> 8) | (buffer[3] & 0x00FF);
}

void ModbusTCPMessage::SetProtocol(unsigned short protocol)
{
    buffer[2] = (protocol & 0xFF00) >> 8;
    buffer[3] = (protocol & 0x00FF);

    if(length < 4)
    {
        length = 4;
    }
}

unsigned short ModbusTCPMessage::PackageLength()
{
    return ((buffer[4] & 0xFF00) >> 8) | (buffer[5] & 0x00FF);
}

void ModbusTCPMessage::SetPackageLength(unsigned short len)
{
    buffer[4] = (len & 0xFF00) >> 8;
    buffer[5] = (len & 0x00FF);

    if(length < 6)
    {
        length = 6;
    }
}

void ModbusTCPMessage::UpdatePackageLength()
{
    if(length > 7)
    {
        SetPackageLength(length - 6);
    }
    else
    {
        SetPackageLength(0);
    }
}

unsigned char ModbusTCPMessage::UnitAddress()
{
    return buffer[6];
}

unsigned char ModbusTCPMessage::FunctionCode()
{
    return buffer[7];
}

void ModbusTCPMessage::UpdateChecksum(bool)
{
    // Not used in the TCP version
}
