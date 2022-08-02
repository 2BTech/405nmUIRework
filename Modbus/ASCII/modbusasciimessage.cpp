#include "modbusasciimessage.h"

ModbusASCIIMessage::ModbusASCIIMessage() : ModbusMessage()
{

}

ModbusASCIIMessage::ModbusASCIIMessage(ModbusASCIIMessage * other) : ModbusMessage()
{
    this->length = other->length;

    int i;
    for(i = 0; i < other->length; i++)
    {
        buffer[i] = other->buffer[i];
    }
    for(; i < 512; i++)
    {
        buffer[i] = 0;
    }
    ModbusASCIIMessage::ParseMessage();
}

ModbusASCIIMessage::ModbusASCIIMessage(unsigned char * message, int count) : ModbusMessage()
{
    memcpy(buffer, message, count);
    memset(buffer + count, 0, 512 - count);
    length = count;
    ModbusASCIIMessage::ParseMessage();
}

ModbusASCIIMessage::~ModbusASCIIMessage()
{

}

int ModbusASCIIMessage::DeviceID()
{
    return QString().append(buffer[1]).append(buffer[2]).toInt(Q_NULLPTR, 16);
}

void ModbusASCIIMessage::SetStart()
{
    buffer[0] = ':';

    if(length < 1)
    {
        length = 1;
    }
}

void ModbusASCIIMessage::Reset()
{
    memset(buffer, 0, 512);
    length = 0;
    //crc = 0;
}

void ModbusASCIIMessage::AddByte(unsigned char byte)
{
    // Make sure the byte us valid
    if(byte >= 'a' && byte <= 'f')
    {
        byte -= 32;
    }
    else if(!((byte >= '0' && byte <= '9') || (byte >= 'A' && byte <= 'F') || byte == ':' || byte == '\r' || byte == '\n'))
    {
        return;
    }

    if(length <= 512)
    {
        buffer[length++] = byte;

        ParseMessage();
    }
    else
    {
        qDebug() << ("ModbusASCIIMessage overrun");
        //BaseLogger::LogCritical("ModbusASCIIMessage overrun");
    }
}

uchar ModbusASCIIMessage::IsRequestComplete()
{
    // Need at least the function code before it is possible to determine if the message is complete
    if (length < 5)
    {
        return 0;
    }

    // All read requests will be a length of 17, the single write requestes will have a length of 17, the write multiple requests can have
    // more
    uchar functCode = FunctionCode();
    if (functCode == SET_MULTIPLE_COILS || functCode == SET_MULTIPLE_REGISTERS)
    {
        // Get number of bytes field and add the final 4
        int numBytes = (GetByte(13) * 2) + 4;
        // Check if incomplete
        if (numBytes < length)
        {
            return 0;
        }
        if (numBytes == length)
        {
            ushort received = Checksum();
            ushort calced = LRC(buffer + 1, length - 3);

            if (received == calced)
            {
                return 1;
            }
            else
            {
                //BaseLogger::LogCritical("ModbusASCIIMessage received invalid checksum at check 1. Calced: " + QString::number(calced, 16) + " Received: " + QString::number(received, 16));
                //BaseLogger::LogCritical("Received message: " + BaseLogger::CollectionToString(buffer, length, 16));
                qDebug() << ("ModbusASCIIMessage received invalid checksum at check 1. Calced: " + QString::number(calced, 16) + " Received: " + QString::number(received, 16));
                qDebug() << ("Received message: ") << buffer;
                return 2;
            }
        }
        // Too many bytes for the message
        else
        {
            return 2;
        }
    }
    else
    {
        // All requests except for the write collection will have a length of 17
        if (length > 17)
        {
            return 2;
        }
        else if (length == 17)
        {
            ushort received = Checksum();
            ushort calced = LRC(buffer + 1, length - 5);

            if (received == calced)
            {
                return 1;
            }
            else
            {
                //BaseLogger::LogCritical("ModbusASCIIMessage received invalid checksum at check 2. Calced: " + QString::number(calced, 16) + " Received: " + QString::number(received, 16));
                //BaseLogger::LogCritical("Received message: " + BaseLogger::CollectionToString(buffer, length, 16));
                qDebug() << ("Received message: ") << buffer;
                qDebug() << ("ModbusASCIIMessage received invalid checksum at check 2. Calced: " + QString::number(calced, 16) + " Received: " + QString::number(received, 16));
                buffer[length] = 0;
                //BaseLogger::Log("Buffer: " + QString().append(reinterpret_cast<char*>(&buffer[0])));
                qDebug() << ("Buffer: " + QString().append(reinterpret_cast<char*>(&buffer[0])));
                return 2;
            }
        }
        else
        {
            return 0;
        }
    }
}

