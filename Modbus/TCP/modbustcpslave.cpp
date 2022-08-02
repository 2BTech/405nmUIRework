#include "modbustcpslave.h"

ModbusTCPSlave::ModbusTCPSlave() : response()
{
    tcpServer = new QTcpServer();

    // Handle accepting new connections
    connect(tcpServer, &QTcpServer::newConnection, this, &ModbusTCPSlave::OnPeerConnect);
    connect(tcpServer, &QTcpServer::acceptError, this, &ModbusTCPSlave::OnPeerSocketError);
}

unsigned char ModbusTCPSlave::UnitAddress()
{
    return unitID;
}

void ModbusTCPSlave::SetUnitAddress(unsigned char unitAddress)
{
    unitID = unitAddress;
}

void ModbusTCPSlave::SetConnectionInformation(QString targetIP, unsigned short port)
{
    if(tcpServer->isListening())
    {
        tcpServer->close();
    }

    this->targetIP = targetIP;
    this->port = port;

    //tcpServer.listen(QHostAddress(targetIP), port);
    if(!tcpServer->listen(QHostAddress::AnyIPv4, port))
    {
        qDebug() << ("Failed to start modbus slave tcp server Start(ushort) at " + QString::number(port) + ". " + tcpServer->errorString());
        //BaseLogger::LogCritical("Failed to start modbus slave tcp server Start(ushort) at " + QString::number(port) + ". " + tcpServer->errorString());
        return;
    }
//    else
//    {
//        BaseLogger::Log("ModbusTCPSlave is listening at " + QString::number(port));
//    }

//    BaseLogger::Log("ModbusTCPSlave::SetConnectionInformation is targeting " + QHostAddress(targetIP).toString() + " at " + QString::number(port));
//    BaseLogger::Log("Is listening: " + BaseLogger::BoolToString(tcpServer.isListening()));
}

bool ModbusTCPSlave::IsConnected()
{
    return tcpServer != Q_NULLPTR && tcpServer->isListening();
}

bool ModbusTCPSlave::OpenConnection()
{
    // If there is an existing connection, then there is nothing to do
    if(IsConnected())
    {
        return true;
    }
    // Attempt to have the server start listening to the given port
    if(tcpServer->listen(QHostAddress::AnyIPv4, port))
    {
        //BaseLogger::Log("ModbusTCPSlave::OpenConnection Is listening: " + BaseLogger::BoolToString(tcpServer->isListening()) + " at " + QString::number(port));
        qDebug() << ("ModbusTCPSlave::OpenConnection Is listening: ") << tcpServer->isListening() << " at " << QString::number(port);
        return true;
    }
    // Failed
    else
    {
        qDebug() << ("Modbus TCP server is listening: ") << tcpServer->isListening();
        //BaseLogger::Log("Modbus TCP server is listening: " + BaseLogger::BoolToString(tcpServer->isListening()));
        return false;
    }
}

void ModbusTCPSlave::CloseConnection()
{
    if(tcpServer->isListening())
    {
        tcpServer->close();
    }
    qDebug() << ("Is listening(CloseConnection): ") << tcpServer->isListening();
    //BaseLogger::Log("Is listening(CloseConnection): " + BaseLogger::BoolToString(tcpServer->isListening()));
}

void ModbusTCPSlave::OnPeerConnect()
{
    if(curConn != Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave is trying to accept new connection when one is already present");
        //BaseLogger::LogCritical("ModbusTCPSlave is trying to accept new connection when one is already present");
        tcpServer->pauseAccepting();
    }
    else
    {
        curConn = tcpServer->nextPendingConnection();

        connect(curConn, &QTcpSocket::readyRead, this, &ModbusTCPSlave::ReadyRead);
        connect(curConn, &QTcpSocket::disconnected, this, &ModbusTCPSlave::OnPeerDisconnect);

        // Currently only support one connection at a time
        tcpServer->pauseAccepting();

        emit NewConnection(Helpers::ParsePeerIP(curConn->peerAddress().toString()));

        qDebug() << ("Accepted new connection: " + Helpers::ParsePeerIP(curConn->peerAddress().toString()));
        //BaseLogger::Log("Accepted new connection: " + Helpers::ParsePeerIP(curConn->peerAddress().toString()));
    }
}

void ModbusTCPSlave::OnPeerSocketError(QTcpSocket::SocketError socketError)
{
    qDebug() << ("Accept error: " + QString::number(socketError));
    //BaseLogger::Log("Accept error: " + QString::number(socketError));
}

