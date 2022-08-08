#include "modbushandler.h"

ModbusHandler::ModbusHandler()
{
    //Set up ping timer to continously run when started
    //pingTimer.setSingleShot(false);
    //connect(&pingTimer, &QTimer::timeout, this, &ModbusHandler::PingSlave);

    connect(&dataTable, &ModbusDatatable::OutputCoilChanged, this, &ModbusHandler::UpdatedOutputCoil);
    connect(&dataTable, &ModbusDatatable::OutputRegisterChanged, this, &ModbusHandler::UpdatedOutputRegister);
}

ModbusHandler * ModbusHandler::instance = Q_NULLPTR;
ModbusHandler * ModbusHandler::GetInstance()
{
    if(instance == Q_NULLPTR)
    {
        instance = new ModbusHandler();
    }
    return instance;
}

ModbusDatatable * ModbusHandler::DataTable()
{
    return &dataTable;
}

int ModbusHandler::ProtocolID()
{
    return currentProtocol;
}

QString ModbusHandler::ProtocolName()
{
    switch (currentProtocol)
    {
    // TCP
    case 0:
        return "TCP";

    // RTU
    case 1:
        return "RTU";

    // ASCII
    case 2:
        return "ASCII";

    default:
        return "ERROR";
    }
}

bool ModbusHandler::IsMaster()
{
    return master != Q_NULLPTR;
}

bool ModbusHandler::IsSlave()
{
    return slave != Q_NULLPTR;
}

bool ModbusHandler::IsConnected()
{
    return (IsMaster() && reinterpret_cast<ModbusBase*>(master)->IsConnected()) || (IsSlave() && reinterpret_cast<ModbusBase*>(slave)->IsConnected());
}

