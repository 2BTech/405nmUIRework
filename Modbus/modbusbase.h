#ifndef MODBUSBASE_H
#define MODBUSBASE_H

// Inheritance, signals and slots
#include <QObject>

// Stores all of the modbus values
#include "modbusdatatable.h"
// Base class for handling modbus messages
#include "Modbus/modbusmessage.h"

#include "modbusdefs.h"

//https://fernhillsoftware.com/help/drivers/modbus/modbus-protocol.html#modbusPDU
// Read coils function
//      - Function code = 0x01
//  Request PDU - 5 bytes
//  - Funct code - 1 bytes
//  - Start addr - 2 bytes
//  - Length     - 2 bytes
//  Response PDU - 3 - 252 bytes - 2 + ceil(length + 7) / 8
//  - Funct code - 1 byte
//  - Length     - ceil(length + 7) / 8
//  - Data       - N bytes
//  Error Response
//  - Funct code - 0x81
//  - Excep code - 1 byte

//  Examples:
//  Read 12 coils starting at 33
// Coils 40 and 42 are set, all others are clear

//  RTU
//      Request - [ 02 01 00 20 00 0C 3D F6 ]
//      - 02 - Unit ID
//      - 01 - Funct code
//      - [ 00 20 ] - start address = 32
//      - [ 00 0C ] - Length = 12
//      - [ 3D F6 ] - checksum
//      Response - [ 02 01 80 1D FD ]
//      - 02 - Unit ID
//      - 01 - Funct code
//      - 02 - Num bytes
//      - [ 80 02 ] - Coil data
//      - [ 1D FD ] - checksum

//  ASCII
//      Request - [ 3A 30 32 30 31 30 30 32 30 30 30 30 43 44 31 0D 0A ]
//      - 3A - ':'
//      - Unit ID - [ 30 32 ] 0
//      - Message [ 30 31 30 30 32 30 30 30 30 43 ]
//      - LRC [ 44 31 ]
//      - End [ 0D 0A ]
//      Response - [ 3A 30 32 30 31 30 32 38 30 30 32 37 39 0D 0A ]
//      - 3A - ':'
//      - Unit ID - [ 30 32 ]
//      - Message - [ 30 31 30 32 38 30 30 32 ]
//      - LRC [ 44 31 ]
//      - End [ 1D FD ]

//  TCP
//      Request - [ 00 05 00 00 00 06 02 01 00 20 00 0C ]
//      - Transaction ID - [ 00 05 ]
//      - Protocol - [ 00 00 ]
//      - Length - [ 00 06 ]
//      - Unit ID - [ 02 ]
//      - Message - [ 01 00 20 00 0C ]
//      Response - [ 00 05 00 00 00 05 02 01 02 80 02 ]
//      - Transaction ID - [ 00 05 ]
//      - Protocol - [ 00 00 ]
//      - Length - [ 05 ]
//      - Unit ID - [ 02 ]
//      - Message - [ 01 02 80 02 ]



// Read discrete inputs function
//      - Function code = 0x02
//  Request PDU - 5 bytes
//  - Funct code - 1 bytes
//  - Start addr - 2 bytes
//  - Length     - 2 bytes
//  Response PDU - 3 - 252 bytes - 2 + (length + 7) / 8
//  - Funct code - 1 byte
//  - Length     - (length + 7) / 8
//  - Data       - N bytes
//  Error Response
//  - Funct code - 0x81
//  - Excep code - 1 byte

//  Examples:
//  Read 16 inputs starting at 1
//  Inputs 1 and 3 are set, all others are clear

//  RTU
//      Request - [ 01 02 01 F4 00 10 39 DC ]
//      - 01 - Unit ID
//      - 02 - Funct code
//      - [ 01 F4 ] - start address = 500
//      - [ 00 10 ] - Length = 16
//      - [ 39 DC ] - checksum
//      Response - [ 01 02 02 05 00 BA E8 ]
//      - 01 - Unit ID
//      - 02 - Funct code
//      - 02 - Num bytes
//      - [ 05 00 ] - Coil data
//      - [ BA E8 ] - checksum

