#include "modbusmessage.h"

ModbusMessage::ModbusMessage()
{
    memset(buffer, 0, 512);
}

ModbusMessage::~ModbusMessage()
{

}

void ModbusMessage::AddByte(unsigned char byte)
{
    buffer[length++] = byte;

    ParseMessage();
}

void ModbusMessage::AddBytes(unsigned char *bytes, int count)
{
    memcpy(buffer + length, bytes, count);
    length += count;

    ParseMessage();
}

unsigned char * ModbusMessage::Buffer()
{
    return buffer;
}

int ModbusMessage::Length()
{
    return length;
}

void ModbusMessage::UpdateLength(int length)
{
    this->length = length;
}
