#include "modbusdatatable.h"

ModbusDatatable::ModbusDatatable()
{

}

ModbusDatatable::ModbusDatatable(int numberOfCoils, int numberOfDiscreteInputs,
                                 int numberOfInputRegisters, int numberOfHoldingRegisters)
{
    ResizeOutputCoils(numberOfCoils);
    ResizeInputCoils(numberOfDiscreteInputs);
    ResizeInputRegisters(numberOfInputRegisters);
    ResizeOutputRegisters(numberOfHoldingRegisters);
}

ModbusDatatable::~ModbusDatatable()
{
    if(outputCoils != Q_NULLPTR)
    {
        free(outputCoils);
    }
    if(inputCoils != Q_NULLPTR)
    {
        free(inputCoils);
    }
    if(inputRegisters != Q_NULLPTR)
    {
        free(inputRegisters);
    }
    if(outputRegisters != Q_NULLPTR)
    {
        free(outputRegisters);
    }
}

unsigned char * ModbusDatatable::GetOutputCoilsReference()
{
    return outputCoils;
}

unsigned char * ModbusDatatable::GetInputCoilsReference()
{
    return inputCoils;
}

unsigned short * ModbusDatatable::GetInputRegistersReference()
{
    return inputRegisters;
}

unsigned short * ModbusDatatable::GetOutputRegistersReference()
{
    return outputRegisters;
}

void ModbusDatatable::ResizeOutputCoils(int quantity)
{
    // Make sure the passed quantity is valid
    if(quantity >= 0 && quantity <= MAX_COILS)
    {
        // If the quantity matches the current number, then no need to resize
        if(quantity == numberOfOutputCoils)
        {
            return;
        }

        int numCBytes = (quantity + 7) / 8;

        // Use malloc/realloc instead of 'new' because easier to resize
        if(outputCoils == Q_NULLPTR)
        {
            outputCoils = reinterpret_cast<uchar*>(malloc(numCBytes));
        }
        else
        {
            outputCoils = reinterpret_cast<uchar*>(realloc(outputCoils, numCBytes));
        }

        if(quantity > numberOfOutputCoils)
        {
            for(; numberOfOutputCoils < quantity; numberOfOutputCoils++)
            {
                outputCoils[qFloor(numberOfOutputCoils/8)] &= ~(1UL << (numberOfOutputCoils % 8));
            }
        }

        // Save the number of coil bytes
        numberOfOutputCoilBytes = numCBytes;
        // Save the number of coils
        numberOfOutputCoils = quantity;

        emit AlteredOutputCoilsArray(outputCoils, numCBytes);
    }
    else
    {
        qDebug() << ("Error: Invalid quantity in ResizeCoils");
        //BaseLogger::LogCritical("Error: Invalid quantity in ResizeCoils");
    }
}

int ModbusDatatable::NumberOfOutputCoils()
{
    return numberOfOutputCoils;
}

int ModbusDatatable::NumberOfOutputCoilBytes()
{
    return numberOfOutputCoilBytes;
}

void ModbusDatatable::SetOutputCoil(int address, bool value)
{
    if(address >= 0 && address < numberOfOutputCoils)
    {
        if(value)
        {
            outputCoils[qFloor(address/8)] |= 1UL << (address % 8);
        }
        else
        {
            outputCoils[qFloor(address/8)] &= ~(1UL << (address % 8));
        }
    }
    else
    {
        //BaseLogger::LogCritical("address < 0 || address >= numberOfOutputCoils");
        qDebug() << ("address < 0 || address >= numberOfOutputCoils");
        //throw new std::out_of_range("address < 0 || address >= numberOfOutputCoils");
        throw new std::exception();
    }
}

void ModbusDatatable::SetOutputCoils(int address, int numCoils, unsigned char *bytes)
{
    if(address < 0 || (address + numCoils > numberOfOutputCoils))
    {
        //BaseLogger::LogCritical("address < 0 || (address + numCoils > numberOfOutputCoils)");
        qDebug() << ("address < 0 || (address + numCoils > numberOfOutputCoils)");
        //throw std::out_of_range("address < 0 || (address + numCoils > numberOfOutputCoils)");
        throw new std::exception();
    }

//    BaseLogger::Log("Data byte: " + BaseLogger::CollectionToString(bytes, (numCoils + 7)/8));
    numCoils += address;
    for(int i = 0; i < numCoils; i++)
    {
        // Check if the coil is set in the bytes array
        if((bytes[i/8] & (1UL << (i % 8))) != 0)
        {
            qDebug() << ("Setting output coil " + QString::number(address));
            //BaseLogger::Log("Setting output coil " + QString::number(address));
            // Using the address, set the bit in the output coils array
            outputCoils[address/8] |= (1UL << (address % 8));
        }
        else
        {
            qDebug() << ("Clearing output coil " + QString::number(address));
            //BaseLogger::Log("Clearing output coil " + QString::number(address));
            // Using the address, clear the bit in the output array
            outputCoils[address/8] &= ~(1UL << (address%8));
        }

        address++;
    }
}

