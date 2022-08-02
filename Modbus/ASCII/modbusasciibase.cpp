#include "modbusasciibase.h"

ModbusASCIIBase::ModbusASCIIBase()
    : ModbusBase(),
      currentMessage()
{
    connect(&serialPort, &QSerialPort::readyRead, this, &ModbusASCIIBase::ReadyRead);
}


void ModbusASCIIBase::SetSerialPortInformation(QString portname, int baudrate)
{
    if(serialPort.isOpen())
    {
        //BaseLogger::LogCritical("Trying to set serial port information while serial port is open. " + QString(__PRETTY_FUNCTION__));
        qDebug() << ("Trying to set serial port information while serial port is open. " + QString(__PRETTY_FUNCTION__));
    }
    else
    {
        serialPort.setPortName(portname);
        serialPort.setBaudRate(baudrate);
    }
}

QString ModbusASCIIBase::PortName()
{
    return serialPort.portName();
}

int ModbusASCIIBase::BaudRate()
{
    return serialPort.baudRate();
}

bool ModbusASCIIBase::OpenConnection()
{
    if(!serialPort.isOpen())
    {
        if(serialPort.open(QIODevice::ReadWrite))
        {
#if !TEST_BUILD
            //BaseLogger::Log("Opened ModbusASCII base at " + serialPort.portName() + " : " + QString::number(serialPort.baudRate()));
            qDebug() << ("Opened ModbusASCII base at " + serialPort.portName() + " : " + QString::number(serialPort.baudRate()));
#endif // !TEST_BUILD
        }
        else
        {
            errorString = "ModbusMasterASCII failed to connect serial port at " + serialPort.portName() + " : " + QString::number(serialPort.baudRate()) + ". " + serialPort.errorString();
#if !TEST_BUILD
            //BaseLogger::LogCritical(errorString);
            qDebug() << errorString;
#endif // !TEST_BUILD
            emit ErrorOccured(CONNECTION_ERROR);
        }
        return serialPort.isOpen();
    }
    else
    {
        return true;
    }
}

void ModbusASCIIBase::CloseConnection()
{
    serialPort.close();
}

bool ModbusASCIIBase::IsConnected()
{
    return serialPort.isOpen();
}

void ModbusASCIIBase::ReadyRead()
{
    //BaseLogger::Log("In ModbusASCIIBase::ReadyRead");
    char curByte;
    while (serialPort.bytesAvailable())
    {
        serialPort.read(&curByte, 1);
        currentMessage.AddByte(curByte);

        //BaseLogger::Log("ASCII message'" + QString().append(reinterpret_cast<char*>(currentMessage.Buffer())) + "' : " + BaseLogger::BoolToString(currentMessage.ParseMessage()) + " Len: " + QString::number(currentMessage.Length()));

        if(currentMessage.ParseMessage() == 1)
        {
            HandleMessage(&currentMessage);
            currentMessage.Reset();
        }
        else if (currentMessage.ParseMessage() == 2)
        {
            currentMessage.Reset();
            //BaseLogger::LogCritical("Received invalid ascii message. Clearing");
        }
    }
}


void ModbusASCIIBase::MoveToThread(QThread * thread)
{
    serialPort.moveToThread(thread);
    this->moveToThread(thread);
}