uchar ModbusASCIIMessage::IsResponseComplete()
{
    // Need at least the function code before it is possible to determine if the message is complete
    if (length < 5)
    {
        return 0;
    }

    int byteCount;
    switch(FunctionCode())
    {
    case READ_COILS:
    case READ_DISCRETE_INPUTS:
    case READ_HOLDING_REGISTERS:
    case READ_INPUT_REGISTERS:
        byteCount = 5 + (GetByte(5) * 2) + 4;
        break;

    case SET_SINGLE_COIL:
    case SET_SINGLE_REGISTER:
    case SET_MULTIPLE_COILS:
    case SET_MULTIPLE_REGISTERS:
        byteCount = 17;
        break;

    default:
        //BaseLogger::LogCritical("ModbusASCIIMessage has bad function code");
        qDebug() << ("ModbusASCIIMessage has bad function code");
        return 2;
    }

    // Incomplete
    if (length < byteCount)
    {
        return 0;
    }
    // Complete
    else if (length == byteCount)
    {
        // The last bytes should be '\r' and '\n'
        if (buffer[length-2] != '\r' && buffer[length - 1] != '\n')
        {
            //BaseLogger::LogCritical("ModbusASCIIMessage has enough bytes but is missing the ending bytes");
            qDebug() << ("ModbusASCIIMessage has enough bytes but is missing the ending bytes");
            return 2;
        }

        // Check checksum
        ushort received = Checksum();
        ushort calced = LRC(buffer + 1, length - 5);

        if (received == calced)
        {
            return 1;
        }
        else
        {
            //BaseLogger::LogCritical("ModbusASCIIMessage received invalid checksum at check 3. Calced: " + QString::number(calced, 16) + " Received: " + QString::number(received, 16));
            //BaseLogger::LogCritical("Received message: " + BaseLogger::CollectionToString(buffer, length, 16));
            qDebug() << ("ModbusASCIIMessage received invalid checksum at check 3. Calced: " + QString::number(calced, 16) + " Received: " + QString::number(received, 16));
            qDebug() << ("Received message: ") << buffer;
            return 2;
        }
    }
    // Overflow
    else
    {
        //BaseLogger::LogCritical("ModbusASCIIMessage has overflowed in IsResponseComplete");
        qDebug() << ("ModbusASCIIMessage has overflowed in IsResponseComplete");
        return 2;
    }
}

uchar ModbusASCIIMessage::ParseMessage()
{
    return IsRequestComplete();
}

void ModbusASCIIMessage::SetUnitAddress(unsigned char unitAddress)
{
    sprintf(reinterpret_cast<char*>(buffer + 1), "%02X", unitAddress);
    if(length < 3)
    {
        length = 3;
    }
}

void ModbusASCIIMessage::SetFunctionCode(unsigned char functionCode)
{
    sprintf(reinterpret_cast<char*>(buffer + 3), "%02X", functionCode);
    if(length < 5)
    {
        length = 5;
    }
}

unsigned short ModbusASCIIMessage::GetValue(int offset)
{
    if(offset >= 0 && offset + 3 < length)
    {
        QString temp = QString();
        temp.append(buffer[offset]);
        temp.append(buffer[offset + 1]);
        temp.append(buffer[offset + 2]);
        temp.append(buffer[offset + 3]);
        return temp.toUShort(nullptr, 16);
    }
    else
    {
        //BaseLogger::Log(QString().append(reinterpret_cast<char*>(buffer)));
        //BaseLogger::LogCritical("ModbusASCIIMessage::GetValue out of range");
        //BaseLogger::Log("Offset: " + QString::number(offset) + " Length: " + QString::number(length));
        qDebug() << (QString().append(reinterpret_cast<char*>(buffer)));
        qDebug() << "ModbusASCIIMessage::GetValue out of range";
        qDebug() << ("Offset: " + QString::number(offset) + " Length: " + QString::number(length));
        //throw new std::out_of_range("offset");
        throw new std::exception();
    }
}

