#ifndef MODBUSDATATABLE_H
#define MODBUSDATATABLE_H

// Inheritance, signals and slots
#include <QObject>
#include <QtMath>
#include <QDebug>

#define MAX_COILS 65535
#define MAX_DISCRETE_INPUTS 65535
#define MAX_INPUT_REGISTERS 65535
#define MAX_HOLDING_REGISTERS 65535

union byteArray{
    char bytes[2];
    short word;
};

// This class holds data in an easily accessable way for
// the modbus classes

// Data types
// Coil
//      - Bit value
//      - Read & write
// Discrete Input
//      - Bit value
//      - Read only
// Input Register
//      - 16-bit (short)
//      - Read only
// Holding Register
//      - 16-bit (short)
//      - Read & Write

// Message Structure
// RTU - 8 bit binary
//      - |Unit Address(1 byte)|Message(N bytes)|CRC(2 bytes)|
//      - CRC = Cyclic Redundancy Check
//          - Unit Address and Message fields
// ASCII - Hexadecimal (0-9,A-F)
//      - |START(ASCII 58 ':')|Unit Address(2 chars)|Message(N characters)|LRC(2 characters)|END(ASCII 13, ASCII 10)|
//      - LRC = Longitudinal Redundancy Check
//          - Address and Message fields
//      - Max message length = 513 characters
// TCP -
//      - |Transaction ID(2 bytes)|Protocol(2 bytes)|Length(2 bytes)|Unit Address(1 byte)|Message(N bytes)|
//      - Protocol = 0. Signifies modbus protocol
//      - Length = # of following bytes
//      - Message = PDU, max length = 260

// The modbus datatable is in charge of tracking all of the different data related values for the modbus classes. It has 4 internal data array: input
// coils, output coils, input registers, and output registers. Coils are a form of data made up of 1 bit. They are used to signal different values such
// as if something is on or if a state is true. Registers are 16 bit values. They are used to store data. Input values are read-only while output values are read-write.
//
// When an object in the app holds a reference to a modbus value, it is holding a reference to one of the values in the internal data array.
class ModbusDatatable : public QObject
{
    Q_OBJECT

public:
    ModbusDatatable();
    ~ModbusDatatable();

    ModbusDatatable(int numberOfCoils, int numberOfDiscreteInputs,
                    int numberOfInputRegisters, int numberOfHoldingRegisters);

    // Resizes the output coils buffer to just hold quantity output coils. Uses malloc and realloc to allocate the array.
    void ResizeOutputCoils(int quantity);
    // Gets the number of output coils.
    int NumberOfOutputCoils();
    // Gets the number of bytes used to hold output coils.
    int NumberOfOutputCoilBytes();
    // Sets the coil at the specified index to value.
    void SetOutputCoil(int address, bool value);
    // Sets the value for a collection of coils.
    void SetOutputCoils(int address, int numCoils, unsigned char* bytes);
    // Gets the value of the specified coil.
    bool GetOutputCoil(int address);
    // This function gets a collection of bytes and loads them into the given buffer. This is mainly used when writing modbus values.
    void GetOutputCoils_charArr(int startAddress, int quantity, unsigned char* buffer, int numBytes);

    // Resizes the input coils buffer to just hold the desired quantity. Uses malloc and realloc to allocate the array.
    void ResizeInputCoils(int quantity);
    // Gets the number of input coils.
    int NumberOfInputCoils();
    // Sets the input coil to the specified value.
    void SetInputCoil(int address, bool value);
    // Sets a collection of input coils.
    void SetInputCoils(int address, int numCoils, unsigned char* bytes);
    // Gets the value of the coil.
    bool GetInputCoil(int address);
    // Gets a range of coils as a list.
    QList<bool> GetInputCoils(int startAddress, int quantity);
    // This function gets a range of coils and loads them into the buffer.
    void GetInputCoils_charArr(int startAddress, int quantity, unsigned char* buffer, int numBytes);

