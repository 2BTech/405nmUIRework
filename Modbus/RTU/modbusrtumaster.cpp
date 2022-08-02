#include "modbusrtumaster.h"

ModbusRTUMaster::ModbusRTUMaster()
{

}

void ModbusRTUMaster::HandleMessage(ModbusMessage * message)
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

void ModbusRTUMaster::ReadOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadOutputCoils NULL DATATABLE");
        qDebug() << ("ModbusRTUMaster::ReadOutputCoils NULL DATATABLE");
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadOutputCoils Address + length >= dataTable->NumberOfOutputCoils()");
        qDebug() << ("ModbusRTUMaster::ReadOutputCoils Address + length >= dataTable->NumberOfOutputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_COILS);

    request.SetValue(2, address);

    request.SetValue(4, length);

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::ReadInputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadInputCoils NULL DATATABLE");
        qDebug() << ("ModbusRTUMaster::ReadInputCoils NULL DATATABLE");
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfInputCoils())
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadInputCoils Address + length >= dataTable->NumberOfInputCoils()");
        qDebug() << ("ModbusRTUMaster::ReadInputCoils Address + length >= dataTable->NumberOfInputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfInputCoils()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_DISCRETE_INPUTS);

    request.SetValue(2, address);

    request.SetValue(4, length);

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::ReadOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadOutputRegisters NULL DATATABLE");
        qDebug() << ("ModbusRTUMaster::ReadOutputRegisters NULL DATATABLE");
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadOutputRegisters Address + length >= dataTable->NumberOfOutputRegisters()");
        qDebug() << ("ModbusRTUMaster::ReadOutputRegisters Address + length >= dataTable->NumberOfOutputRegisters()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_HOLDING_REGISTERS);

    request.SetValue(2, address);

    request.SetValue(4, length);

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::ReadInputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadInputRegisters NULL DATATABLE");
        qDebug() << ("ModbusRTUMaster::ReadInputRegisters NULL DATATABLE");
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfInputRegisters())
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::ReadInputRegisters address + length >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusRTUMaster::ReadInputRegisters address + length >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address + length >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_INPUT_REGISTERS);

    request.SetValue(2, address);

    request.SetValue(4, length);

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::WriteOutputCoil(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::WriteOutputCoil NULL DATATABLE");
        qDebug() << ("ModbusRTUMaster::WriteOutputCoil NULL DATATABLE");
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::WriteOutputCoil address >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusRTUMaster::WriteOutputCoil address >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_SINGLE_COIL);

    request.SetValue(2, address);

    request.SetValue(4, (dataTable->GetOutputCoil(address) ? 0xFF00 : 0x0000));

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::WriteOutputRegister(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::WriteOutputRegister NULL DATATABLE");
        qDebug() << ("ModbusRTUMaster::WriteOutputRegister NULL DATATABLE");
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::WriteOutputRegister address >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusRTUMaster::WriteOutputRegister address >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_SINGLE_REGISTER);

    request.SetValue(2, address);

    request.SetValue(4, dataTable->GetOutputRegister(address));

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::WriteMultipleOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical(QString(__PRETTY_FUNCTION__) + ": NULL DATABASE");
        qDebug() << (QString(__PRETTY_FUNCTION__) + ": NULL DATABASE");
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("address + length >= dataTable->NumberOfInputOutputCoils()");
        qDebug() << ("address + length >= dataTable->NumberOfInputOutputCoils()");
        throw new std::out_of_range("address + length >= dataTable->NumberOfOutputCoils()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_MULTIPLE_COILS);

    // Address of first coil
    request.SetValue(2, address);

    // Number of coils being accessed
    request.SetValue(4, length);

    // Number of bytes remaining in message
    unsigned short numBytes = (length + 7) / 8;
    request.SetByte(6, numBytes);

    dataTable->GetOutputCoils_charArr(address, length, request.Buffer() + 7, numBytes);

//    uchar buffer[numBytes];
//    //dataTable->GetOutputRegisters_charArr(address, length, buffer);
//    for(int i = 0; i < numBytes; i++)
//    {
//        request.SetByte(14 + i, buffer[i]);
//    }

    request.UpdateLength(7 + numBytes);

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::WriteMultipleOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::WriteMultipleOutputRegisters NULL DATATABLE");
        qDebug() << ("ModbusRTUMaster::WriteMultipleOutputRegisters NULL DATATABLE");
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("ModbusRTUMaster::WriteMultipleOutputRegisters address + length >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusRTUMaster::WriteMultipleOutputRegisters address + length >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("address + length >= dataTable->NumberOfInputRegisters()");
    }

    request.Reset();

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_MULTIPLE_REGISTERS);

    request.SetValue(2, address);

    request.SetValue(4, length);

    request.SetByte(6, length * 2);

    dataTable->GetOutputRegisters_charArr(address, length, request.Buffer() + 7);

    request.UpdateLength(7 + (length * 2));

    request.UpdateChecksum(false);

    serialPort.write(reinterpret_cast<char*>(request.Buffer()), request.Length());
}

void ModbusRTUMaster::ParseReadOutputCoils(ModbusMessage *)
{
    // The expected response is for the request to be echoed back
}

void ModbusRTUMaster::ParseReadInputCoils(ModbusMessage *)
{
    // The expected response is for the request to be echoed back
}

void ModbusRTUMaster::ParseReadOutputRegisters(ModbusMessage *response)
{
    dataTable->SetOutputRegisters(request.Address(), response->Buffer() + 3, request.Length());
}

void ModbusRTUMaster::ParseReadInputRegisters(ModbusMessage *response)
{
    dataTable->SetInputRegisters(request.Address(), request.Length(), response->Buffer() + 3);
}

void ModbusRTUMaster::ParseWriteOutputCoil(ModbusMessage *)
{
    // The expected response is for the request to be echoed back
}

void ModbusRTUMaster::ParseWriteOutputRegister(ModbusMessage *)
{
    // The expected response is for the request to be echoed back
}

void ModbusRTUMaster::ParseWriteMultipleOutputCoils(ModbusMessage *)
{
    // The expected response is for the request to be echoed back
}

void ModbusRTUMaster::ParseWriteMultipleOutputRegisters(ModbusMessage *)
{
    // The expected response is for the request to be echoed back
}

void ModbusRTUMaster::MoveToThread(QThread * thread)
{
    ModbusRTUBase::MoveToThread(thread);
}