void ModbusASCIIMessage::SetValue(int offset, unsigned short value)
{
    sprintf(reinterpret_cast<char*>(buffer+offset), "%04X", value);
    if(length < offset + 4)
    {
        length = offset + 4;
    }
}

unsigned char ModbusASCIIMessage::GetByte(int index)
{
    if(index >= 0 && index < length)
    {
        char temp[3] = {0, 0, 0};
        memcpy(temp, reinterpret_cast<char*>(buffer + index), 2);
        return static_cast<unsigned char>(atoi(temp));
    }
    else
    {
        //BaseLogger::LogCritical("ModbusASCIIMessage::GetByte index out of bounds. Length: " + QString::number(length) + " Index: " + QString::number(index));
        qDebug() << ("ModbusASCIIMessage::GetByte index out of bounds. Length: " + QString::number(length) + " Index: " + QString::number(index));
        //throw new std::out_of_range("index");
        throw new std::exception();
    }
}

void ModbusASCIIMessage::SetByte(int index, unsigned char value)
{
    //BaseLogger::Log("Setting byte at " + QString::number(index).toLatin1());
    //BaseLogger::Log(QString::number(*(buffer+index)));
    sprintf(reinterpret_cast<char*>(buffer+index), "%02X", value);

    if(length < index + 2)
    {
        length = index + 2;
    }
}

unsigned char * ModbusASCIIMessage::PDU()
{
    return buffer + 1;
}

int ModbusASCIIMessage::PDULength()
{
    return length - 3;
}

unsigned short ModbusASCIIMessage::Address()
{
    return QString().append(buffer[5]).append(buffer[6]).append(buffer[7]).append(buffer[8]).toInt(Q_NULLPTR, 16);
}

unsigned short ModbusASCIIMessage::ValueCount()
{
    return QString().append(buffer[9]).append(buffer[10]).append(buffer[11]).append(buffer[12]).toInt(Q_NULLPTR, 16);
}

unsigned char ModbusASCIIMessage::UnitAddress()
{
    return  QString().append(buffer[1]).append(buffer[2]).toInt(Q_NULLPTR, 16);
    //return buffer[0];
}

unsigned char ModbusASCIIMessage::FunctionCode()
{
    return  QString().append(buffer[3]).append(buffer[4]).toInt(Q_NULLPTR, 16);
    //return buffer[1];
}

void ModbusASCIIMessage::UpdateChecksum(bool hasLRC)
{
    // If the buffer already has a lrc, then replace the last 2 bytes
    if(hasLRC)
    {
        //BaseLogger::Log("Updating checksum");
        ushort lrc = LRC(buffer + 3, this->length - 5);
        sprintf(reinterpret_cast<char*>(buffer + length - 2), "%02X", lrc);
    }
    else
    {
        // Address and message fields
        ushort lrc = LRC(buffer + 1, this->length - 1);
        sprintf(reinterpret_cast<char*>(buffer + length), "%02X", lrc);
        length += 2;
    }
}

unsigned char ModbusASCIIMessage::LRC(unsigned char *message, int length)
{
    uint8_t lrc = 0;
    char buffer[3];
    for (int i = 0; i < length; i += 2)
    {
        buffer[0] = message[i];
        buffer[1] = message[i + 1];
        lrc += strtol(buffer, 0, 16) & 0xFF;
    }
//    for (int i = 0; i < length; i++)
//    {
//        lrc += message[i] & 0xFF;
//    }
    lrc &= 0xFF;

    return ((lrc ^ 0xFF) + 1) & 0xFF;
}

unsigned int ModbusASCIIMessage::Checksum()
{
//    if(length > 4)
//    {
//        if(buffer[length - 2] == '\r' && buffer[length - 1] == '\n')
//        {
//            return QString().append(buffer[length - 4]).append(buffer[length - 3]).toInt(Q_NULLPTR, 16);
//        }
//        else
//        {
//            return QString().append(buffer[length-2]).append(buffer[length-1]).toInt(Q_NULLPTR, 16);
//        }
//    }
    //BaseLogger::LogCritical("In ModbusASCIIMessage::Checksum");
    //return lrc;
    char temp[3] = { 0, 0, 0 };
    temp[0] = buffer[length - 4];
    temp[1] = buffer[length - 3];
    return QString(temp).toInt(Q_NULLPTR, 16);
}

void ModbusASCIIMessage::AddEnd()
{
    buffer[length++] = 0x0D;
    buffer[length++] = 0x0A;
}
