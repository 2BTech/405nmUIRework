from operator import le
import re
from tabnanny import check
import struct

from numpy import append

class ModbusDatatable():

    def __init__(self, numOutputCoils, numInputCoils, numOutputRegisters, numInputRegisters) -> None:
        self.numberOfOutputCoils = numOutputCoils
        self.outputCoils = []
        for _ in range(numOutputCoils):
            self.outputCoils.append(False)

        self.numberOfInputCoils = numInputCoils
        self.inputCoils = []
        for _ in range(numInputCoils):
            self.inputCoils.append(False)

        self.numberOfOutputRegisters = numOutputRegisters
        self.outputRegisters = []
        for _ in range(numOutputRegisters):
            self.outputRegisters.append(0)

        self.numberOfInputRegisters = numInputRegisters
        self.inputRegisters = []
        for _ in range(numInputRegisters):
            self.inputRegisters.append(0)

    def SetOutputCoil(self, address, value):
        #print('Setting output coil ', address, ' to ', value)
        self.outputCoils[address] = value

    def SetOutputCoils(self, address, length, bytes):
        for byte in bytes:
            for i in range(8):
                check = 1 << i

                self.SetOutputCoil(address, (byte & check) != 0)

                length -= 1
                address += 1

                if length <= 0:
                    return
    
    def GetOutputCoil(self, address):
        return self.outputCoils[address]

    def GetOutputCoils(self, address, length):
        return self.outputCoils[address:address+length]

    def GetOutputCoilsPacked(self, start, length):
        result = []
        byte = 0
        index = 0

        length += start
        while start < length:

            if self.outputCoils[start]:
                byte |= 1 << index

            index += 1
            start += 1

            if index >= 8:
                index = 0
                result.append(byte)
                byte = 0
            
        if index != 0:
            result.append(byte)

        return result

    def LogOutputCoils(self, start=None, length=None):
        if start == None:
            start = 0
        if length == None:
            length = self.numberOfOutputCoils

        print('Datatable Output Coils: ', start, ' to ', length)
        print(self.outputCoils[start:length])



    def SetInputCoil(self, address, value):
        #print('Setting input coil ', address, ' to ', value)
        self.inputCoils[address] = value

    def SetInputCoils(self, address, length, bytes):
        for byte in bytes:
            for i in range(8):
                check = 1 << i

                self.SetInputCoil(address, (byte & check) != 0)

                length -= 1
                address += 1

                if length <= 0:
                    return
    
    def GetInputCoil(self, address):
        return self.inputCoils[address]

    def GetInputCoils(self, address, length):
        return self.inputCoils[address:address+length]

    def GetInputCoilsPacked(self, start, length):
        result = []
        byte = 0
        index = 0

        length += start
        while start < length:

            if self.inputCoils[start]:
                byte |= 1 << index

            index += 1
            start += 1

            if index >= 8:
                index = 0
                result.append(byte)
                byte = 0
        return result

    def LogInputCoils(self, start=None, length=None):
        if start == None:
            start = 0
        if length == None:
            length = self.numberOfInputCoils

        print('Datatable Input Coils: ', start, ' to ', length)
        print(self.inputCoils[start:length])

    def SetOutputRegister(self, address, value):
        #print('Setting output register ', address, ' to ', value)
        self.outputRegisters[address] = value

    def SetOutputRegisters(self, address, length, values):
        for i in range(len(values)):
            print('Setting register: ', i + address)
            self.outputRegisters[i + address] = values[i]
    
    def GetOutputRegister(self, address):
        return self.outputRegisters[address]

    def GetOutputRegisters(self, address, length):
        return self.outputRegisters[address:address+length]

    def GetOutputRegistersPacked(self, address, length):
        res = []
        for byt in self.GetOutputRegisters(address, length):
            for v in struct.pack('>h', byt):
                res.append(v)
        return res

    def LogOutputRegisters(self, start=None, length=None):
        if start == None:
            start = 0
        if length == None:
            length = self.numberOfOutputCoils

        print('Datatable Output Registers: ', start, ' to ', length)
        print(self.outputRegisters[start:length])


    
    def SetInputRegister(self, address, value):
        #print('Setting input register ', address, ' to ', value)
        self.inputRegisters[address] = value
    
    def SetInputRegisters(self, address, length, values):
        for i in range(len(values)):
            self.inputRegisters[i + address] = values[i]

    def GetInputRegister(self, address):
        return self.inputRegisters[address]

    def GetInputRegisters(self, address, length):
        return self.inputRegisters[address:address+length]

    def LogInputRegisters(self, start=None, length=None):
        if start == None:
            start = 0
        if length == None:
            length = self.numberOfInputCoils

        print('Datatable Input Registers: ', start, ' to ', length)
        print(self.inputRegisters[start:length])

if __name__ == "__main__":
    print("In modbus datatable")

    datatable = ModbusDatatable(100, 100, 100, 100)

    # Testing output coils
    # datatable.LogOutputCoils()
    
    # for i in range(100):
    #     datatable.SetOutputCoil(i, i % 2 == 0)

    # datatable.LogOutputCoils()

    # for i in range(100):
    #     datatable.SetOutputCoil(i, False)

    datatable.SetOutputCoils(0, 16, [0b11110000, 0b00001111])
    print(datatable.GetOutputCoils(0, 8))
    print(datatable.GetOutputCoilsPacked(0, 16))
    print(datatable.GetOutputCoilsPacked(4, 8))

    #datatable.LogOutputCoils()