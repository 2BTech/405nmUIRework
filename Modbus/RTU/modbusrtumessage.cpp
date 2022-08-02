#include "modbusrtumessage.h"

ModbusRTUMessage::ModbusRTUMessage() : ModbusMessage()
{
    GenerateCRCTable();
}

ModbusRTUMessage::ModbusRTUMessage(ModbusRTUMessage * other) : ModbusMessage()
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
    ModbusRTUMessage::ParseMessage();
}

ModbusRTUMessage::ModbusRTUMessage(unsigned char * message, int count) : ModbusMessage()
{
    memcpy(buffer, message, count);
    memset(buffer + count, 0, 256 - count);
    length = count;
    ModbusRTUMessage::ParseMessage();
}

ModbusRTUMessage::~ModbusRTUMessage()
{

}

void ModbusRTUMessage::Reset()
{
    memset(buffer, 0, 256);
    length = 0;
    crcCehecksum = 0;
}

void ModbusRTUMessage::AddByte(unsigned char byte)
{
    if(length <= 256)
    {
        buffer[length++] = byte;

        ParseMessage();
    }
    else
    {
        //BaseLogger::LogCritical("ModbusRTUMessage overrun");
        qDebug() << ("ModbusRTUMessage overrun");
    }
}

unsigned char ModbusRTUMessage::IsRequestComplete()
{
    // Need atleast 3 bytes before the message can be checked
    if(length < 3)
    {
        return 0;
    }

    if (FunctionCode() == SET_MULTIPLE_COILS || FunctionCode() == SET_MULTIPLE_REGISTERS)
    {
        // Check if the entire message has been received
        if(length < buffer[2] + 3)
        {
            return 0;
        }

        crcCehecksum = ((buffer[length - 2] << 8) | buffer[length-1]) & 0xFFFF;

        short ncrc = CRC(buffer, length - 2);

        if (ncrc == crcCehecksum)
        {
            return 1;
        }
        else
        {
            qDebug() << ("ModbusRTUMessage received invalid checksum(1). Received: " + QString::number(crcCehecksum & 0xFFFF, 16) + " Expected: " + QString::number(ncrc & 0xFFFF, 16));
            qDebug() << ("Received rtu message: ") << buffer;

//            BaseLogger::LogCritical("ModbusRTUMessage received invalid checksum(1). Received: " + QString::number(crcCehecksum & 0xFFFF, 16) + " Expected: " + QString::number(ncrc & 0xFFFF, 16));
//            BaseLogger::LogCritical("Received rtu message: " + BaseLogger::CollectionToString(buffer, length, 16));
            return 2;
        }
    }
    else
    {
        // All requests except for the write collection will have a length of 8
        if (length > 8)
        {
            return 2;
        }
        else if (length == 8)
        {
            crcCehecksum = ((buffer[length - 2] << 8) | buffer[length-1]) & 0xFFFF;

            short ncrc = CRC(buffer, length - 2);

            if (ncrc == crcCehecksum)
            {
                return 1;
            }
            else
            {
                //BaseLogger::LogCritical("ModbusRTUMessage received invalid checksum(1). Received: " + QString::number(crcCehecksum, 16) + " Expected: " + QString::number(ncrc, 16));
                //BaseLogger::LogCritical("Received message: " + BaseLogger::CollectionToString(buffer, length, 16));

                qDebug() << ("ModbusRTUMessage received invalid checksum(1). Received: " + QString::number(crcCehecksum, 16) + " Expected: " + QString::number(ncrc, 16));
                qDebug() << ("Received message: ") << buffer;
                return 2;
            }
        }
        else
        {
            return 0;
        }
    }
}

uchar ModbusRTUMessage::IsResponseComplete()
{
    // Need atleast 3 bytes before the message can be checked
    if(length < 3)
    {
        return 0;
    }

    // Check if the entire message has been received
    if(length < buffer[2] + 3)
    {
        return 0;
    }

    crcCehecksum = ((buffer[length - 2] << 8) | buffer[length-1]) & 0xFFFF;

    short ncrc = CRC(buffer, length - 2);

    if (ncrc == crcCehecksum)
    {
        return 1;
    }
    else
    {
        //BaseLogger::LogCritical("ModbusRTUMessage received invalid checksum(1). Received: " + QString::number(crcCehecksum, 16) + " Expected: " + QString::number(ncrc, 16));
        //BaseLogger::LogCritical("Received message: " + BaseLogger::CollectionToString(buffer, length, 16));
        qDebug() << ("ModbusRTUMessage received invalid checksum(1). Received: " + QString::number(crcCehecksum, 16) + " Expected: " + QString::number(ncrc, 16));
        qDebug() << ("Received message: ") << buffer;
        return 2;
    }
}

