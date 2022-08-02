#include "modbusasciislave.h"

ModbusASCIISlave::ModbusASCIISlave() : ModbusASCIIBase(), ModbusSlave(), response()
{
    //BaseLogger::Log("Creating modbus ascii slave");
}

ModbusASCIISlave::ModbusASCIISlave(ModbusASCIISlave* other) : ModbusASCIIBase(), ModbusSlave(), response()
{
    serialPort.setPortName(other->serialPort.portName());
    serialPort.setBaudRate(other->serialPort.baudRate());
}

void ModbusASCIISlave::HandleMessage(ModbusMessage * message)
{
    memcpy(currentMessage.Buffer(), message->Buffer(), message->Length());
    currentMessage.UpdateLength(message->Length());

    // Check if the message is a broadcast or is for this device
    if(message->UnitAddress() == 0 || message->UnitAddress() == unitID)
    {
        switch (message->FunctionCode())
        {
        case READ_COILS:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave READ_COILS");
            qDebug() << ("ModbusASCIISlave READ_COILS");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            ReadOutputCoils(message->Address(), message->ValueCount());
            break;

        case READ_DISCRETE_INPUTS:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave READ_DISCRETE_INPUTS");
            qDebug() << ("ModbusASCIISlave READ_DISCRETE_INPUTS");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            ReadInputCoils(message->Address(), message->ValueCount());
            break;

        case READ_HOLDING_REGISTERS:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave READ_HOLDING_REGISTERS");
            qDebug() << ("ModbusASCIISlave READ_HOLDING_REGISTERS");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            ReadOutputRegisters(message->Address(), message->ValueCount());
            break;

        case READ_INPUT_REGISTERS:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave READ_INPUT_REGISTERS");
            qDebug() << ("ModbusASCIISlave READ_INPUT_REGISTERS");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            ReadInputRegisters(message->Address(), message->ValueCount());
            break;

        case SET_SINGLE_COIL:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave SET_SINGLE_COIL");
            qDebug() << ("ModbusASCIISlave SET_SINGLE_COIL");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            WriteOutputCoil(message->Address());
            break;

        case SET_SINGLE_REGISTER:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave SET_SINGLE_REGISTER");
            qDebug() << ("ModbusASCIISlave SET_SINGLE_REGISTER");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            WriteOutputRegister(message->Address());
            break;

        case SET_MULTIPLE_COILS:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave SET_MULTIPLE_COILS");
            qDebug() << ("ModbusASCIISlave SET_MULTIPLE_COILS");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            WriteMultipleOutputCoils(message->Address(), message->ValueCount());
            break;

        case SET_MULTIPLE_REGISTERS:
#ifdef DEBUG_MODBUS_ASCII_SLAVE
            //BaseLogger::Log("ModbusASCIISlave SET_MULTIPLE_REGISTERS");
            qDebug() << ("ModbusASCIISlave SET_MULTIPLE_REGISTERS");
#endif // DEBUG_MODBUS_ASCII_SLAVE
            WriteMultipleOutputRegisters(message->Address(), message->ValueCount());
            break;

        default:
            //BaseLogger::LogCritical("Modbus ASCII slave received bad function code: " + QString::number(message->FunctionCode()));
            qDebug() << ("Modbus ASCII slave received bad function code: " + QString::number(message->FunctionCode()));
            break;
        }
    }
#if TEST_BUILD
    else
    {
        BaseLogger::Log("Not parsing message: " + QString::number(message->UnitAddress()) + " != " + QString::number(unitID));
    }
#endif
}

void ModbusASCIISlave::ReadOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadOutputCoils NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::ReadOutputCoils NULL DATATABLE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadOutputCoils Address + length >= dataTable->NumberOfOutputCoils()");
        qDebug() << ("ModbusASCIISlave::ReadOutputCoils Address + length >= dataTable->NumberOfOutputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    response.Reset();

    response.SetStart();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_COILS);

    response.SetByte(5, ((length + 7) / 8));

    int len = (length + 7) / 8;
    char buffer[len];
    memset(buffer, 0, len);
    dataTable->GetOutputCoils_charArr(address, length, reinterpret_cast<unsigned char*>(buffer), len);
    for(int i = 0; i < len; i++)
    {
        //BaseLogger::Log("Setting byte: " + QString::number(7 + (i * 2)));
        response.SetByte(7 + (i * 2), buffer[i]);
    }

    response.UpdateLength(7 + (response.GetByte(5) * 2));

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
#ifdef DEBUG_MODBUS_ASCII_SLAVE
    //BaseLogger::Log("ASCII slave is sending READ_COILS response: " + BaseLogger::CollectionToString(response.Buffer(), response.Length()));
    qDebug() << "ASCII slave is sending READ_COILS response: " << response.Buffer();