bool ModbusDatatable::GetOutputCoil(int address)
{
    if(address >= 0 && address < numberOfOutputCoils)
    {
        return (outputCoils[address/8] & (1 << address % 8)) != 0;
    }
    else
    {
        qDebug() << ("address < 0 || address >= numberOfOutputCoils");
        //BaseLogger::LogCritical("address < 0 || address >= numberOfOutputCoils");
        //throw new std::out_of_range("address < 0 || address >= numberOfOutputCoils");
        throw new std::exception();
    }
}

void ModbusDatatable::GetOutputCoils_charArr(int address, int quantity, unsigned char * buffer, int numBytes)
{
    if(address < 0 || address + quantity > numberOfOutputCoils)
    {
        qDebug() << ("address < 0 || address + quantity >= numberOfOutputCoils");
        //BaseLogger::LogCritical("address < 0 || address + quantity >= numberOfOutputCoils");
        //throw std::out_of_range("address < 0 || address + quantity >= numberOfOutputCoils");
        throw new std::exception();
    }
    else
    {
        // Initialize buffer to 0
        memset(buffer, 0, numBytes);

        quantity += address;
        int index = 0, counter = 0;
        for(; address < quantity; address++)
        {
            buffer[index] |= ((outputCoils[address / 8] >> (address % 8)) & 1) << (counter++);
            if(counter == 8)
            {
                index++;
                counter = 0;
            }
        }
        if(counter != 0)
        {
            index++;
        }
    }
}

void ModbusDatatable::ResizeInputCoils(int quantity)
{
    // Make sure the passed quantity is valid
    if(quantity >= 0 && quantity <= MAX_DISCRETE_INPUTS)
    {
        // If the quantity matches the current number, then no need to resize
        if(quantity == numberOfInputCoils)
        {
            return;
        }

        int numCBytes = (quantity + 7) / 8;

        // Use malloc/realloc instead of 'new' because easier to resize
        if(inputCoils == Q_NULLPTR)
        {
            inputCoils = reinterpret_cast<uchar*>(malloc(numCBytes));
        }
        else
        {
            inputCoils = reinterpret_cast<uchar*>(realloc(inputCoils, numCBytes));
        }

        if(quantity > numberOfInputCoils)
        {
            for(; numberOfInputCoils < quantity; numberOfInputCoils++)
            {
                inputCoils[qFloor(numberOfInputCoils/8)] &= ~(1UL << (numberOfInputCoils % 8));
            }
        }

        // Save the number of coil bytes
        numberOfInputCoilBytes = numCBytes;
        // Save the number of coils
        numberOfInputCoils = quantity;
    }
    else
    {
        //BaseLogger::LogCritical("Error: Failed to resize Discrete Inputs");
        qDebug() << ("Error: Failed to resize Discrete Inputs");
    }
}

int ModbusDatatable::NumberOfInputCoils()
{
    return numberOfInputCoils;
}

void ModbusDatatable::SetInputCoil(int address, bool value)
{
    if(address >= 0 && address < numberOfInputCoils)
    {
        if(value)
        {
            inputCoils[qFloor(address/8)] |= 1UL << (address % 8);
        }
        else
        {
            inputCoils[qFloor(address/8)] &= ~(1UL << (address % 8));
        }
    }
    else
    {
        //throw std::out_of_range("address < 0 || address >= numberOfInputCoils");
        throw new std::exception();
    }
}