// This doesn't work, the differences between the requests and responses is just too different, shoul
uchar ModbusRTUMessage::ParseMessage()
{
    return IsRequestComplete();
}

void ModbusRTUMessage::SetUnitAddress(unsigned char unitAddress)
{
    buffer[0] = unitAddress;

    if(length < 1)
    {
        length = 1;
    }
}

void ModbusRTUMessage::SetFunctionCode(unsigned char functionCode)
{
    buffer[1] = functionCode;

    if(length < 2)
    {
        length = 2;
    }
}

unsigned short ModbusRTUMessage::GetValue(int offset)
{
    if(offset >= 0 && offset + 1 < length)
    {
        return (buffer[offset] << 8) | buffer[offset + 1];
    }
    else
    {
        //BaseLogger::LogCritical("ModbusRTUMessage::GetValue offset out of bounds");
        qDebug() << ("ModbusRTUMessage::GetValue offset out of bounds");
        //throw new std::out_of_range("offset");
        throw new std::exception();
    }
}

void ModbusRTUMessage::SetValue(int offset, unsigned short value)
{
    buffer[offset] = (value & 0xFF00) >> 8;
    buffer[offset + 1] = value & 0x00FF;

    if(length < offset + 2)
    {
        length = offset + 2;
    }
}

unsigned char ModbusRTUMessage::GetByte(int index)
{
    if(index >= 0 && index < length)
    {
        return buffer[index];
    }
    else
    {
        //BaseLogger::LogCritical("ModbusRTUMessage::GetByte index out of bounds");
        qDebug() << ("ModbusRTUMessage::GetByte index out of bounds");
        //throw new std::out_of_range("index");
        throw new std::exception();
    }
}

void ModbusRTUMessage::SetByte(int index, unsigned char value)
{
    buffer[index] = value;

    // If appending byte to message, then increase length
    if(length <= index)
    {
        length = index + 1;
    }
}

unsigned char * ModbusRTUMessage::PDU()
{
    return buffer + 1;
}

int ModbusRTUMessage::PDULength()
{
    return length - 3;
}

unsigned short ModbusRTUMessage::Address()
{
    return ((buffer[2] & 0xFF00) >> 8) | (buffer[3] & 0x00FF);
}

unsigned short ModbusRTUMessage::ValueCount()
{
    return ((buffer[4] & 0xFF00) >> 8) | (buffer[5] & 0x00FF);
}

unsigned char ModbusRTUMessage::UnitAddress()
{
    return buffer[0];
}

unsigned char ModbusRTUMessage::FunctionCode()
{
    return buffer[1];
}

void ModbusRTUMessage::UpdateChecksum(bool hasCRC)
{
    // If the buffer already has a crc, then skip the last 2 bytes
    if(hasCRC)
    {
        crcCehecksum = CRC(buffer, this->length - 2);
        buffer[length - 1] = crcCehecksum & 0x00FF;
        buffer[length - 2] = (crcCehecksum & 0xFF00) >> 8;
    }
    else
    {
        crcCehecksum = CRC(buffer, this->length);
        buffer[length++] = (crcCehecksum & 0xFF00) >> 8;
        buffer[length++] = crcCehecksum & 0x00FF;
    }
}

unsigned short ModbusRTUMessage::CRC(unsigned char *message, int length)
{
//    static const unsigned short table[2] = { 0x0000, 0xA001 };
//    unsigned short crc = 0xFFFF;
//    int i = 0;
//    char b = 0;
//    unsigned int x = 0;

//    for(i = 0; i < length; i++)
//    {
//        crc ^= message[i];

//        for( b = 0; b < 8; b++ )
//        {
//            x = crc & 0x01;
//            crc >>= 1;
//            crc ^= table[x];
//        }
//    }

//    return crc;

    ushort crc = 0xFFFF;
    for (int i = 0; i < length; i++)
    {
        ushort idx = crcTable[(crc ^ message[i]) & 0xFF];
        crc = ((crc >> 8) & 0xFF) ^ idx;
    }
    return ((crc << 8) & 0xFF00) | ((crc >> 8) & 0x00FF);
}

ushort* ModbusRTUMessage::crcTable = Q_NULLPTR;
void ModbusRTUMessage::GenerateCRCTable()
{
    if(crcTable != Q_NULLPTR)
    {
        return;
    }

    //BaseLogger::Log("Creating CRC table");

    crcTable = new ushort[256];
    for (int byte = 0, i = 0; i < 256; i++)
    {
        byte = i;
        ushort crc = 0x0000;
        for (int j = 0; j < 8; j++)
        {
            if ((byte ^ crc) & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc >>= 1;
            }
            byte >>= 1;
        }
        crcTable[i] = crc;
    }
    //BaseLogger::Log("CRC Table:\n" + BaseLogger::CollectionToString(crcTable, 256, 16));
}