#endif // DEBUG_MODBUS_ASCII_SLAVE
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusASCIISlave::ReadInputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadInputCoils NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::ReadInputCoils NULL DATATABLE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfInputCoils())
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadInputCoils Address + length >= dataTable->NumberOfInputCoils()");
        qDebug() << ("ModbusASCIISlave::ReadInputCoils Address + length >= dataTable->NumberOfInputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfInputCoils()");
    }

    response.Reset();

    response.SetStart();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_DISCRETE_INPUTS);

    response.SetByte(5, (length + 7) / 8);

    int len = (length + 7) / 8;
    char buffer[len];
    memset(buffer, 0, len);
    dataTable->GetInputCoils_charArr(address, length, reinterpret_cast<unsigned char*>(buffer), len);
    for(int i = 0; i < len; i++)
    {
        //BaseLogger::Log("Setting byte: " + QString::number(7 + (i * 2)));
        response.SetByte(7 + (i * 2), buffer[i]);
    }

    response.UpdateLength(7 + (response.GetByte(5) * 2));

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusASCIISlave::ReadOutputRegisters(unsigned short address, unsigned short length)
{
    // DEBUG
    //BaseLogger::Log("Starting ModbusASCIISlave::ReadOutputRegisters");

    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadOutputRegisters NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::ReadOutputRegisters NULL DATATABLE");
        throw new std::exception();
    }
    else if(address + length > dataTable->NumberOfOutputRegisters())
    {
        qDebug() << ("ModbusASCIISlave::ReadOutputRegisters Address + length >= dataTable->NumberOfOutputRegisters()");
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadOutputRegisters Address + length >= dataTable->NumberOfOutputRegisters()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
    }

    response.Reset();

    // 0
    response.SetStart();

    //BaseLogger::Log("After set start: " + QByteArray().append(reinterpret_cast<char*>(response.Buffer())));

    // 1-2
    response.SetUnitAddress(unitID);

    //BaseLogger::Log("After set unit address: " + QByteArray().append(reinterpret_cast<char*>(response.Buffer())));

    // 3-4
    response.SetFunctionCode(READ_HOLDING_REGISTERS);

    //BaseLogger::Log("After set function code: " + QByteArray().append(reinterpret_cast<char*>(response.Buffer())));

    // 5-6, number of bytes
    response.SetByte(5, length * 2);

    //BaseLogger::Log("After set length byte: " + QByteArray().append(reinterpret_cast<char*>(response.Buffer())));

    // 7 - (7 + (2 * length))
    int len = length * 2;
    char buffer[len];
    memset(buffer, 0, len);
    dataTable->GetOutputRegisters_charArr(address, length, reinterpret_cast<unsigned char*>(buffer));
    //BaseLogger::Log("Adding registers of len: " + QString::number(len));
    qDebug() << ("Adding registers of len: " + QString::number(len));
    for(int i = 0; i < len; i++)
    {
        response.SetByte(7 + (i * 2), buffer[i]);
    }

    //BaseLogger::Log("After set data: " + QByteArray().append(reinterpret_cast<char*>(response.Buffer())));

    //BaseLogger::Log("Number of bytes: " + QString::number(response.GetByte(5) * 2));
    //response.UpdateLength(7 + (response.GetByte(5) * 2));

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
#ifdef DEBUG_MODBUS_ASCII_SLAVE
    //BaseLogger::Log("ModbusASCIISlave READ_HOLDING_REGISTERS response: " + BaseLogger::CollectionToString(reinterpret_cast<char*>(response.Buffer()), response.Length()));
    qDebug() << "ModbusASCIISlave READ_HOLDING_REGISTERS response: " << response.Buffer();
#endif // DEBUG_MODBUS_ASCII_SLAVE

    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusASCIISlave::ReadInputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadInputRegisters NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::ReadInputRegisters NULL DATATABLE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfInputRegisters())
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::ReadInputRegisters Address + length >= dataTable->NumberOfInputRegisters()");
        qDebug() << ("ModbusASCIISlave::ReadInputRegisters Address + length >= dataTable->NumberOfInputRegisters()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfInputRegisters()");
    }

    response.Reset();

    response.SetStart();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_INPUT_REGISTERS);

    response.SetByte(5, length * 2);

    int len = length * 2;
    char buffer[len];
    memset(buffer, 0, len);
    dataTable->GetInputRegisters_charArr(address, length, reinterpret_cast<unsigned char*>(buffer));
    for(int i = 0; i < len; i++)
        response.SetByte(7 + (i * 2), buffer[i]);

    response.UpdateLength(7 + (response.GetByte(5) * 2));

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}