void ModbusDatatable::SetInputCoils(int address, int numCoils, unsigned char *bytes)
{
    if(address < 0 || (address + numCoils > numberOfInputCoils))
    {
        //BaseLogger::LogCritical("address < 0 || (address + numCoils > numberOfInputCoils)");
        qDebug() << ("address < 0 || (address + numCoils > numberOfInputCoils)");
        //throw std::out_of_range("address < 0 || (address + numCoils > numberOfInputCoils)");
        throw new std::exception();
    }

    numCoils += address;
    for(int i = 0; i < numCoils; i++)
    {
        // Check if the coil is set in the bytes array
        if((bytes[i/8] & (1UL << (i % 8))) != 0)
        {
            // Using the address, set the bit in the output coils array
            inputCoils[address/8] |= (1UL << (address % 8));
        }
        else
        {
            // Using the address, clear the bit in the output array
            inputCoils[address/8] &= ~(1UL << (address%8));
        }

        address++;
    }
}

bool ModbusDatatable::GetInputCoil(int address)
{
    if(address >= 0 && address < numberOfInputCoils)
    {
        return (inputCoils[address/8] & (1 << address % 8)) != 0;
    }
    else
    {
        //BaseLogger::LogCritical("Error: Index out of bounds in " + QString(__PRETTY_FUNCTION__));
        //BaseLogger::LogCritical("Index: " + QString::number(address) + " Total Coils: " + QString::number(NumberOfOutputCoils()));
        qDebug() << ("Error: Index out of bounds in " + QString(__PRETTY_FUNCTION__));
        qDebug() << ("Index: " + QString::number(address) + " Total Coils: " + QString::number(NumberOfOutputCoils()));
        return false;
    }
}

QList<bool> ModbusDatatable::GetInputCoils(int startAddress, int quantity)
{
    if(startAddress >= 0 && startAddress + quantity <= numberOfInputCoils)
    {
        QList<bool> values;
        for(int i = startAddress; i < startAddress + quantity; i++)
        {
            values.append(inputCoils[i]);
        }
        return values;
    }
    else
    {
        //BaseLogger::LogCritical("Error: Index out of bounds in GetDiscreteInputs");
        qDebug() << ("Error: Index out of bounds in GetDiscreteInputs");
        return QList<bool>();
    }
}

void ModbusDatatable::GetInputCoils_charArr(int address, int quantity, unsigned char * buffer, int numBytes)
{
    if(address < 0 || address + quantity > numberOfInputCoils)
    {
        //BaseLogger::LogCritical("address < 0 || address + quantity > numberOfInputCoils");
        qDebug() << ("address < 0 || address + quantity > numberOfInputCoils");
        //throw new std::out_of_range("address < 0 || address + quantity >= numberOfInputCoils");
        throw new std::exception();
    }
    else
    {
        memset(buffer, 0, numBytes);

        quantity += address;
        int index = 0, counter = 0;
        for(; address < quantity; address++)
        {
            buffer[index] |= ((inputCoils[address / 8] >> (address % 8)) & 1) << (counter++);
            if(counter == 8)
            {
                index++;
                counter = 0;
            }
        }
        if(counter != 0)
        {
            index++;
        }
    }
}

void ModbusDatatable::ResizeInputRegisters(int quantity)
{
    // Make sure the passed quantity is valid
    if(quantity >= 0 && quantity <= MAX_HOLDING_REGISTERS)
    {
        // If the quantity matches the current number, then no need to resize
        if(quantity == numberOfInputRegisters)
        {
            return;
        }

        int numBytes = (quantity) * 2;
        if(inputRegisters == Q_NULLPTR)
        {
            inputRegisters = reinterpret_cast<ushort*>(malloc(numBytes));
        }
        else
        {
            inputRegisters = reinterpret_cast<ushort*>(realloc(inputRegisters, numBytes));
        }

        // If adding registers, give a default value of 0
        if(quantity > numberOfInputRegisters)
        {
            memset(inputRegisters + (numberOfInputRegisters), 0, (quantity - numberOfInputRegisters) * 2);
        }

        //Update the size value
        numberOfInputRegisters = quantity;
    }
    else
    {
        qDebug() << ("Error: Unable to resize Input Registers");
        //BaseLogger::LogCritical("Error: Unable to resize Input Registers");
    }
}

int ModbusDatatable::NumberOfInputRegisters()
{
    return numberOfInputRegisters;
}

void ModbusDatatable::SetInputRegister(int address, unsigned short value)
{
    if(address >= 0 && address <= numberOfInputRegisters)
    {
        inputRegisters[address] = value;
    }
    else
    {
        qDebug() << ("Error: Unable to set Input Register");
        //BaseLogger::LogCritical("Error: Unable to set Input Register");
    }
}