void ModbusHandler::UpdateModbusSettings(int protocolType, QString target, int port_baudRate, unsigned char unitID)
{
    //BaseLogger::Log("Updating ModbusHandler to use protocol: " + QString::number(protocolType) + " targeting " + target + " at " + QString::number(port_baudRate) + " with an id of " + QString::number(unitID));
    //BaseLogger::Log("Modbus ID: " + QString::number(unitID));

    qDebug() << ("Updating ModbusHandler to use protocol: " + QString::number(protocolType) + " targeting " + target + " at " + QString::number(port_baudRate) + " with an id of " + QString::number(unitID));

    if(protocolType != currentProtocol)
    {
        currentProtocol = protocolType;
        if(IsConnected())
        {
            CloseModbus();
        }

        switch (protocolType)
        {
        // TCP
        case 0:
            slave = new ModbusTCPSlave();
            dynamic_cast<ModbusTCPSlave*>(slave)->SetConnectionInformation(target, port_baudRate);
            dynamic_cast<ModbusTCPSlave*>(slave)->SetInformation(unitID, &dataTable);
            if(!dynamic_cast<ModbusTCPSlave*>(slave)->OpenConnection())
            {
                //BaseLogger::LogCritical("Failed to open ModbusTCPSlave connection: " + dynamic_cast<ModbusTCPSlave*>(slave)->ErrorString());
                qDebug() << ("Failed to open ModbusTCPSlave connection: " + dynamic_cast<ModbusTCPSlave*>(slave)->ErrorString());
            }
            connect(dynamic_cast<ModbusTCPSlave*>(slave), &ModbusTCPSlave::NewConnection, this, &ModbusHandler::TCPSlaveConnection);
            break;

        // RTU
        case 1:
            slave = new ModbusRTUSlave();
            dynamic_cast<ModbusRTUSlave*>(slave)->SetSerialPortInformation(target, port_baudRate);
            dynamic_cast<ModbusRTUSlave*>(slave)->SetInformation(unitID, &dataTable);
            if(!dynamic_cast<ModbusRTUSlave*>(slave)->OpenConnection())
            {
                qDebug() << ("Failed to open ModbusRTUSlave connection: " + dynamic_cast<ModbusRTUSlave*>(slave)->ErrorString());
                //BaseLogger::LogCritical("Failed to open ModbusRTUSlave connection: " + dynamic_cast<ModbusRTUSlave*>(slave)->ErrorString());
            }
            break;

        // ASCII
        case 2:
            slave = new ModbusASCIISlave();
            dynamic_cast<ModbusASCIISlave*>(slave)->SetSerialPortInformation(target, port_baudRate);
            dynamic_cast<ModbusASCIISlave*>(slave)->SetInformation(unitID, &dataTable);
            if (!dynamic_cast<ModbusASCIISlave*>(slave)->OpenConnection())
            {
                qDebug() << ("Failed to open ModbusASCIISlave connection: " + dynamic_cast<ModbusASCIISlave*>(slave)->ErrorString());
                //BaseLogger::LogCritical("Failed to open ModbusASCIISlave connection: " + dynamic_cast<ModbusASCIISlave*>(slave)->ErrorString());
            }
            break;

        default:
            qDebug() << ("Trying to create new modbus instance of invalid protocol: " + QString::number(protocolType));
            //BaseLogger::LogCritical("Trying to create new modbus instance of invalid protocol: " + QString::number(protocolType));
            break;
        }

        if(slave != Q_NULLPTR)
        {
            modbusThread.start();
            slave->MoveToThread(&modbusThread);
            modbusThread.start();
        }
    }
    else
    {
        if(slave == Q_NULLPTR)
        {
            //BaseLogger::LogCritical("Trying to update null instance of ModbusSlave");
            qDebug() << ("Trying to update null instance of ModbusSlave");
            return;
        }

        switch (protocolType)
        {
        // TCP
        case 0:
            QMetaObject::invokeMethod(dynamic_cast<ModbusBase*>(dynamic_cast<ModbusTCPSlave*>(slave)), "SetUnitID",
                                       Qt::QueuedConnection,
                                       Q_ARG(unsigned char, unitID));

            QMetaObject::invokeMethod(dynamic_cast<ModbusTCPBase*>(dynamic_cast<ModbusTCPBase*>(slave)), "SetConnectionInformation",
                                       Qt::QueuedConnection,
                                       Q_ARG(QString, target), Q_ARG(unsigned short, port_baudRate));

            break;

        // RTU
        case 1:
            QMetaObject::invokeMethod(reinterpret_cast<ModbusBase*>(slave), "SetUnitID",
                                       Qt::QueuedConnection,
                                       Q_ARG(unsigned char, unitID));

            QMetaObject::invokeMethod(dynamic_cast<ModbusRTUSlave*>(slave), "SetSerialPortInformation",
                                       Qt::QueuedConnection,
                                       Q_ARG(QString, target), Q_ARG(unsigned short, port_baudRate));
            break;

        //ASCII
        case 2:
            QMetaObject::invokeMethod(reinterpret_cast<ModbusBase*>(slave), "SetUnitID",
                                       Qt::QueuedConnection,
                                       Q_ARG(unsigned char, unitID));

            QMetaObject::invokeMethod(dynamic_cast<ModbusASCIISlave*>(slave), "SetSerialPortInformation",
                                       Qt::QueuedConnection,
                                       Q_ARG(QString, target), Q_ARG(unsigned short, port_baudRate));
            break;

        default:
            //BaseLogger::LogCritical("Trying to update modbus instance of invalid protocol: " + QString::number(protocolType));
            qDebug() << ("Trying to update modbus instance of invalid protocol: " + QString::number(protocolType));
            break;
        }
    }
}

void ModbusHandler::CloseModbus()
{
    if(slave != Q_NULLPTR)
    {
        switch (currentProtocol)
        {
        // TCP
        case 0:
            dynamic_cast<ModbusTCPSlave*>(slave)->CloseConnection();
            dynamic_cast<ModbusTCPSlave*>(slave)->deleteLater();
            break;

        // RTU
        case 1:
            dynamic_cast<ModbusRTUSlave*>(slave)->CloseConnection();
            dynamic_cast<ModbusRTUSlave*>(slave)->deleteLater();
            break;

        // ASCII
        case 2:
            dynamic_cast<ModbusASCIISlave*>(slave)->CloseConnection();
            dynamic_cast<ModbusASCIISlave*>(slave)->deleteLater();
            break;

        //INVALID
        default:
            //BaseLogger::LogCritical("Trying to delete unkown modbus slave type");
            qDebug() << ("Trying to delete unkown modbus slave type");
            break;
        }
    }
}

