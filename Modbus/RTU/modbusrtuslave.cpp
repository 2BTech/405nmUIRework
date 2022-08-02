#include "modbusrtuslave.h"

ModbusRTUSlave::ModbusRTUSlave() : ModbusRTUBase(), ModbusSlave(), response()
{

}

ModbusRTUSlave::ModbusRTUSlave(ModbusRTUSlave * other) : ModbusRTUBase(), ModbusSlave(), response()
{
    serialPort.setPortName(other->serialPort.portName());
    serialPort.setBaudRate(other->serialPort.baudRate());
}

void ModbusRTUSlave::HandleMessage(ModbusMessage * message)
{
    memcpy(currentMessage.Buffer(), message->Buffer(), message->Length());
    currentMessage.UpdateLength(message->Length());

    //BaseLogger::Log("Message address: " + QString::number(message->UnitAddress()) + " Device address: " + QString::number(unitID));
    //BaseLogger::Log("Is message for this: " + BaseLogger::BoolToString((message->UnitAddress() == 0 || message->UnitAddress() == unitID)));

    qDebug() << ("Message address: " + QString::number(message->UnitAddress()) + " Device address: " + QString::number(unitID));
    qDebug() << ("Is message for this: ") << (message->UnitAddress() == 0 || message->UnitAddress() == unitID);

    // Check if the message is a broadcast or is for this device
    if (message->UnitAddress() == 0 || message->UnitAddress() == unitID)
    {
        switch (message->FunctionCode())
        {
        case READ_COILS:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: READ_COILS");
            qDebug() << ("ModbusRTUSlave::HandleMessage: READ_COILS");
#endif // DEBUG_MODBUS_RTU_SLAVE
            ReadOutputCoils(message->Address(), message->ValueCount());
            break;

        case READ_DISCRETE_INPUTS:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: READ_DISCRETE_INPUTS");
            qDebug() << ("ModbusRTUSlave::HandleMessage: READ_DISCRETE_INPUTS");
#endif // DEBUG_MODBUS_RTU_SLAVE
            ReadInputCoils(message->Address(), message->ValueCount());
            break;

        case READ_HOLDING_REGISTERS:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: READ_HOLDING_REGISTERS");
            qDebug() << ("ModbusRTUSlave::HandleMessage: READ_HOLDING_REGISTERS");
#endif // DEBUG_MODBUS_RTU_SLAVE
            ReadOutputRegisters(message->Address(), message->ValueCount());
            break;

        case READ_INPUT_REGISTERS:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: READ_INPUT_REGISTERS");
            qDebug() << ("ModbusRTUSlave::HandleMessage: READ_INPUT_REGISTERS");
#endif // DEBUG_MODBUS_RTU_SLAVE
            ReadInputRegisters(message->Address(), message->ValueCount());
            break;

        case SET_SINGLE_COIL:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: SET_SINGLE_COIL");
            qDebug() << ("ModbusRTUSlave::HandleMessage: SET_SINGLE_COIL");
#endif // DEBUG_MODBUS_RTU_SLAVE
            WriteOutputCoil(message->Address());
            break;

        case SET_SINGLE_REGISTER:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: SET_SINGLE_REGISTER");
            qDebug() << ("ModbusRTUSlave::HandleMessage: SET_SINGLE_REGISTER");
#endif // DEBUG_MODBUS_RTU_SLAVE
            WriteOutputRegister(message->Address());
            break;

        case SET_MULTIPLE_COILS:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: SET_MULTIPLE_COILS");
            qDebug() << ("ModbusRTUSlave::HandleMessage: SET_MULTIPLE_COILS");
#endif // DEBUG_MODBUS_RTU_SLAVE
            WriteMultipleOutputCoils(message->Address(), message->ValueCount());
            break;

        case SET_MULTIPLE_REGISTERS:
#ifdef DEBUG_MODBUS_RTU_SLAVE
            //BaseLogger::Log("ModbusRTUSlave::HandleMessage: SET_MULTIPLE_REGISTERS");
            qDebug() << ("ModbusRTUSlave::HandleMessage: SET_MULTIPLE_REGISTERS");
#endif // DEBUG_MODBUS_RTU_SLAVE
            WriteMultipleOutputRegisters(message->Address(), message->ValueCount());
            break;

        default:
            //BaseLogger::LogCritical("Modbus TCP slave received bad function code: " + QString::number(message->FunctionCode()));
            qDebug() << ("Modbus TCP slave received bad function code: " + QString::number(message->FunctionCode()));
            break;
        }
    }
}