void ModbusTCPSlave::OnPeerDisconnect()
{
    sender()->deleteLater();
    emit ClosedConnection();
    curConn = Q_NULLPTR;
    //BaseLogger::Log("Peer disconnected");
    qDebug() << ("Peer disconnected");

    // Only allow 1 connection at a time, accepting is paused while a connection is held to prevent
    // a remote device being held up trying to connect to this
    tcpServer->resumeAccepting();

    currentMessage.Reset();
}

void ModbusTCPSlave::ReadyRead()
{
    char input [512];
    memset(input, 0, 512);
    while (!curConn->atEnd())
    {
        int count = curConn->read(input, 512);
        currentMessage.AddBytes(reinterpret_cast<unsigned char*>(input), count);

        if(currentMessage.ParseMessage())
        {
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("Modbus TCP Slave received message");
#endif // DEBUG_MODBUS_TCP_SLAVE
            //BaseLogger::Log("ModbusTCPSlave::ReadyRead: " + BaseLogger::CollectionToString(curMessage.Buffer(), curMessage.Length()));
            HandleMessage(&currentMessage);
            currentMessage.Reset();
        }
    }
}

void ModbusTCPSlave::HandleMessage(ModbusMessage *message)
{
    ModbusTCPMessage* tcpMessage = dynamic_cast<ModbusTCPMessage*>(message);
    if(tcpMessage != Q_NULLPTR)
    {
        HandleMessage(tcpMessage);
    }
    else
    {
        qDebug() << ("ModbusTCPSlave recieved null or non ModbusTCPMessage message");
        //BaseLogger::LogCritical("ModbusTCPSlave recieved null or non ModbusTCPMessage message");
    }
}

void ModbusTCPSlave::HandleMessage(ModbusTCPMessage *message)
{
    if(message == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::HandleMessage: Message is null");
        //BaseLogger::LogCritical("ModbusTCPSlave::HandleMessage: Message is null");
        throw "ModbusTCPSlave::HandleMessage: Message is null";
    }

    currentMessage = ModbusTCPMessage(message);

    // Check if the message is a broadcast or is for this device
    if(message->UnitAddress() == 0 || message->UnitAddress() == unitID)
    {
#ifdef DEBUG_MODBUS_TCP_SLAVE
        BaseLogger::Log("Function code: " + QString::number(message->FunctionCode()));
#endif // DEBUG_MODBUS_TCP_SLAVE
        switch (message->FunctionCode())
        {
        case READ_COILS:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave READ_COILS");
#endif // DEBUG_MODBUS_TCP_SLAVE
            //BaseLogger::Log(BaseLogger::CollectionToString(message->Buffer(), message->Length()));
            ReadOutputCoils(message->Address(), message->ValueCount());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        case READ_DISCRETE_INPUTS:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave READ_DISCRETE_INPUTS");
#endif // DEBUG_MODBUS_TCP_SLAVE
            ReadInputCoils(message->Address(), message->ValueCount());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        case READ_HOLDING_REGISTERS:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave READ_HOLDING_REGISTERS");
#endif // DEBUG_MODBUS_TCP_SLAVE
            ReadOutputRegisters(message->Address(), message->ValueCount());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        case READ_INPUT_REGISTERS:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave READ_INPUT_REGISTERS");
#endif // DEBUG_MODBUS_TCP_SLAVE
            ReadInputRegisters(message->Address(), message->ValueCount());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        case SET_SINGLE_COIL:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave SET_SINGLE_COIL");
#endif // DEBUG_MODBUS_TCP_SLAVE
            WriteOutputCoil(message->Address());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        case SET_SINGLE_REGISTER:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave SET_SINGLE_REGISTER");
#endif // DEBUG_MODBUS_TCP_SLAVE
            WriteOutputRegister(message->Address());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        case SET_MULTIPLE_COILS:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave SET_MULTIPLE_COILS");
#endif //DEBUG_MODBUS_TCP_SLAVE
            WriteMultipleOutputCoils(message->Address(), message->ValueCount());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        case SET_MULTIPLE_REGISTERS:
#ifdef DEBUG_MODBUS_TCP_SLAVE
            BaseLogger::Log("ModbusTCPSlave SET_MULTIPLE_REGISTERS");
#endif // DEBUG_MODBUS_TCP_SLAVE
            WriteMultipleOutputRegisters(message->Address(), message->ValueCount());
            //BaseLogger::Log("ModbusTCPSlave finished");
            break;

        default:
            qDebug() << ("Modbus RTU slave received bad function code: " + QString::number(message->FunctionCode()));
            //BaseLogger::LogCritical("Modbus RTU slave received bad function code: " + QString::number(message->FunctionCode()));
            break;
        }
    }
}

