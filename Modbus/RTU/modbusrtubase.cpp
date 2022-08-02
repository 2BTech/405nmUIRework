#include "modbusrtubase.h"

ModbusRTUBase::ModbusRTUBase() : ModbusBase(), currentMessage()
{
    connect(&serialPort, &QSerialPort::readyRead, this, &ModbusRTUBase::ReadyRead);
}

void ModbusRTUBase::SetSerialPortInformation(QString portname, int baudrate)
{
    if(serialPort.isOpen())
    {
        //BaseLogger::LogCritical("Trying to set serial port information while serial port is open");
        qDebug() << ("Trying to set serial port information while serial port is open");
    }
    else
    {
        serialPort.setPortName(portname);
        serialPort.setBaudRate(baudrate);
//        BaseLogger::Log("Opened RTU serial port.\nPort: " + (serialPort.portName()) +
//                                              "\nBaudrate: " + QString::number(serialPort.baudRate()) +
//                                              "\nParity: " + QString::number(serialPort.parity()) +
//                                              "\nStopBits: " + QString::number(serialPort.stopBits()));
        qDebug() << ("Opened RTU serial port.\nPort: " + (serialPort.portName()) +
                     "\nBaudrate: " + QString::number(serialPort.baudRate()) +
                     "\nParity: " + QString::number(serialPort.parity()) +
                     "\nStopBits: " + QString::number(serialPort.stopBits()));
    }
}

QString ModbusRTUBase::PortName()
{
    return serialPort.portName();
}

int ModbusRTUBase::BaudRate()
{
    return serialPort.baudRate();
}

bool ModbusRTUBase::OpenConnection()
{
    if(serialPort.open(QIODevice::ReadWrite))
    {
#if !TEST_BUILD
        //BaseLogger::Log("Opened ModbusRTU master at " + serialPort.portName() + " : " + QString::number(serialPort.baudRate()));
        qDebug() << ("Opened ModbusRTU master at " + serialPort.portName() + " : " + QString::number(serialPort.baudRate()));
#endif
    }
    else
    {
        errorString = "ModbusMasterRTU failed to connect serial port at '" + serialPort.portName() + "' : " + QString::number(serialPort.baudRate()) + ". " + serialPort.errorString();
        const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        //BaseLogger::Log("Num avaialble serial ports: " + QString::number(ports.count()));
        qDebug() << ("Num avaialble serial ports: " + QString::number(ports.count()));
        for (const QSerialPortInfo &port : ports)
        {
            //BaseLogger::Log("Port: " + port.portName());
            qDebug() << ("Port: " + port.portName());
        }
#if !TEST_BUILD
        //BaseLogger::LogCritical(errorString);
        qDebug() << "ERROR: " << errorString;
#endif
        emit ErrorOccured(CONNECTION_ERROR);
    }
    return serialPort.isOpen();
}

void ModbusRTUBase::CloseConnection()
{
    serialPort.close();
}

bool ModbusRTUBase::IsConnected()
{
    return serialPort.isOpen();
}

void ModbusRTUBase::ReadyRead()
{
    uchar status;
    //BaseLogger::Log("In ModbusRTUBase::ReadyRead()");
    qDebug() << ("In ModbusRTUBase::ReadyRead()");
    char curByte;
    while (serialPort.bytesAvailable())
    {
        serialPort.read(&curByte, 1);
        currentMessage.AddByte(curByte);

        status = currentMessage.ParseMessage();

        if(status == 1)
        {
            //BaseLogger::Log("ModbusRTUBase: parsing message");
            qDebug() << ("ModbusRTUBase: parsing message");
            HandleMessage(&currentMessage);
            currentMessage.Reset();
        }
        else if (status == 2)
        {
            //BaseLogger::LogCritical("Modbus RTU Base received invalid message. Clearing.");
            qDebug() << ("Modbus RTU Base received invalid message. Clearing.");
            currentMessage.Reset();
        }
    }

}

void ModbusRTUBase::MoveToThread(QThread* thread)
{
    serialPort.moveToThread(thread);
    this->moveToThread(thread);
}