void ModbusDatatable::SetInputRegisters(int address, int numRegisters, unsigned char * buffer)
{
    if(address >= 0 && (address + numRegisters < numberOfInputRegisters))
    {
        while(numRegisters)
        {
            inputRegisters[address] = ((buffer[0] << 8) | buffer[1]);
            address++;
            buffer += 2;
            numRegisters--;
        }
    }
    else
    {
        qDebug() << ("Error: Index out of bounds in SeInputRegisters");
        //BaseLogger::LogCritical("Error: Index out of bounds in SeInputRegisters");
    }
}

unsigned short ModbusDatatable::GetInputRegister(int address)
{
    if(address >= 0 && address < numberOfInputRegisters)
    {
        return inputRegisters[address];
    }
    else
    {
        qDebug() << ("address < 0 || address >= numberOfInputRegisters");
        //BaseLogger::LogCritical("address < 0 || address >= numberOfInputRegisters");
        //throw new std::out_of_range("address < 0 || address >= numberOfInputRegisters");
        throw new std::exception();
    }
}

QList<unsigned short> ModbusDatatable::GetInputRegisters(int startAddress, int quantity)
{
    if(startAddress >= 0 && startAddress + quantity <= numberOfInputRegisters)
    {
        QList<unsigned short> values;
        quantity += startAddress;
        for(; startAddress < quantity; startAddress++)
        {
            values.append(inputRegisters[startAddress]);
        }
        return values;
    }
    else
    {
        qDebug() << ("Error: Unable to get input registers");
        //BaseLogger::LogCritical("Error: Unable to get input registers");
        return QList<unsigned short>();
    }
}

void ModbusDatatable::GetInputRegisters_charArr(int address, int quantity, unsigned char * buffer)
{
    if(address >= 0 && address + quantity <= numberOfInputRegisters)
    {
        quantity += address;
        for(; address < quantity; address++)
        {
            buffer[1] = (inputRegisters[address] & 0x00FF);
            buffer[0] = (inputRegisters[address] & 0xFF00) >> 8;
            buffer += 2;
        }
    }
    else
    {
        qDebug() << ("Error: Unable to get input registers");
        //BaseLogger::LogCritical("Error: Unable to get input registers");
        //throw new std::out_of_range("address < 0 || address + quantity > numberOfInputRegisters");
        throw new std::exception();
    }
}

void ModbusDatatable::ResizeOutputRegisters(int quantity)
{
    if(quantity >= 0 && quantity < MAX_HOLDING_REGISTERS)
    {
        if(quantity == numberOfOutputRegisters)
        {
            return;
        }

        int numBytes = quantity * 2;

        if(outputRegisters == Q_NULLPTR)
        {
            outputRegisters = reinterpret_cast<ushort*>(malloc(numBytes));
        }

        if(quantity > numberOfOutputRegisters)
        {
            memset(outputRegisters + (numberOfOutputRegisters), 0, (quantity - numberOfOutputRegisters) * 2);
        }
        numberOfOutputRegisters = quantity;
    }
    else
    {
        qDebug() << ("Error: Unable to resize holding registers");
        //BaseLogger::LogCritical("Error: Unable to resize holding registers");
    }
}

int ModbusDatatable::NumberOfOutputRegisters()
{
    return numberOfOutputRegisters;
}

void ModbusDatatable::SetOutputRegister(int address, unsigned short value)
{
    if(address >= 0 && address < numberOfOutputRegisters)
    {
        outputRegisters[address] = value;
    }
    else
    {
        qDebug() << ("Error: Unable to set holding register");
        //BaseLogger::LogCritical("Error: Unable to set holding register");
    }
}

void ModbusDatatable::SetOutputRegisters(int address, unsigned char *buffer, int numRegisters)
{
    if(address >= 0 && (address + numRegisters <= numberOfOutputRegisters))
    {
        while(numRegisters)
        {
            outputRegisters[address] = ((buffer[1] << 8) | buffer[0]);
            //BaseLogger::Log("Set output register at " + QString::number(address) + " to " + QString::number(outputRegisters[address]));
            address++;
            buffer += 2;
            numRegisters--;
            emit OutputRegisterChanged(address);
        }
    }
    else
    {
        qDebug() << "Error: Index out of bounds in SetHoldingRegisters. Requesting " << (address + numRegisters) << " total " << numberOfOutputRegisters;
        //BaseLogger::LogCritical("Error: Index out of bounds in SetHoldingRegisters");
    }
}

