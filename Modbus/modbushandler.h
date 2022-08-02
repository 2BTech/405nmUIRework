#ifndef MODBUSHANDLER_H
#define MODBUSHANDLER_H

// Inheritance, signals and slots
#include <QObject>
// Running the modbus classes in a non blocking way
#include <QThread>

// Holds data in an accessable way for the modbus classes
#include "modbusdatatable.h"

#include "modbusmaster.h"
#include "modbusslave.h"

#include "RTU/modbusrtumaster.h"
#include "RTU/modbusrtuslave.h"

#include "TCP/modbustcpmaster.h"
#include "TCP/modbustcpslave.h"

#include "ASCII/modbusasciimaster.h"
#include "ASCII/modbusasciislave.h"

#include "ValueHandlers/ValueObjects/basevalueobject.h"

// The modbus handler is used to handle the active modbus connection. It acts an interface for working with the different Modbus protocols.
class ModbusHandler : public QObject
{
    Q_OBJECT
public:
    static ModbusHandler * GetInstance();

//Shared functions
public:
    // Gets a reference to the modbus data table
    ModbusDatatable* DataTable();

    // Gets currentProtocol. Describes which protocol is active.
    int ProtocolID();
    // Gets the string representation for currentProtocol.
    QString ProtocolName();

    // Is the device connected as a modbus master.
    bool IsMaster();
    // Is the device connected as a modbus slave.
    bool IsSlave();
    // Returns true if either IsMaster or IsSlave is true.
    bool IsConnected();

    // Triggers the current modbus master or slave to open a connection.
    void StartModbus();

    void RegisterValueObjects(QList<BaseValueObject*> values);

public slots:
    // This function is used to open or update a modbus connection. The protocol that is used depends on protocolType.
    // If the current connection mathches the protocol, the instance is updated. Else, the current instance is
    // deleted and replaced by the new protocol.
    void UpdateModbusSettings(int protocolType, QString target, int port_baudRate, unsigned char unitID);

    void SetModbusCoil(int index);

signals:
    // This signal is emmited when the current modbus class
    void TCPSlaveConnection(QString peerIP);
    // This signal is triggered when the current modbus class establishes a new connection.
    bool Connected();
    // This signal is triggered when the current modbus class establishes a new connection and is a master.
    bool ConnectedAsMaster();
    // This signal is triggered when the current modbus class establishes a new connection and is a slave.
    bool ConnectedAsSlave();
    // This signal is triggered when the current modbus class closes a connection.
    bool Disconnected();

    // This signal is triggered when a coil in the datatable is changed.
    bool UpdatedOutputCoil(int coilIndex);
    // This signal is triggered when a input coil is changed.
    bool UpdatedInputCoil(int coilIndex, bool value);
    // This signal is triggreed when an output register is changed.
    bool UpdatedOutputRegister(int registerIndex);
    // This signal is triggered when an input register is changed.
    bool UpdatedInputRegister(int registerIndex, unsigned short value);

private slots:
    void OnValueChange();

private:
    ModbusHandler();

    // Singleton instance
    static ModbusHandler* instance;

    // Modbus datatable
    ModbusDatatable dataTable;

    // Active modbus master instance
    ModbusMaster* master = Q_NULLPTR;
    // Active modbus slave instance
    ModbusSlave* slave = Q_NULLPTR;
    // Describes the current protocol
    int currentProtocol = -1;

    // Thread to run the modbus classes in
    QThread modbusThread;

    void CloseModbus();
};

#endif // MODBUSHANDLER_H