void ModbusRTUSlave::ReadOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_COILS);

    response.SetByte(2, (length + 7) / 8);

    dataTable->GetOutputCoils_charArr(address, length, response.PDU() + 2, (length + 7) / 8);

    response.UpdateLength(3 + response.GetByte(2));

    response.UpdateChecksum(false);

//    uchar testResponse[] = { 0x02, 0x01, 0x02, 0x80, 0x02, 0x1D, 0xFD, 0x00 };
//    serialPort.write(reinterpret_cast<char*>(testResponse), 7);

#if !TEST_BUILD
#ifdef DEBUG_MODBUS_RTU_SLAVE
    //BaseLogger::Log("ModbusRTUSlave response to READ_COILS: " + BaseLogger::CollectionToString(response.Buffer(), response.Length(), 16));
    qDebug() << "ModbusRTUSlave response to READ_COILS: " << response.Buffer();
#endif // DEBUG_MODBUS_RTU_SLAVE
    /*int wrote = */serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
    //BaseLogger::Log("Wrote a response of length: " + QString::number(wrote) + " : " + BaseLogger::CollectionToString(reinterpret_cast<char*>(response.Buffer()), response.Length(), 16));
    //char test[] = { 0x05, 0x01, 0x01, 0x01, 0x78, -0x6F, 0x00 };
    //BaseLogger::Log("Wrote a response of length: " + QString::number(wrote) + " : " + BaseLogger::CollectionToString(test, response.Length(), 16));
#endif
}

void ModbusRTUSlave::ReadInputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfInputCoils())
    {
        throw new std::out_of_range("Address + length >= dataTable->NumberOfInputCoils()");
    }

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_DISCRETE_INPUTS);

    response.SetByte(2, (length + 7) / 8);

    dataTable->GetInputCoils_charArr(address, length, response.PDU() + 2, (length + 7) / 8);

    response.UpdateLength(3 + response.GetByte(2));

    response.UpdateChecksum(false);

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusRTUSlave::ReadOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length > dataTable->NumberOfOutputRegisters())
    {
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
    }

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_HOLDING_REGISTERS);

    response.SetByte(2, length * 2);

    dataTable->GetOutputRegisters_charArr(address, length, response.PDU() + 2);

    response.UpdateLength(3 + response.GetByte(2));

    response.UpdateChecksum(false);

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusRTUSlave::ReadInputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfInputRegisters())
    {
        throw new std::out_of_range("Address + length >= dataTable->NumberOfInputRegisters()");
    }

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_INPUT_REGISTERS);

    response.SetByte(2, length * 2);

    dataTable->GetInputRegisters_charArr(address, length, response.PDU() + 2);

    response.UpdateLength(3 + response.GetByte(2));

    response.UpdateChecksum(false);

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusRTUSlave::WriteOutputCoil(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputCoils())
    {
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    dataTable->SetOutputCoil(address, currentMessage.GetValue(4) == 0xFF00);

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_SINGLE_COIL);

    response.SetValue(2, address);

    response.SetValue(4, currentMessage.GetValue(4));

    response.UpdateChecksum(false);

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusRTUSlave::WriteOutputRegister(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputRegisters())
    {
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
    }

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_SINGLE_REGISTER);

    response.SetValue(2, address);

    response.SetValue(4, currentMessage.GetValue(4));

    dataTable->SetOutputRegister(address, currentMessage.GetValue(4));

    response.UpdateChecksum(false);

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusRTUSlave::WriteMultipleOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_MULTIPLE_COILS);

    response.SetValue(2, address);

    response.SetValue(4, length);

    dataTable->SetOutputCoils(address, length, currentMessage.Buffer() + 7);

    response.UpdateChecksum(false);

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusRTUSlave::WriteMultipleOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        throw new std::out_of_range(__PRETTY_FUNCTION__);
    }

    response.Reset();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_MULTIPLE_REGISTERS);

    response.SetValue(2, address);

    response.SetValue(4, length);

    unsigned char * bytes = currentMessage.Buffer() + 7;
    for(int i = 0; i < length; i++)
    {
        dataTable->SetOutputRegister(address + i, (bytes[0] << 8) | bytes[1]);
        bytes += 2;
    }

    dataTable->GetOutputRegisters_charArr(address, length, currentMessage.PDU() + 6);

    response.UpdateChecksum(false);

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusRTUSlave::MoveToThread(QThread * thread)
{
    ModbusRTUBase::MoveToThread(thread);
}
