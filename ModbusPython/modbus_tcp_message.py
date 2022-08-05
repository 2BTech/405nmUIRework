from operator import indexOf, le
import struct

from numpy import insert, source

class ModbusTCPMessage():

    def __init__(self):
        # Create a buffer of length 260
        self.buffer = []
        for i in range(0, 260):
            self.buffer.append(0)
        self.length = 0

    def GetTransactionID(self):
        return struct.unpack('>H', bytes(self.buffer[0:2]))

    def SetTransactionID(self, value):
        source = struct.pack('>H', value)
        self.buffer[0] = source[0]
        self.buffer[1] = source[1]

    def GetProtocol(self):
        return struct.unpack('>H', bytes(self.buffer[2:4]))

    def SetProtocol(self, value):
        source = struct.pack('>H', value)
        self.buffer[2] = source[0]
        self.buffer[3] = source[1]

    def GetPackageLength(self):
        return struct.unpack('>H', bytes(self.buffer[4:6]))

    def SetPackageLength(self, value):
        source = struct.pack('>H', value)
        self.buffer[4] = source[0]
        self.buffer[5] = source[1]

    def UpdatePackageLength(self):
        if self.length < 7:
            self.SetPackageLength(0)
        else:
            self.SetPackageLength(self.length - 6)

    def SetUnitAddress(self, unitAddress):
        self.buffer[6] = unitAddress

        if self.length < 7:
            self.length = 7

    def GetUnitAddress(self):
        if self.length < 7:
            return self.buffer[6]
        else:
            return -1

    def SetFunctionCode(self, functionCode):
        self.buffer[7] = functionCode

        if self.length < 8:
            self.length = 8

    def GetFunctionCode(self):
        if self.length > 8:
            return self.buffer[7]
        else:
            return -1

    def GetAddress(self):
        return struct.unpack('>H', bytes(self.buffer[8:10]))[0]

    def SetAddress(self, value):
        source = struct.pack('>H', value)
        self.buffer[8] = source[0]
        self.buffer[9] = source[1]

        if self.length < 10:
            self.length = 10

    def GetValueCount(self):
        return struct.unpack('>H', bytes(self.buffer[10:12]))[0]

    def SetValueCount(self, value):
        source = struct.pack('>H', value)
        self.buffer[10] = source[0]
        self.buffer[11] = source[1]

        if self.length < 12:
            self.length = 12

    def SetRegisterValue(self, index, value):
        if index > 0 and index < 259:
            source = struct.pack('>H', value)
            self.buffer[index] = source[0]
            self.buffer[index+1] = source[1]

            if self.length < index + 2:
                self.length = index + 2
        else:
            print('Error: Trying to set value in modbus tcp message at out of bounds location: ' + str(index))

    def GetRegisterValue(self, index):
        if index > 0 and index < self.length - 1:
            return struct.unpack('>H', bytes(self.buffer[index, index + 2]))[0]
        else:
            print('ERROR: Trying to get value in modbus tcp message at out of bounds location: ' + str(index))

    def SetByteValue(self, index, value):
        if index > 0 and index < 260:
            self.buffer[index] = value

            if self.length < index + 1:
                self.length = index + 1
        else:
            print('Error: Trying to set byte in modbus tcp message at out of bounds location: ' + str(index))

    def GetByteValue(self, index):
        if index > 0 and index < self.length:
            return self.buffer[index]
        else:
            print('ERROR: Trying to get byte in modbus tcp message at out of bounds location: ' + str(index))

    def Reset(self):
        self.length = 0
        for i in range(0, 260):
            self.buffer[i] = 0

    def AddByte(self, char):
        if self.length < 260:
            self.buffer[self.length] = char
            self.length += 1
        else:
            print('Error: Trying to add byte to modbus tcp message when it is full')

    def AddBytes(self, data):
        for byte in data:
            self.buffer[self.length] = byte
            self.length += 1

    def IsComplete(self):
        if self.length < 7:
            return False
        else:
            packageLength = struct.unpack('>h', bytes(self.buffer[4:6]))[0] + 6
            return packageLength == self.length

    def GetPDU(self):
        return self.buffer[7:self.length]

    def GetPDULength(self):
        return self.length - 7

    def GetHeader(self):
        return self.buffer[0:7]

    def GetBuffer(self):
        return self.buffer[:self.length]

    def GetBufferAsBytes(self):
        return bytes(self.buffer[:self.length])

    def LogInformation(self):
        print('Type: ModbusTCPMessage',
              'Length: ' + str(self.length),
              '\tTransaction ID: ' + str(self.GetTransactionID()),
              '\tProtocol: ' + str(self.GetProtocol()),
              '\tPackage length: ' + str(self.GetPackageLength()),
              sep='\n')