#include "modbustcpmaster.h"

ModbusTCPMaster::ModbusTCPMaster() : request()
{
    transactionID = 0;
}

void ModbusTCPMaster::SetConnectionInformation(QString targetIP, unsigned short port)
{
    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        //BaseLogger::LogCritical("Trying to set connection information while tcp socket is open");
        qDebug() << ("Trying to set connection information while tcp socket is open");
    }
    else
    {
        this->port = port;
        this->targetIP = targetIP;
    }
}

bool ModbusTCPMaster::IsConnected()
{
    if(tcpSocket != Q_NULLPTR)
    {
        return tcpSocket->isOpen();
    }
    else
    {
        return false;
    }
}

bool ModbusTCPMaster::OpenConnection()
{
    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        return false;
    }
    else
    {
        if(tcpSocket != Q_NULLPTR)
        {
            tcpSocket->deleteLater();
        }

        tcpSocket = new QTcpSocket(this);
        // ToDo, this is wrong but dont have time to fix and then test, should already be in proper thread?
        tcpSocket->moveToThread(thread());
        tcpSocket->connectToHost(targetIP, port, QIODevice::ReadWrite);
        return tcpSocket->isOpen();
    }
}

void ModbusTCPMaster::CloseConnection()
{
    if(tcpSocket != Q_NULLPTR)
    {
        tcpSocket->close();
        tcpSocket->deleteLater();
    }
}

void ModbusTCPMaster::ReadyRead()
{
    char input [512] {};
    memset(input, 0, 512);
    while (!tcpSocket->atEnd())
    {
        int count = tcpSocket->read(input, 512);
        currentMessage.AddBytes(reinterpret_cast<unsigned char*>(input), count);

        if(currentMessage.ParseMessage())
        {
            //BaseLogger::Log("Modbus TCP master received message");
            qDebug() << ("Modbus TCP master received message");
            //BaseLogger::Log("ModbusTCPSlave::ReadyRead: " + BaseLogger::CollectionToString(curMessage.Buffer(), curMessage.Length()));
            HandleMessage(&currentMessage);
            currentMessage.Reset();
        }
    }
}

void ModbusTCPMaster::HandleMessage(ModbusMessage *message)
{
    ModbusTCPMessage* tcpMessage = dynamic_cast<ModbusTCPMessage*>(message);

    if(tcpMessage != Q_NULLPTR)
    {
        HandleMessage(tcpMessage);
    }
    else
    {
        qDebug() << (QString(__PRETTY_FUNCTION__) + " failed to cast ModbusMessage to ModbusTCPMessage");
        //BaseLogger::LogCritical(QString(__PRETTY_FUNCTION__) + " failed to cast ModbusMessage to ModbusTCPMessage");
    }
}

void ModbusTCPMaster::HandleMessage(ModbusTCPMessage * message)
{
    if(message->UnitAddress() == 0 || message->UnitAddress() == unitID)
    {
        switch (message->FunctionCode())
        {
        case READ_COILS:
            //BaseLogger::Log("Parsing ReadOutputCoils");
            ParseReadOutputCoils(message);
            break;

        case READ_DISCRETE_INPUTS:
            //BaseLogger::Log("Parsing ReadInputCoils");
            ParseReadInputCoils(message);
            break;

        case READ_HOLDING_REGISTERS:
            //BaseLogger::Log("Parsing ReadOutputRegisters");
            ParseReadOutputRegisters(message);
            break;

        case READ_INPUT_REGISTERS:
            //BaseLogger::Log("Parsing ReadInputRegisters");
            ParseReadInputRegisters(message);
            break;

        case SET_SINGLE_COIL:
            //BaseLogger::Log("Parsing WriteOutputCoil");
            ParseWriteOutputCoil(message);
            break;

        case SET_SINGLE_REGISTER:
            //BaseLogger::Log("Parsing OutputRegister");
            ParseWriteOutputRegister(message);
            break;

        case SET_MULTIPLE_COILS:
            //BaseLogger::Log("Parsing WriteMultipleOutputCoils");
            ParseWriteMultipleOutputCoils(message);
            break;

        case SET_MULTIPLE_REGISTERS:
            //BaseLogger::Log("Parsing WriteMultipleOutputRegisters");
            ParseWriteMultipleOutputRegisters(message);
            break;

        default:
            qDebug() << ("Received message with unhandled function code: " + QString::number(message->FunctionCode()));
            //BaseLogger::Log("Received message with unhandled function code: " + QString::number(message->FunctionCode()));
            break;
        }
    }
}