void ModbusTCPSlave::ReadOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::ReadOutputCoils no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::ReadOutputCoils no database");
        //throw new std::exception();;
        WriteError(READ_COILS, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address + length > dataTable->NumberOfOutputCoils())
    {
        qDebug() << ("ModbusTCPSlave::ReadOutputCoils out of bounds");
        //BaseLogger::LogCritical("ModbusTCPSlave::ReadOutputCoils out of bounds");
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
        WriteError(READ_COILS, ILLEGAL_DATA_ADDRESS);
        return;
    }

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_COILS);

    response.SetByte(8, (length + 7) / 8);

    dataTable->GetOutputCoils_charArr(address, length, response.Buffer() + 9, (length + 7) / 8);

    response.UpdateLength(9 + response.GetByte(8));

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::ReadInputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::ReadOutputCoils no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::ReadOutputCoils no database");
        //throw new std::exception();;
        WriteError(READ_DISCRETE_INPUTS, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address + length >= dataTable->NumberOfInputCoils())
    {
        qDebug() << ("ModbusTCPSlave::ReadInputCoils out of bounds");
        //BaseLogger::LogCritical("ModbusTCPSlave::ReadInputCoils out of bounds");
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfInputCoils()");
        WriteError(READ_DISCRETE_INPUTS, ILLEGAL_DATA_ADDRESS);
        return;
    }

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_DISCRETE_INPUTS);

    response.SetByte(8, (length + 7) / 8);

    dataTable->GetInputCoils_charArr(address, length, response.Buffer() + 9, (length + 7) / 8);

    response.UpdateLength(9 + response.GetByte(8));

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::ReadOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::ReadOutputRegisters no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::ReadOutputRegisters no database");
        //throw new std::exception();;
        WriteError(READ_HOLDING_REGISTERS, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address + length > dataTable->NumberOfOutputRegisters())
    {
//        BaseLogger::Log("Address: " + QString::number(address));
//        BaseLogger::Log("Length: " + QString::number(length));

//        BaseLogger::Log("Num output coils: " + QString::number(dataTable->NumberOfOutputCoils()));
//        BaseLogger::Log("Num input coils: " + QString::number(dataTable->NumberOfInputCoils()));
//        BaseLogger::Log("Num output registers: " + QString::number(dataTable->NumberOfOutputRegisters()));
//        BaseLogger::Log("Num input registers: " + QString::number(dataTable->NumberOfInputRegisters()));

        qDebug() << ("Address + length >= dataTable->NumberOfOutputRegisters()");
        //BaseLogger::LogCritical("Address + length >= dataTable->NumberOfOutputRegisters()");
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputRegisters()");
        WriteError(READ_HOLDING_REGISTERS, ILLEGAL_DATA_ADDRESS);
        return;
    }

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_HOLDING_REGISTERS);

    response.SetByte(8, length * 2);

    dataTable->GetOutputRegisters_charArr(address, length, response.PDU() + 2);

    response.UpdateLength(9 + response.GetByte(8));

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::ReadInputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::ReadInputRegisters no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::ReadInputRegisters no database");
        //throw new std::exception();;
        WriteError(READ_INPUT_REGISTERS, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address + length >= dataTable->NumberOfInputRegisters())
    {
        qDebug() << ("Address + length >= dataTable->NumberOfInputRegisters()");
        //BaseLogger::LogCritical("Address + length >= dataTable->NumberOfInputRegisters()");
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfInputRegisters()");
        WriteError(READ_INPUT_REGISTERS, ILLEGAL_DATA_ADDRESS);
        return;
    }

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(READ_INPUT_REGISTERS);

    response.SetByte(8, length * 2);

    dataTable->GetInputRegisters_charArr(address, length, response.PDU() + 2);

    response.UpdateLength(9 + response.GetByte(8));

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::WriteOutputCoil(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::WriteOutputCoil no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::WriteOutputCoil no database");
        //throw new std::exception();;
        WriteError(SET_SINGLE_COIL, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address >= dataTable->NumberOfOutputCoils())
    {
        qDebug() << ("Address >= dataTable->NumberOfOutputCoils()");
        //BaseLogger::LogCritical("Address >= dataTable->NumberOfOutputCoils()");
        //throw new std::out_of_range("Address >= dataTable->NumberOfOutputCoils()");
        WriteError(SET_SINGLE_COIL, ILLEGAL_DATA_ADDRESS);
        return;
    }

    dataTable->SetOutputCoil(address, currentMessage.GetValue(10) == 0xFF00);

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_SINGLE_COIL);

    response.SetValue(8, address);

    response.SetValue(10, currentMessage.GetValue(10));

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::WriteOutputRegister(unsigned short address)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::WriteOutputRegister no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::WriteOutputRegister no database");
        //throw new std::exception();;
        WriteError(SET_SINGLE_REGISTER, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address >= dataTable->NumberOfOutputRegisters())
    {
        qDebug() << ("Address >= dataTable->NumberOfOutputRegisters()");
        //BaseLogger::LogCritical("Address >= dataTable->NumberOfOutputRegisters()");
        //throw new std::out_of_range("Address >= dataTable->NumberOfOutputRegisters()");
        WriteError(SET_SINGLE_REGISTER, ILLEGAL_DATA_ADDRESS);
        return;
    }

    dataTable->SetOutputRegister(address, currentMessage.GetValue(10));

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_SINGLE_REGISTER);

    response.SetValue(8, address);

    response.SetValue(10, currentMessage.GetValue(10));

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::WriteMultipleOutputCoils(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::WriteMultipleOutputCoils no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::WriteMultipleOutputCoils no database");
        //throw new std::exception();;
        WriteError(SET_MULTIPLE_COILS, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address + length >= dataTable->NumberOfOutputCoils())
    {
        qDebug() << ("Address + length >= dataTable->NumberOfOutputCoils()");
        //BaseLogger::LogCritical("Address + length >= dataTable->NumberOfOutputCoils()");
        //throw new std::out_of_range("Address + length >= dataTable->NumberOfOutputCoils()");
        WriteError(SET_MULTIPLE_COILS, ILLEGAL_DATA_ADDRESS);
        return;
    }

//    BaseLogger::Log("Modbus TCP Slave is writing multiple output coils starting at " + QString::number(address) + " for " + QString::number(length));
//    BaseLogger::Log("Bytes: " + BaseLogger::CollectionToString(currentMessage.PDU() + 6, (length + 7) / 8));
//    BaseLogger::Log("Buffer: " + BaseLogger::CollectionToString(currentMessage.Buffer(), currentMessage.Length()));

//    BaseLogger::Log("Address: " + QString::number(address));
//    BaseLogger::Log("Length: " + QString::number(length));

    dataTable->SetOutputCoils(address, length, currentMessage.PDU() + 6);

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_MULTIPLE_COILS);

    response.SetValue(8, address);

    response.SetValue(10, length);

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::WriteMultipleOutputRegisters(unsigned short address, unsigned short length)
{
    if(dataTable == Q_NULLPTR)
    {
        qDebug() << ("ModbusTCPSlave::WriteMultipleOutputRegisters no database");
        //BaseLogger::LogCritical("ModbusTCPSlave::WriteMultipleOutputRegisters no database");
        //throw new std::exception();;
        WriteError(SET_MULTIPLE_REGISTERS, SLAVE_DEVICE_FAILURE);
        return;
    }
    else if(address + length >= dataTable->NumberOfOutputRegisters())
    {
        qDebug() << ("Address + length >= dataTable->NumberOfOutputRegisters()");
        //BaseLogger::LogCritical("Address + length >= dataTable->NumberOfOutputRegisters()");
        //throw new std::out_of_range(__PRETTY_FUNCTION__);
        WriteError(SET_MULTIPLE_REGISTERS, ILLEGAL_DATA_ADDRESS);
        return;
    }

    qDebug() << ("Modbus TCP Slave is in WriteMultipleOutputRegisters");
    qDebug() << ("Address: " + QString::number(address) + " Length: " + length);

    //BaseLogger::Log("Modbus TCP Slave is in WriteMultipleOutputRegisters");
    //BaseLogger::Log("Address: " + QString::number(address) + " Length: " + length);

    dataTable->SetOutputRegisters(address, currentMessage.PDU() + 6, length);

    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(SET_MULTIPLE_REGISTERS);

    response.SetValue(8, address);

    response.SetValue(10, length);

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::WriteError(unsigned char functionCode, unsigned char error)
{
    response.Reset();

    response.SetTransactionID(currentMessage.TransactionID());

    response.SetProtocol(currentMessage.Protocol());

    response.SetUnitAddress(unitID);

    response.SetFunctionCode(functionCode + 0x80);

    response.SetByte(8, error);

    response.UpdatePackageLength();

#if !TEST_BUILD
    curConn->write(reinterpret_cast<char*>(response.Buffer()), response.Length());
#endif // !TEST_BUILD
}

void ModbusTCPSlave::MoveToThread(QThread* thread)
{
    tcpServer->moveToThread(thread);
    if(curConn != Q_NULLPTR)
    {
        curConn->moveToThread(thread);
    }
    this->moveToThread(thread);
}