void ModbusASCIISlave::WriteOutputCoil(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteOutputCoil NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::WriteOutputCoil NULL DATATABLE");
        throw new std::exception();
    }
    else if(address >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteOutputCoil Address + length >= dataTable->NumberOfOutputCoils()");
        qDebug() << ("ModbusASCIISlave::WriteOutputCoil Address + length >= dataTable->NumberOfOutputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    //BaseLogger::Log("Setting coil: " + BaseLogger::BoolToString(curMessage.GetValue(9) == 0xFF00));
    //BaseLogger::Log("Value: " + QString::number(curMessage.GetValue(9)) + " : " + QString::number(curMessage.GetValue(9), 16));
    //BaseLogger::Log("Cur message: " + QString().append(reinterpret_cast<char*>(curMessage.Buffer())));
    dataTable->SetOutputCoil(address, currentMessage.GetValue(9) == 0xFF00);

    response.Reset();

    response.SetStart();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_SINGLE_COIL);

    // Coil address
    //BaseLogger::Log("Address: " + QString::number(address));
    response.SetValue(5, address);

    // Coil value
    response.SetValue(9, currentMessage.GetValue(9));
    //response.SetByte(11, 0);

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusASCIISlave::WriteOutputRegister(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteOutputRegister NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::WriteOutputRegister NULL DATATABLE");
        throw new std::exception();
    }
    else if(address >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteOutputRegister Address + length >= dataTable->NumberOfOutputRegisters()");
        qDebug() << ("ModbusASCIISlave::WriteOutputRegister Address + length >= dataTable->NumberOfOutputRegisters()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
    }

    response.Reset();

    response.SetStart();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_SINGLE_REGISTER);

    // Address - 4 bytes
    response.SetValue(5, address);

    // Register value - 4 bytes
    //BaseLogger::Log("Address: " + QString::number(address) + " : " + QString::number(address, 16));
    qDebug() << ("Address: " + QString::number(address) + " : " + QString::number(address, 16));
    response.SetValue(9, currentMessage.GetValue(9));

    // Appy the new value
    //BaseLogger::Log("Setting register to " + QString::number(currentMessage.GetValue(9)) + " : " + QString::number(currentMessage.GetValue(9), 16) + " : " + QString::number(currentMessage.GetValue(9)));
    qDebug() << ("Setting register to " + QString::number(currentMessage.GetValue(9)) + " : " + QString::number(currentMessage.GetValue(9), 16) + " : " + QString::number(currentMessage.GetValue(9)));
    dataTable->SetOutputRegister(address, currentMessage.GetValue(9));

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusASCIISlave::WriteMultipleOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteMultipleOutputCoils NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::WriteMultipleOutputCoils NULL DATATABLE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteMultipleOutputCoils Address + length >= dataTable->NumberOfOutputCoils()");
        qDebug() << ("ModbusASCIISlave::WriteMultipleOutputCoils Address + length >= dataTable->NumberOfOutputCoils()");
        throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
    }

    response.Reset();

    response.SetStart();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_MULTIPLE_COILS);

    response.SetValue(2, address);

    response.SetValue(4, length);

    response.SetByte(6, (length + 7)/8);

    dataTable->GetOutputCoils_charArr(address, length, currentMessage.PDU() + 6, (length + 7)/8);

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusASCIISlave::WriteMultipleOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteMultipleOutputRegisters NULL DATATABLE");
        qDebug() << ("ModbusASCIISlave::WriteMultipleOutputRegisters NULL DATATABLE");
        throw new std::exception();
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        //BaseLogger::LogCritical("ModbusASCIISlave::WriteMultipleOutputRegisters address out of range");
        qDebug() << ("ModbusASCIISlave::WriteMultipleOutputRegisters address out of range");
        throw new std::out_of_range(__PRETTY_FUNCTION__);
    }

    response.Reset();

    response.SetStart();

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_MULTIPLE_REGISTERS);

    response.SetValue(5, address);

    response.SetValue(9, length);

    //unsigned char * bytes = curMessage.Buffer() + 7;
    for(int i = 0; i < length; i++)
    {
        //BaseLogger::Log("Setting byte " + QString::number(address + i) + " to " + QString::number(currentMessage.GetValue(13 + (i * 4))));
        qDebug() << ("Setting byte " + QString::number(address + i) + " to " + QString::number(currentMessage.GetValue(13 + (i * 4))));
        dataTable->SetOutputRegister(address + i, currentMessage.GetValue(13 + (i * 4)));
        //dataTable->SetOutputRegister(address + i, (bytes[0] << 8) | bytes[1]);
        //bytes += 2;
    }

    dataTable->GetOutputRegisters_charArr(address, length, currentMessage.PDU() + 6);

    response.UpdateChecksum(false);

    response.AddEnd();

#if !TEST_BUILD
    serialPort.write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif
}

void ModbusASCIISlave::MoveToThread(QThread * thread)
{
    ModbusASCIIBase::MoveToThread(thread);
}