    // Resizes the input registers array to just hold the desired quantity. Uses malloc and realloc to allocate the array.
    void ResizeInputRegisters(int quantity);
    // Gets the number of input registers in the data table.
    int NumberOfInputRegisters();
    // Sets the input register at the specified index to value.
    void SetInputRegister(int address, unsigned short value);
    // Sets a collection of input registers based on bytes.
    void SetInputRegisters(int address, int numRegister, unsigned char * bytes);
    // Gets the value of the specified input register.
    unsigned short GetInputRegister(int address);
    // Gets a collection of input registers and returns them as a list.
    QList<unsigned short> GetInputRegisters(int startAddress, int quantity);
    // Gets a collection of input registers and load them into buffer. The buffer is assumed to be large enough to hold the register.
    void GetInputRegisters_charArr(int startAddress, int quantity, unsigned char * buffer);

    // Gets the number of output registers.
    void ResizeOutputRegisters(int quantity);
    // Gets the number of output registers in the data table.
    int NumberOfOutputRegisters();
    // Sets the register at address with value.
    void SetOutputRegister(int address, unsigned short value);
    // Sets a collection of output registers starting at address based off of the values in buffer. The length of buffer is assumed to be 2*numRegisters.
    void SetOutputRegisters(int address, unsigned char* buffer, int numRegisters);
    void SetOutputRegisters(int address, short* buffer, int numRegisters);
    // Gets the value of the register at address.
    unsigned short GetOutputRegister(int address);
    // Gets quanity of registers starting at startAddress. Returns the registers in a list.
    QList<short> GetOutputRegisters(int startAddress, int quantity);
    // Gets quanity registers starting at startAddress. The registers are loaded into buffer. Buffer is assumed to be large enough to hold all the registers.
    void GetOutputRegisters_charArr(int startAddress, int quantity, unsigned char * buffer);

    // Converts a list of chars that represent coils into a list of bools.
    static QList<bool> ConvertList(QList<char> original);
    // Packs a list of coil values into a list of char values.
    static QList<char> ConvertList(QList<bool> original);
    // Returns true if bit in val is set.
    static bool CheckBit(int val, int bit);
    // Returns val after setting the bit.
    static int SetBit(int val, int bit);
    // Returns val after clearing the bit.
    static int ClearBit(int val, int bit);

    // Copies quanity entries from source to dest.
    static void CopyBools(bool* source, bool* dest, int quantity);
    // Copies quanity entries from source to dest.
    static void CopyShorts(unsigned short* source, unsigned short* dest, int quantity);

    // Gets a reference to the internal output coils array
    unsigned char* GetOutputCoilsReference();
    // Gets a reference to the internal input coils array
    unsigned char* GetInputCoilsReference();
    // Gets a reference to the internal input registers array
    unsigned short* GetInputRegistersReference();
    // Gets a reference to the internal output registers array
    unsigned short* GetOutputRegistersReference();

public slots:

signals:
    // This signal is triggered when the value of an output coil is changed by this class.
    void OutputCoilChanged(int index);
    // This signal is triggered when the value of an output register is changed by this class.
    void OutputRegisterChanged(int index);

    // This signals is emitted when the modbus data table alters the output coils array. This is intedned to be used by objects that hold references to output coils to update the reference.
    void AlteredOutputCoilsArray(uchar* updatedPointer, int numBytes);
    // This signal is emitted when the data table alters the input coils array.
    void AlteredInputCoils();
    // This signal is emitted when the data table alters the input registers array.
    void AlteredInputRegisters();
    // This signal is emitted when the data table alters the output registers array.
    void AlteredOutputRegisters();

private slots:

private:
    // Number of output coils
    int numberOfOutputCoils = 0;
    // Number of input coils
    int numberOfInputCoils = 0;
    // Number of bytes used to hold the output coils
    int numberOfOutputCoilBytes = 0;
    // Number of bytes used tp hold the input coils
    int numberOfInputCoilBytes = 0;

    // Number of input registers
    int numberOfInputRegisters = 0;
    // Number of output registers
    int numberOfOutputRegisters = 0;

    // Output coils array
    unsigned char* outputCoils = Q_NULLPTR;
    // Input coils array
    unsigned char* inputCoils = Q_NULLPTR;
    // Input registers array
    unsigned short* inputRegisters = Q_NULLPTR;
    // Output registers array
    unsigned short* outputRegisters = Q_NULLPTR;
};

#endif // MODBUSDATATABLE_H