void ModbusHandler::StartModbus()
{
    if(IsSlave())
    {
        //BaseLogger::Log("Slave");
        //BaseLogger::Log("Protocol: " + QString::number(currentProtocol));
        //BaseLogger::Log("Is tcp slave: " + BaseLogger::BoolToString(reinterpret_cast<ModbusTCPSlave*>(reinterpret_cast<ModbusBase*>(slave)) != Q_NULLPTR));
        qDebug() << ("Slave");
        qDebug() << ("Protocol: " + QString::number(currentProtocol));
        qDebug() << ("Is tcp slave: ") << (reinterpret_cast<ModbusTCPSlave*>(reinterpret_cast<ModbusBase*>(slave)) != Q_NULLPTR);
        static_cast<ModbusTCPSlave*>(slave)->OpenConnection();
    }
    else if(IsMaster())
    {
        //BaseLogger::Log("Master");
        qDebug() << ("Master");
        reinterpret_cast<ModbusBase*>(master)->OpenConnection();
    }
    else
    {
        //BaseLogger::LogCritical("Neither master nor slave");
        qDebug() << ("Neither master nor slave");
    }
}

void ModbusHandler::RegisterValueObjects(QList<BaseValueObject*> values)
{
    for (BaseValueObject* val : values)
    {
        connect(val, &BaseValueObject::ValueChanged, this, &ModbusHandler::OnValueChange);
        val->Mutex()->lock();
        dataTable.SetOutputRegisters(val->GetRegisterIndex(), val->GetRegisterValues(), val->GetNumRegisters());
        val->Mutex()->unlock();
        UpdateValueObjectRegisters(val);
    }
}

void ModbusHandler::OnValueChange()
{
    BaseValueObject* val = dynamic_cast<BaseValueObject*>(sender());
    UpdateValueObjectRegisters(val);
}

void ModbusHandler::UpdateValueObjectRegisters(BaseValueObject* val)
{
    val->Mutex()->lock();
    //qDebug() << "Setting modbus registers for " << val->getName() << " starting at " << val->GetRegisterIndex() << " for " << val->GetNumRegisters();


    switch(val->GetValueType())
    {
    case ValueType::FLOAT_VALUE:
        dataTable.SetOutputRegisters(val->GetRegisterIndex(), val->GetBytes() + 2, 1);
        dataTable.SetOutputRegisters(val->GetRegisterIndex() + 1, val->GetBytes(), 1);
        break;

    case ValueType::INT_VALUE:
        dataTable.SetOutputRegisters(val->GetRegisterIndex(), val->GetBytes() + 2, 1);
        dataTable.SetOutputRegisters(val->GetRegisterIndex() + 1, val->GetBytes(), 1);
        break;

    case ValueType::UCHAR_VALUE:
        dataTable.SetOutputRegisters(val->GetRegisterIndex(), val->GetBytes(), val->GetNumRegisters());
        break;

    case ValueType::STRING_VALUE:
        //qDebug() << "Reg pointer: " << val->GetRegisterValues();
        dataTable.SetOutputRegisters(val->GetRegisterIndex(), val->GetRegisterValues(), val->GetNumRegisters());
        //qDebug() << "Setting modbus registers for " << val->getName() << " with " << reinterpret_cast<char*>(val->GetBytes()) << " starting at " << val->GetRegisterIndex() << " for " << val->GetNumRegisters();
        break;
    }

    val->Mutex()->unlock();
}

void ModbusHandler::SetModbusCoil(int index)
{
    dataTable.SetOutputCoil(index, true);
}