void ModbusTCPMaster::ReadOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    request.SetPackageLength(6);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_COILS);

    request.SetValue(8, address);

    request.SetValue(10, length);

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::ReadInputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfInputCoils())
    {
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfInputCoils()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    request.SetPackageLength(6);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_DISCRETE_INPUTS);

    request.SetValue(8, address);

    request.SetValue(10, length);

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::ReadOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    request.SetPackageLength(6);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_HOLDING_REGISTERS);

    request.SetValue(8, address);

    request.SetValue(10, length);

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::ReadInputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address + length >= dataTable->NumberOfInputRegisters())
    {
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfInputRegisters()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    request.SetPackageLength(6);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(READ_INPUT_REGISTERS);

    request.SetValue(8, address);

    request.SetValue(10, length);

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::WriteOutputCoil(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputCoils())
    {
        //throw new std::out_of_range("length >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    request.SetPackageLength(6);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_SINGLE_COIL);

    request.SetValue(8, address);

    if(dataTable->GetOutputCoil(address))
    {
        request.SetValue(10, 0xFF00);
    }
    else
    {
        request.SetValue(10, 0);
    }

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::WriteOutputRegister(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputRegisters())
    {
        //throw new std::out_of_range("length >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    request.SetPackageLength(6);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_SINGLE_REGISTER);

    request.SetValue(8, address);

    request.SetValue(10, dataTable->GetOutputRegister(address));

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::WriteMultipleOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputCoils())
    {
        //throw new std::out_of_range("length >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    // This is updated at then end
    request.SetPackageLength(5);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_MULTIPLE_COILS);

    // Set address bytes
    request.SetValue(8, address);

    // Set length bytes
    request.SetValue(10, length);

    // Set number of remaining bytes
    request.SetByte(12, (length+7)/8);

    // Append bytes the coils have been packed into
    dataTable->GetOutputCoils_charArr(address, length, request.Buffer() + 13, (length+7)/8);

    request.UpdateLength(13 + ((length+7)/8));

    request.UpdatePackageLength();

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::WriteMultipleOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(address >= dataTable->NumberOfOutputRegisters())
    {
        //throw new std::out_of_range("length >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    request.Reset();

    request.SetTransactionID(transactionID++);

    if(transactionID > 65000)
    {
        transactionID = 0;
    }

    request.SetProtocol(0);

    request.SetPackageLength(5);

    request.SetUnitAddress(unitID);

    request.SetFunctionCode(SET_MULTIPLE_REGISTERS);

    request.SetValue(8, address);

    request.SetValue(10, length);

    request.SetByte(12, length * 2);

    dataTable->GetOutputRegisters_charArr(address, length, request.Buffer() + 12);

    request.UpdateLength(13 + (length * 2));

    request.UpdatePackageLength();

    if(tcpSocket != Q_NULLPTR && tcpSocket->isOpen())
    {
        tcpSocket->write(reinterpret_cast<char*>(request.Buffer()), request.Length());
    }
    else
    {
        qDebug() << ("ModbusTCPMaster trying to write message to null or closed socket");
        //BaseLogger::LogCritical("ModbusTCPMaster trying to write message to null or closed socket");
    }
}

void ModbusTCPMaster::ParseReadOutputCoils(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(request.Address() + request.ValueCount() > dataTable->NumberOfOutputCoils())
    {
        //throw new std::out_of_range("message->Address() >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    dataTable->SetOutputCoils(request.Address(), request.ValueCount(), message->PDU() + 2);
}

void ModbusTCPMaster::ParseReadInputCoils(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(request.Address() + request.ValueCount() > dataTable->NumberOfInputCoils())
    {
        //throw new std::out_of_range("message->Address() >= dataTable->NumberOfInputCoils()");
        throw new std::exception();
    }

//    BaseLogger::Log("Data byte: " + QString::number(*(message->PDU() + 1)));
//    BaseLogger::Log("Address: " + QString::number(request.Address()));
//    BaseLogger::Log("Num coils: " + QString::number(request.ValueCount()));

    dataTable->SetInputCoils(request.Address(), request.ValueCount(), message->PDU() + 2);
}

void ModbusTCPMaster::ParseReadOutputRegisters(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(request.Address() + request.ValueCount() >= dataTable->NumberOfOutputRegisters())
    {
        //throw new std::out_of_range("message->Address() >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    dataTable->SetOutputRegisters(request.Address(), message->PDU() + 2, request.ValueCount());
}

void ModbusTCPMaster::ParseReadInputRegisters(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

    if(dataTable == Q_NULLPTR)
    {
        throw new std::exception();;
    }
    else if(request.Address() + request.ValueCount() >= dataTable->NumberOfInputRegisters())
    {
        //throw new std::out_of_range("message->Address() >= dataTable->NumberOfOutputCoils()");
        throw new std::exception();
    }

    dataTable->SetInputRegisters(request.Address(), request.ValueCount(), message->PDU() + 2);
}

void ModbusTCPMaster::ParseWriteOutputCoil(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

    if(memcmp(message->Buffer(), request.Buffer(), 8) != 0)
    {
//        BaseLogger::LogCritical("Received unexpected response for " + QString(__PRETTY_FUNCTION__));
//        BaseLogger::LogCritical("M: " + BaseLogger::CollectionToString(message->Buffer(), 8));
//        BaseLogger::LogCritical("R: " + BaseLogger::CollectionToString(request.Buffer(), 8));

        qDebug() << ("Received unexpected response for " + QString(__PRETTY_FUNCTION__));
        qDebug() << ("M: ") << message->Buffer();
        qDebug() << ("R: ") << request.Buffer();
#if TEST_BUILD
        throw new failed_test_exception;
#endif // TEST_BUILD
    }
}

void ModbusTCPMaster::ParseWriteOutputRegister(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

    if(memcmp(message->Buffer(), request.Buffer(), 8) != 0)
    {
//        BaseLogger::LogCritical("Received unexpected response for " + QString(__PRETTY_FUNCTION__));
//        BaseLogger::LogCritical("M: " + BaseLogger::CollectionToString(message->Buffer(), 8));
//        BaseLogger::LogCritical("R: " + BaseLogger::CollectionToString(request.Buffer(), 8));

        qDebug() << ("Received unexpected response for " + QString(__PRETTY_FUNCTION__));
        qDebug() << ("M: ") << message->Buffer();
        qDebug() << ("R: ") << request.Buffer();

#if TEST_BUILD
        throw new failed_test_exception;
#endif // TEST_BUILD
    }
}

void ModbusTCPMaster::ParseWriteMultipleOutputCoils(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

//    if(memcmp(message->Buffer(), request.Buffer(), 8) != 0)
//    {
//        BaseLogger::LogCritical("Received unexpected response for " + QString(__PRETTY_FUNCTION__));
//        BaseLogger::LogCritical("M: " + BaseLogger::CollectionToString(message->Buffer(), 8));
//        BaseLogger::LogCritical("R: " + BaseLogger::CollectionToString(request.Buffer(), 8));
//#if TEST_BUILD
//        throw new failed_test_exception;
//#endif // TEST_BUILD
//    }
}

void ModbusTCPMaster::ParseWriteMultipleOutputRegisters(ModbusMessage * response)
{
    ModbusTCPMessage * message = dynamic_cast<ModbusTCPMessage*>(response);
    if(message == Q_NULLPTR)
    {
        throw new std::bad_cast();
    }

//    if(memcmp(message->Buffer(), request.Buffer(), 8) != 0)
//    {
//        BaseLogger::LogCritical("Received unexpected response for " + QString(__PRETTY_FUNCTION__));
//        BaseLogger::LogCritical("M: " + BaseLogger::CollectionToString(message->Buffer(), 8));
//        BaseLogger::LogCritical("R: " + BaseLogger::CollectionToString(request.Buffer(), 8));
//#if TEST_BUILD
//        throw new failed_test_exception;
//#endif // TEST_BUILD
//    }
}

void ModbusTCPMaster::MoveToThread(QThread* thread)
{
    if (tcpSocket != Q_NULLPTR)
    {
        tcpSocket->moveToThread(thread);
    }
    this->moveToThread(thread);
}