void ModbusDatatable::SetOutputRegisters(int address, short* buffer, int numRegisters)
{
    if(address >= 0 && (address + numRegisters <= numberOfOutputRegisters))
    {
        while(numRegisters)
        {
            outputRegisters[address] = *(buffer++);
            //BaseLogger::Log("Set output register at " + QString::number(address) + " to " + QString::number(outputRegisters[address]));
            address++;
            buffer += 2;
            numRegisters--;
            emit OutputRegisterChanged(address);
        }
    }
    else
    {
        qDebug() << "Error: Index out of bounds in SetHoldingRegisters. Requesting " << (address + numRegisters) << " total " << numberOfOutputRegisters;
        //BaseLogger::LogCritical("Error: Index out of bounds in SetHoldingRegisters");
    }
}

unsigned short ModbusDatatable::GetOutputRegister(int address)
{
    if(address >= 0 && address < numberOfOutputRegisters)
    {
        return outputRegisters[address];
    }
    else
    {
        qDebug() << ("address < 0 || address >= numberOfOutputRegisters");
        //BaseLogger::LogCritical("address < 0 || address >= numberOfOutputRegisters");
        //throw new std::out_of_range("address < 0 || address >= numberOfOutputRegisters");
        throw new std::exception();
        //BaseLogger::LogCritical("Error: Unable to get holding register. Index out of bounds");
        //BaseLogger::LogCritical("Address: " + QString::number(address) + " Num Registers: " + QString::number(NumberOfOutputRegisters()));
        return 0;
    }
}

QList<short> ModbusDatatable::GetOutputRegisters(int startAddress, int quantity)
{
    if(startAddress >= 0 && startAddress + quantity <= numberOfOutputRegisters)
    {
        QList<short> values;
        for(int i = startAddress; i < startAddress + quantity; i++)
        {
            values.append(outputRegisters[i]);
        }
        return values;
    }
    else
    {
        qDebug() << ("Error: Unable to get holding registers. Index out of bounds");
        //BaseLogger::LogCritical("Error: Unable to get holding registers. Index out of bounds");
        return QList<short>();
    }
}

void ModbusDatatable::GetOutputRegisters_charArr(int address, int quantity, unsigned char * buffer)
{
    if(address >= 0 && address + quantity <= numberOfOutputRegisters)
    {
        quantity += address;
        for(; address < quantity; address++)
        {
            buffer[1] = (outputRegisters[address] & 0x00FF);
            buffer[0] = (outputRegisters[address] & 0xFF00) >> 8;
            buffer += 2;
        }
    }
    else
    {
        qDebug() << ("Error: Unable to get output registers: " + QString::number(address) + " + " + QString::number(quantity) + " > " + QString::number(numberOfOutputRegisters));
        //BaseLogger::LogCritical("Error: Unable to get output registers: " + QString::number(address) + " + " + QString::number(quantity) + " > " + QString::number(numberOfOutputRegisters));
        //throw new std::out_of_range(__PRETTY_FUNCTION__);
        throw new std::exception();
    }
}

QList<bool> ModbusDatatable::ConvertList(QList<char> original)
{
    QList<bool> values;

    for(int i = 0, j; i < original.length(); i++)
    {
        for(j = 7; j >= 0; j--)
        {
            values.append( (original[i] & (1 << j)) != 0 );
        }
    }
    return values;
}

QList<char> ModbusDatatable::ConvertList(QList<bool> original)
{
    QList<char> values;

    char temp = 0;
    for(int i = 0; i < original.length(); i++)
    {
        if(i % 8 == 0 && i != 0)
        {
            values.append(temp);
            temp = 0;
        }
        temp += (original[i] << (i % 8)); // 0 or 1 times 2 to the ith power
    }
    values.append(temp);

    return values;
}

bool ModbusDatatable::CheckBit(int val, int bit)
{
    return val & (1 << bit);
}

int ModbusDatatable::SetBit(int val, int bit)
{
    return val | (1 << bit);
}

int ModbusDatatable::ClearBit(int val, int bit)
{
    return val & ~(1 << bit);
}

void ModbusDatatable::CopyBools(bool* source, bool* dest, int quantity)
{
    while(quantity--)
    {
        *dest++ = *source++;
    }
//    for(int i = 0; i < quantity; i++)
//    {
//        dest[i] = source[i];
//    }
}

void ModbusDatatable::CopyShorts(unsigned short * source, unsigned short * dest, int quantity)
{
    while(quantity--)
    {
        *dest++ = *source++;
    }
//    for(int i = 0; i < quantity; i++)
//    {
//        dest[i] = source[i];
//    }
}
