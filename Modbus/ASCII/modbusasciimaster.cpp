#include "modbusasciimaster.h"

ModbusASCIIMaster::ModbusASCIIMaster() : ModbusASCIIBase() , ModbusMaster()
{

}

void ModbusASCIIMaster::HandleMessage(ModbusMessage * message)
{
    if(message->UnitAddress() == 0 || message->UnitAddress() == unitID)
    {
        switch (message->FunctionCode())
        {
        case READ_COILS:
            ParseReadOutputCoils(message);
            break;

        case READ_DISCRETE_INPUTS:
            ParseReadInputCoils(message);
            break;

        case READ_HOLDING_REGISTERS:
            ParseReadOutputRegisters(message);
            break;

        case READ_INPUT_REGISTERS:
            ParseReadInputRegisters(message);
            break;

        case SET_SINGLE_COIL:
            ParseWriteOutputCoil(message);
            break;

        case SET_SINGLE_REGISTER:
            ParseWriteOutputRegister(message);
            break;

        case SET_MULTIPLE_COILS:
            ParseWriteMultipleOutputCoils(message);
            break;

        case SET_MULTIPLE_REGISTERS:
            ParseWriteMultipleOutputRegisters(message);
            break;

        default:
            break;
        }
    }
}

void ModbusASCIIMaster::ReadOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::ReadOutputCoils NULL DATATABLE");
        qDebug() << ("ModbusASCIIMaster::ReadOutputCoils NULL DATATABLE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("Address + length >= dataTable->NumberOfOutputCoils()");
        qDebug() << ("Address + length >= dataTable->NumberOfOutputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_COILS);

    request.SetValue(5, address);

    request.SetValue(9, length);

    request.UpdateChecksum(false);

    request.AddEnd();

#if TEST_BUILD
    if(unitID != request.GetByte(1))
    {
        BaseLogger::LogUnitTest("ModbusASCIIMaster::ReadOutputCoils: Unit ID is incorrect");
    }
    if(request.FunctionCode() != READ_COILS)
    {
        BaseLogger::LogUnitTest("ModbusASCIIMaster::ReadOutputCoils: Function code is incorrect");
    }
    if(request.GetValue(5) != address)
    {
        BaseLogger::LogUnitTest("ModbusASCIIMaster::ReadOutputCoils: address is incorrect");
    }
    if(request.GetValue(9) != length)
    {
        BaseLogger::LogUnitTest("ModbusASCIIMaster::ReadOutputCoils: length is incorrect");
    }
#endif

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::ReadInputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::ReadInputCoils NULL DATABASE");
        qDebug() << ("ModbusASCIIMaster::ReadInputCoils NULL DATABASE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfInputCoils())
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::ReadInputCoils Address + length >= dataTable->NumberOfInputCoils()");
        qDebug() << ("ModbusASCIIMaster::ReadInputCoils Address + length >= dataTable->NumberOfInputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfInputCoils()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_DISCRETE_INPUTS);

    request.SetValue(5, address);

    request.SetValue(9, length);

    request.UpdateChecksum(false);

    request.AddEnd();

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::ReadOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::ReadOutputRegisters NULL DATABASE");
        qDebug() << ("ModbusASCIIMaster::ReadOutputRegisters NULL DATABASE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::ReadOutputRegisters Address + length >= dataTable->NumberOfOutputRegisters()");
        qDebug() << ("ModbusASCIIMaster::ReadOutputRegisters Address + length >= dataTable->NumberOfOutputRegisters()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_HOLDING_REGISTERS);

    request.SetValue(5, address);

    request.SetValue(9, length);

    request.UpdateChecksum(false);

    request.AddEnd();

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::ReadInputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::ReadInputRegisters NULL DATABASE");
        qDebug() << ("ModbusASCIIMaster::ReadInputRegisters NULL DATABASE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfInputRegisters())
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::ReadInputRegisters address + length >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusASCIIMaster::ReadInputRegisters address + length >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address + length >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_INPUT_REGISTERS);

    request.SetValue(5, address);

    request.SetValue(9, length);

    request.UpdateChecksum(false);

    request.AddEnd();

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::WriteOutputCoil(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::WriteOutputCoil NULL DATABASE");
        qDebug() << ("ModbusASCIIMaster::WriteOutputCoil NULL DATABASE");
        throw new std::exception();
    }
    else if(address >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::WriteOutputCoil address >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusASCIIMaster::WriteOutputCoil address >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_SINGLE_COIL);

    request.SetValue(5, address);

    request.SetValue(9, (dataTable->GetOutputCoil(address) ? 0xFF00 : 0x0000));

    request.UpdateChecksum(false);

    request.AddEnd();

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::WriteOutputRegister(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::WriteOutputRegister NULL DATABASE");
        qDebug() << ("ModbusASCIIMaster::WriteOutputRegister NULL DATABASE");
        throw new std::exception();
    }
    else if(address >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::WriteOutputRegister address >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusASCIIMaster::WriteOutputRegister address >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_SINGLE_REGISTER);

    request.SetValue(5, address);

    request.SetValue(9, dataTable->GetOutputRegister(address));

    request.UpdateChecksum(false);

    request.AddEnd();

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::WriteMultipleOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::WriteMultipleOutputCoils NULL DATABASE");
        qDebug() << ("ModbusASCIIMaster::WriteMultipleOutputCoils NULL DATABASE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("address + length >= dataTable->NumberOfOutputCoils()");
        qDebug() << ("address + length >= dataTable->NumberOfOutputCoils()");
        throw new std::out_of_range("address + length >= dataTable->NumberOfOutputCoils()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_MULTIPLE_COILS);

    request.SetValue(5, address);

    request.SetValue(9, length);

    unsigned short numBytes = (length + 7) / 8;
    request.SetByte(13, numBytes);

    uchar buffer[numBytes];
    dataTable->GetOutputCoils_charArr(address, length, buffer, numBytes);
    for(int i = 0; i < numBytes; i++)
    {
        request.SetByte(15 + (i * 2), buffer[i]);
    }

    //request.UpdateLength(1 + 14 + (length * 2));

    request.UpdateChecksum(false);

    request.AddEnd();

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::WriteMultipleOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIIMaster::WriteMultipleOutputRegisters NULL DATABASE");
        qDebug() << ("ModbusASCIIMaster::WriteMultipleOutputRegisters NULL DATABASE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("address + length >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("address + length >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address + length >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_MULTIPLE_REGISTERS);

    request.SetValue(5, address);

    request.SetValue(9, length);

    unsigned short numBytes = length*4;
    request.SetByte(13, numBytes);

    for(int i = 0; i < length; i++)
    {
        request.SetValue(15 + (i * 4), dataTable->GetOutputRegister(address + i));
    }

//    uchar buffer[numBytes];
//    dataTable->GetOutputRegisters_charArr(address, length, buffer);
//    for(int i = 0; i < numBytes; i++)
//    {
//        request.SetByte(15 + i, buffer[i]);
//    }

    //request.UpdateLength(14 + numBytes);

    request.UpdateChecksum(false);

    request.AddEnd();

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusASCIIMaster::ParseReadOutputCoils(ModbusMessage *)
{
    // This is just the sent message echoed back


}

void ModbusASCIIMaster::ParseReadInputCoils(ModbusMessage *)
{
    // This is just the sent message echoed back
}

void ModbusASCIIMaster::ParseReadOutputRegisters(ModbusMessage *response)
{
    dataTable->SetOutputRegisters(request.Address(), response->Buffer() + 3, request.Length());
}

void ModbusASCIIMaster::ParseReadInputRegisters(ModbusMessage *response)
{
    dataTable->SetInputRegisters(request.Address(), request.Length(), response->Buffer() + 3);
}

void ModbusASCIIMaster::ParseWriteOutputCoil(ModbusMessage *)
{
    //Request is echoed back

    //dataTable->SetOutputCoil(response->GetValue(2), response->GetValue(4) == 0xFF00);
}

void ModbusASCIIMaster::ParseWriteOutputRegister(ModbusMessage *)
{
    // Request is echoed back

    //dataTable->SetOutputRegister(response->GetValue(2), response->GetValue(4));
}

void ModbusASCIIMaster::ParseWriteMultipleOutputCoils(ModbusMessage *)
{
    // This is just the sent message echoed back
}

void ModbusASCIIMaster::ParseWriteMultipleOutputRegisters(ModbusMessage *)
{
    // This is just the sent message echoed back
}

void ModbusASCIIMaster::MoveToThread(QThread * thread)
{
    ModbusASCIIBase::MoveToThread(thread);
}