//  ASCII
//      Request - [ 3A 30 31 30 32 30 31 46 34 30 30 32 30 45 38 0D 0A ]
//      - 3A - ':'
//      - Unit ID - [ 30 31 ] - "01"
//      - Funct code - [ 30 32 ] - "02"
//      - Message [ 30 31 30 30 32 30 30 30 30 43 ]
//      - LRC [ 44 31 ]
//      - End [ 0D 0A ]
//      Response - [ 3A 30 32 30 31 30 32 38 30 30 32 37 39 0D 0A ]
//      - 3A - ':'
//      - Unit ID - [ 30 32 ]
//      - Message - [ 30 31 30 32 38 30 30 32 ]
//      - LRC [ 44 31 ]
//      - End [ 1D FD ]

//  TCP
//      Request - [ 00 05 00 00 00 06 02 01 00 20 00 0C ]
//      - Transaction ID - [ 00 05 ]
//      - Protocol - [ 00 00 ]
//      - Length - [ 00 06 ]
//      - Unit ID - [ 02 ]
//      - Message - [ 01 00 20 00 0C ]
//      Response - [ 00 05 00 00 00 05 02 01 02 80 02 ]
//      - Transaction ID - [ 00 05 ]
//      - Protocol - [ 00 00 ]
//      - Length - [ 05 ]
//      - Unit ID - [ 02 ]
//      - Message - [ 01 02 80 02 ]

// Describes the encountered error
enum ModbusError
{
    NO_ERROR,
    CONNECTION_ERROR
};

// Describes the state of the class
enum ModbusState
{
    UNCONNECTED_STATE,
    CONNECTED_STATE,
    ERROR_STATE
};

// The modbus base class is inherited by all Modbus master and slave
// classes. This defines common functions and variables used by all
// of the child classes
class ModbusBase : public QObject
{
    Q_OBJECT

public:
    ModbusBase();
    ModbusBase(ModbusDatatable* datatable);

    // This function is intended to be overridden and used as an entry point for initializing the class. This function saves the values for the class.
    virtual void SetInformation(unsigned char unitID, ModbusDatatable * datatable);

    // Sets the data table for the modbus object
    virtual void SetDatatable(ModbusDatatable* datatable);

    // Abstract function. This where the connection should be established.
    virtual bool OpenConnection() = 0;
    // Abstract function. This where the connection should be closed.
    virtual void CloseConnection() = 0;

    // This function handles moving all chld objects to the thread before also moving itself.
    virtual void MoveToThread(QThread* thread) = 0;

    // Sets unitID. The modbus ID for the device
    Q_INVOKABLE void SetUnitID(unsigned char unitID);
    // Gets unitID
    unsigned char UnitID();

    // Returns true if there is an established connection.
    virtual bool IsConnected() = 0;
    // Gets the error string
    QString ErrorString();

signals:
    // This signal is triggered whenever an error occurs.
    void ErrorOccured(ModbusError error);
    // This signal is triggered whenever the state of the object changes.
    void StateChanged(ModbusState state);

    // This signal is triggered when the object establishes a connection.
    void Connected();
    // This signal is triggered when the object loses a connection.
    void Disconnected();

protected slots:
    // This is an abstract function. It will be used to read in data from a connection.
    virtual void ReadyRead() = 0;

protected:
    // Is there an established connection
    bool isConnected = false;
    // String used to describe an encountered error
    QString errorString = "No Error";
    // Standard errors encounted by the modbus protocol
    ModbusError error = ModbusError::NO_ERROR;
    // Enum that describes the state of the class
    ModbusState state = ModbusState::UNCONNECTED_STATE;
    // Reference to the system data table
    ModbusDatatable* dataTable = Q_NULLPTR;
    // Modbus ID of the device
    unsigned char unitID = 0;
};

#endif // MODBUSBASE_H
