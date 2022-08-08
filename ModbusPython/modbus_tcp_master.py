#!/usr/bin/env python3

from audioop import add
import socket
import struct

import modbus_datatable as dt
import modbus_tcp_message as msg

READ_OUTPUT_COILS               = 0x01
READ_INPUT_COILS                = 0x02
READ_OUTPUT_REGISTERS           = 0x03
READ_INPUT_REGISTERS            = 0x04
SET_SINGLE_OUTPUT_COIL          = 0x05
SET_SINGLE_OUTPUT_REGISTER      = 0x06
SET_MULTIPLE_OUTPUT_COILS       = 0x0F
SET_MULTIPLE_OUTPUT_REGISTERS   = 0x10

class ModbusMasterTCP():

    def __init__(self, targetIP, targetPort, targetID, datatable, verbose=False):
        self.targetIP = targetIP
        self.targetPort = targetPort
        self.targetID = targetID
        self.datatable = datatable
        self.transactionID = 1
        self.verbose = verbose
        self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.request = None
        self.response = None

    def OpenConnection(self) -> None:
        if self.verbose:
            print('Connecting to ' + self.targetIP + ' at ' + str(self.targetPort) + " with an id of " + str(self.targetID))

        try:
            self.conn.connect((self.targetIP, self.targetPort))
        except socket.error as err:
            print('Failed to connect to slave: ', err)

    def CloseConnection(self) -> None:
        if self.verbose:
            print('Modbus TCP Master is closing the connection')

        self.conn.close()

    def WriteRequest(self):
        if self.verbose:
            print('Modbus TCP Master is writing request')
        
        try:
            self.conn.send(self.request.GetBufferAsBytes())
        except socket.error as err:
            print('Error: Encountered error when trying to write message')
            # if type(err) == BrokenPipeError:
            #     print('Socket is closed: Trying to re-open connection')
            #     self.OpenConnection()
            #     self.conn.send(self.request.GetBufferAsBytes())

    def ReadResponse(self):
        self.response = msg.ModbusTCPMessage()
        
        self.response.AddBytes(self.conn.recv(260))

        self.ParseResponse()
        self.request = None
        self.response = None

    def ParseResponse(self):

        if self.response == None:
            print('Error: Trying to parse null response')
        elif not self.response.IsComplete:
            print('Error: Trying to parse incomplete response')
        else:
            if self.response.GetFunctionCode() == READ_OUTPUT_COILS:
                if self.verbose:
                    print('TCP Master is parsing response to READ_OUTPUT_COILS')

                self.ParseReadOutputCoils()

                if self.verbose:
                    print('TCP Master finished READ_OUTPUT_COILS')

            elif self.response.GetFunctionCode() == READ_INPUT_COILS:
                if self.verbose:
                    print('TCP Master is parsing response to READ_INPUT_COILS')

                self.ParseReadInputCoils()

                if self.verbose:
                    print('TCP Master finished READ_INPUT_COILS')

            elif self.response.GetFunctionCode() == READ_OUTPUT_REGISTERS:
                if self.verbose:
                    print('TCP Master is parsing response to READ_OUTPUT_REGISTERS')

                self.ParseReadOutputRegisters()

                if self.verbose:
                    print('TCP Master finished READ_OUTPUT_REGISTERS')

            elif self.response.GetFunctionCode() == READ_INPUT_REGISTERS:
                if self.verbose:
                    print('TCP Master is parsing response to READ_INPUT_REGISTERS')

                self.ParseReadInputRegisters()

                if self.verbose:
                    print('TCP Master finished READ_OUTPUT_REGISTERS')

            elif self.response.GetFunctionCode() == SET_SINGLE_OUTPUT_COIL:
                if self.verbose:
                    print('TCP Master is parsing response to SET_SINGLE_OUTPUT_COIL')

                self.ParseWriteOutputCoil()

                if self.verbose:
                    print('TCP Master finished SET_SINGLE_OUTPUT_COIL')

            elif self.response.GetFunctionCode() == SET_SINGLE_OUTPUT_REGISTER:
                if self.verbose:
                    print('TCP Master is parsing response to SET_SINGLE_OUTPUT_REGISTER')

                self.ParseWriteOutputRegister()

                if self.verbose:
                    print('TCP Master finished SET_SINGLE_OUTPUT_REGISTER')

            elif self.response.GetFunctionCode() == SET_MULTIPLE_OUTPUT_COILS:
                if self.verbose:
                    print('TCP Master is parsing response to SET_MULTIPLE_OUTPUT_COILS')

                self.ParseWriteMultipleOutputCoils()

                if self.verbose:
                    print('TCP Master finished SET_MULTIPLE_OUTPUT_COILS')

            elif self.response.GetFunctionCode() == SET_MULTIPLE_OUTPUT_REGISTERS:
                if self.verbose:
                    print('TCP Master is parsing response to SET_MULTIPLE_OUTPUT_REGISTERS')

                self.ParseWriteMultipleOutputRegisters()

                if self.verbose:
                    print('TCP Master finished SET_MULTIPLE_OUTPUT_REGISTERS')

            else:
                if self.verbose:
                    print('ERROR: TCP master response with unknown function code: ', self.response.GetFunctionCode())





    def PopulateMessageHeader(self, transactionID, protocol, packageLength, unitAddress):
        self.request.SetTransactionID(transactionID)
        self.request.SetProtocol(protocol)
        self.request.SetPackageLength(packageLength)
        self.request.SetUnitAddress(unitAddress)

    def ReadOutputCoils(self, start, length):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif (start + length) > self.datatable.numberOfOutputCoils:
            print('ERROR: Modbus Master TCP is requesting output coils that are out of bounds')
            print('Requesting coil: ', start, ' Number of coils: ', self.datatable.numberOfOutputCoils)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is requesting output coils when request is not null')

        if self.verbose:
            print('Modbus TCP Master is reading output coils starting at ', start, ' for ', length)

        self.request = msg.ModbusTCPMessage()

        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(READ_OUTPUT_COILS)

        self.request.SetAddress(start)

        self.request.SetValueCount(length)

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseReadOutputCoils(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            address = self.request.GetAddress()
            quantity = self.request.GetValueCount()
            source = self.response.GetPDU()[2:]
            for byte in source:
                for i in range(8):
                    check = 1 << i

                    if self.verbose:
                        print('Setting output coil at ', address, ' to ', (check & byte != 0))
                    self.datatable.SetOutputCoil(address, (check & byte != 0))

                    address += 1
                    quantity -= 1

                    if(quantity == 0):
                        break
        else:
            print('Modbus master received incomplete response to ReadOutputCoils')
        


    def ReadInputCoils(self, start, length):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif (start + length) > self.datatable.numberOfOutputCoils:
            print('ERROR: Modbus Master TCP is reading input coils that are out of bounds')
            print('Requesting input coils starting at ', start, ' for ', length)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is reading input coils when request is not null')

        if self.verbose:
            print('Modbus TCP Master is reading input coils starting at ', start, ' for ', length)

        self.request = msg.ModbusTCPMessage()

        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(READ_INPUT_COILS)

        self.request.SetAddress(start)

        self.request.SetValueCount(length)

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseReadInputCoils(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            address = self.request.GetAddress()
            quantity = self.request.GetValueCount()
            source = self.response.GetPDU()[2:]
            for byte in source:
                for i in range(8):
                    check = 1 << i

                    if self.verbose:
                        print('Setting input coil at ', address, ' to ', (check & byte != 0))
                    self.datatable.SetInputCoil(address, (check & byte != 0))

                    address += 1
                    quantity -= 1

                    if(quantity == 0):
                        break
        else:
            print('Modbus master received incomplete response to ReadInputCoils')



    def ReadOutputRegisters(self, start, length):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif (start + length) > self.datatable.numberOfOutputRegisters:
            print('ERROR: Modbus Master TCP is reading output registers that are out of bounds')
            print('Reading output registers starting at ', start, ' for ', length)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is reading registers when request is not null')

        if self.verbose:
            print('Reading output registers starting at ', start, ' for ', length)

        self.request = msg.ModbusTCPMessage()

        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(READ_OUTPUT_REGISTERS)

        self.request.SetAddress(start)

        self.request.SetValueCount(length)

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseReadOutputRegisters(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            address = self.request.GetAddress()
            quantity = self.request.GetValueCount()
            source = self.response.GetPDU()[2:]

            if len(source) != (quantity * 2):
                print('Error: Modbus TCP Master received bad number of data bytes in ParseReadOutputRegisters')

            byts = []
            for byte in source:
                byts.append(byte)
                
                if len(byts) == 2:
                    if self.verbose:
                        print('Output Register ', address, ' = ', struct.unpack('>h', bytes(byts))[0])
                    self.datatable.SetOutputRegister(address, struct.unpack('>h', bytes(byts))[0])
                    address += 1
                    byts = []
        else:
            print('Modbus master received incomplete response to ReadOutputRegisters')



    
    def ReadInputRegisters(self, start, length):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif (start + length) > self.datatable.numberOfInputRegisters:
            print('ERROR: Modbus Master TCP is reading input registers that are out of bounds')
            print('Reading input registers: End address: ', start + length, ' Number of input registers: ', self.datatable.numberOfInputRegisters)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is reading input registers when request is not null')

        if self.verbose:
            print('Modbus TCP Master is reading input registers starting at ', start, ' for ', length)

        self.request = msg.ModbusTCPMessage()

        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(READ_INPUT_REGISTERS)

        self.request.SetAddress(start)

        self.request.SetValueCount(length)

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseReadInputRegisters(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            address = self.request.GetAddress()
            quantity = self.request.GetValueCount()
            source = self.response.GetPDU()[2:]

            if len(source) != (quantity * 2):
                print('Error: Modbus TCP Master received bad number of data bytes in ParseReadOutputRegisters')

            byts = []
            for byte in source:
                byts.append(byte)
                if len(byts) == 2:
                    if self.verbose:
                        print('Input Register ', address, ' = ', struct.unpack('>h', bytes(byts))[0])
                    address += 1
                    byts = []
        else:
            print('Modbus master received incomplete response to ReadInputRegisters')

    

    def WriteOutputCoil(self, address, value=None):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif address > self.datatable.numberOfOutputCoils:
            print('ERROR: Modbus Master TCP is writing output coils that are out of bounds')
            print('Writing output coil at ', address, ' with ', value)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is writing output coil when request is not null')

        if value == None:
            value = self.datatable.GetOutputCoil(address)
        elif self.datatable.GetOutputCoil(address) != value:
            self.datatable.SetOutputCoil(address, value)
        
        if self.verbose:
            print('Modbus TCP Master is writing output coil at ', address, ' with ', value)

        self.request = msg.ModbusTCPMessage()

        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(SET_SINGLE_OUTPUT_COIL)

        self.request.SetAddress(address)

        self.request.SetRegisterValue(10, 0xFF00 if value else 0x0000)

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseWriteOutputCoil(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            if self.response.GetBuffer() == self.request.GetBuffer():
                #print('Received valid response for WriteOutputCoil')
                pass
            else:
                print('Received bad response for WriteOutputCoil')
        else:
            print('Modbus master received incomplete response to WriteOuputCoil')
        

    

    def WriteOuputRegister(self, address, value=None):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif address > self.datatable.numberOfOutputRegisters:
            print('ERROR: Modbus Master TCP is writing output register out of bounds')
            print('Writing output register at ', address, ' with ', value)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is writing output register when request is not null')

        if value == None:
            value = self.datatable.GetOutputRegister(address)
        elif self.datatable.GetOutputRegister(address) != value:
            self.datatable.SetOutputRegister(address, value)
        
        if self.verbose:
            print('Modbus TCP Master is writing output register at ', address, ' with ', value)

        self.request = msg.ModbusTCPMessage()
        
        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(SET_SINGLE_OUTPUT_REGISTER)

        self.request.SetAddress(address)

        self.request.SetRegisterValue(10, value)

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseWriteOutputRegister(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            if self.response.GetBuffer() == self.request.GetBuffer():
                pass
                #print('Received valid response for WriteOuputRegister')
            else:
                print('Received bad response for WriteOuputRegister')
        else:
            print('Modbus master received incomplete response to WriteOuputRegister')
        


    def WriteMultipleOutputCoils(self, address, length):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif (address + length) >= self.datatable.numberOfOutputCoils:
            print('ERROR: Modbus Master TCP is reading multiple output coils that are out of bounds')
            print('Writing output coils starting at ', address, ' for ', length)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is writing multiple output coils when request is not null')

        if self.verbose:
            print('Error: Modbus TCP Master is writing multiple output coils starting at ', address, ' for ', length)

        self.request = msg.ModbusTCPMessage()

        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(SET_MULTIPLE_OUTPUT_COILS)

        self.request.SetAddress(address)

        self.request.SetValueCount(length)

        self.request.SetByteValue(12, int((length + 7) / 8))

        for byte in self.datatable.GetOutputCoilsPacked(address, length):
            print('Packed byte: ', byte)
            self.request.AddByte(byte)

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseWriteMultipleOutputCoils(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            if self.response.GetBuffer() == self.request.GetBuffer():
                pass
                #print('Received valid response for ParseWriteMultipleOutputCoils')
            else:
                print('Received bad response for ParseWriteMultipleOutputCoils')
        else:
            print('Modbus master received incomplete response to ParseWriteMultipleOutputCoils')


    def WriteMultipleOuputRegisters(self, address, length):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return 
        elif (address + length) > self.datatable.numberOfOutputRegisters:
            print('ERROR: Modbus Master TCP is writing multiple output registers out of bounds')
            print('Writing output coils starting at ', address, ' for ', length)
            return
        elif self.request != None:
            print('ERROR: Modbus Master TCP is writing multiple output registers when request is not null')

        self.request = msg.ModbusTCPMessage()

        self.PopulateMessageHeader(self.transactionID, 0, 0, self.targetID)

        self.transactionID += 1
        if self.transactionID >= 65000:
            self.transactionID = 0

        self.request.SetFunctionCode(SET_MULTIPLE_OUTPUT_REGISTERS)

        self.request.SetAddress(address)

        self.request.SetValueCount(length)

        self.request.SetByteValue(12, length*2)

        for reg in self.datatable.GetOutputRegisters(address, length):
            self.request.AddBytes(struct.pack('>h', reg))

        self.request.UpdatePackageLength()

        self.conn.sendall(self.request.GetBufferAsBytes())

        self.ReadResponse()

    def ParseWriteMultipleOutputRegisters(self):
        if self.datatable == None:
            print('ERROR: Modbus Master TCP has a null datatable')
            return
        elif self.response == None:
            print('Error: Modbus TCP Master response is null')
            return

        if self.response.IsComplete():
            if self.response.GetBuffer() == self.request.GetBuffer():
                pass
                #print('Received valid response for ParseWriteMultipleOutputRegisters')
            else:
                print('Received bad response for ParseWriteMultipleOutputRegisters')
        else:
            print('Modbus master received incomplete response to ParseWriteMultipleOutputRegisters')

def OutputCoilsTest(master, verbose):

    if verbose:
        print('Starting output coils test')

    master.ReadOutputCoils(0, 10)

    for i in range(10):
        master.datatable.SetOutputCoil(i, False)

    master.WriteMultipleOutputCoils(0, 10)

    for i in range(10):
        master.datatable.SetOutputCoil(i, True)

    master.ReadOutputCoils(0, 10)

    for i in range(10):
        if master.datatable.GetOutputCoil(i):
            print('Failed to set output coil at 1: ', i, ' value = ', master.datatable.GetOutputCoil(i))

    for i in range(10):
        master.datatable.SetOutputCoil(i, True)

    master.WriteMultipleOutputCoils(0, 10)

    for i in range(10):
        master.datatable.SetOutputCoil(i, False)

    master.ReadOutputCoils(0, 10)

    for i in range(10):
        if not master.datatable.GetOutputCoil(i):
            print('Failed to set output coil at 2: ', i, ' value = ', master.datatable.GetOutputCoil(i))

    for i in range(10):
        master.datatable.SetOutputCoil(i, i % 2 != 0)

    master.WriteMultipleOutputCoils(0, 10)

    master.ReadOutputCoils(0, 10)

    for i in range(10):
        if master.datatable.GetOutputCoil(i) != (i % 2 != 0):
            print('Failed to set output coil at 3: ', i, ' value = ', master.datatable.GetOutputCoil(i))

    if verbose:
        print('Finished output coils test')

if __name__ == "__main__":
    print('Starting ModbusMasterTCP')

    master = ModbusMasterTCP('192.168.1.134', 50200, 1, dt.ModbusDatatable(100, 100, 400, 400), False)
    #master = ModbusMasterTCP('000.000.000.000', 50200, 5, dt.ModbusDatatable(100, 100, 100, 100))
    master.OpenConnection()

    #OutputCoilsTest(master, True)

    master.ReadOutputRegisters(0, 100)
    master.ReadOutputRegisters(100, 100)
    master.ReadOutputRegisters(200, 59)

    setRegisters = bytes(master.datatable.GetOutputRegistersPacked(0, 400))

    regs = []

    for i in range(0, len(setRegisters), 2):
        #print(struct.unpack('>h', setRegisters[i:i+2])[0])
        regs.append(struct.unpack('>h', setRegisters[i:i+2])[0])

    print('Avg Time: ', regs[0])
    print('Ad Short: ', regs[1])
    print('Ad Long: ', regs[2])
    print('Ad Diff: ', regs[3])
    print('Ad Per: ', regs[4])
    print('NO Slope: ', struct.unpack('>f', setRegisters[10: 14])[0])
    print('NO Zero: ', struct.unpack('>f', setRegisters[14: 18])[0])
    print('NO Analog: ', struct.unpack('>i', setRegisters[18: 22])[0])
    print('NO2 Slope: ', struct.unpack('>f', setRegisters[22: 26])[0])
    print('NO2 Zero: ', struct.unpack('>f', setRegisters[26: 30])[0])
    print('NO2 Analog: ', struct.unpack('>i', setRegisters[30: 34])[0])
    print('Ozone Flow Slope: ', struct.unpack('>f', setRegisters[34: 38])[0])
    print('Cell Flow Slope: ', struct.unpack('>f', setRegisters[38: 42])[0])
    print('Mode: ', regs[21])

    date = ''
    date += chr(regs[22])
    date += chr(regs[23])
    date += '/'
    date += chr(regs[24])
    date += chr(regs[25])
    date += '/'
    date += chr(regs[26])
    date += chr(regs[27])
    print("Date: ", date)

    date = ''
    date += chr(regs[28])
    date += chr(regs[29])
    date += '/'
    date += chr(regs[30])
    date += chr(regs[31])
    date += '/'
    date += chr(regs[32])
    date += chr(regs[33])
    print("Time: ", date)

    print('Serial Number: ', struct.unpack('>i', setRegisters[68: 72])[0])
    print('Error Code: ', struct.unpack('>i', setRegisters[72: 76])[0])
    print('Date format: ', regs[76])

    modbusIP = ''
    for i in range(77, 102):
        modbusIP += chr(regs[i])
        #print(i, regs[i], chr(regs[i]))
    print('Modbus IP: ', modbusIP)

    print('Modbus Port: ', struct.unpack('>i', setRegisters[204: 208])[0])
    print('Device ID: ', regs[104])

    staticIP = ''
    for i in range(105, 130):
        staticIP += chr(regs[i])
        #print(i, regs[i], chr(regs[i]))
    print('Static IP: ', modbusIP)

    gateway = ''
    for i in range(130, 155):
        gateway += chr(regs[i])
        #print(i, regs[i], chr(regs[i]))
    print('Gateway: ', gateway)

    subnet = ''
    for i in range(155, 180):
        subnet += chr(regs[i])
        #print(i, regs[i], chr(regs[i]))
    print('Subnet: ', subnet)

    date = ''
    for i in range(180, 205):
        subnet += chr(regs[i])
        #print(i, regs[i], chr(regs[i]))
    print('Date: ', date)

    print('Serial number: ', struct.unpack('>i', setRegisters[410: 414])[0])
    print('PDV2: ', struct.unpack('>f', setRegisters[414: 418])[0])
    print('Scrubber Temp: ', struct.unpack('>f', setRegisters[418: 422])[0])
    print('Ozone Flow: ', struct.unpack('>f', setRegisters[422: 426])[0])
    print('Error Byte: ', struct.unpack('>i', setRegisters[426: 430])[0])
    
    time = ''
    for i in range(215, 240):
        subnet += chr(regs[i])
        #print(i, regs[i], chr(regs[i]))
    print('Time: ', time)

    print('Mode: ', regs[240])
    print('Duty Percent: ', struct.unpack('>i', setRegisters[482: 486])[0])
    print('Log Number: ', struct.unpack('>i', setRegisters[486: 490])[0])
    print('NO: ', struct.unpack('>f', setRegisters[490: 494])[0])
    print('NO2: ', struct.unpack('>f', setRegisters[494: 498])[0])
    print('NOx: ', struct.unpack('>f', setRegisters[498: 502])[0])
    print('Cell Temp: ', struct.unpack('>f', setRegisters[502: 506])[0])
    print('Cell Press: ', struct.unpack('>f', setRegisters[506: 510])[0])
    print('Cell Flow: ', struct.unpack('>f', setRegisters[510: 514])[0])
    print('PDV1: ', struct.unpack('>f', setRegisters[514: 518])[0])






    # print('NO Slope: ', struct.unpack('<f', setRegisters[10: 14])[0])

    # temp = []
    # for b in struct.pack('>h', regs[6]):
    #     temp.append(b)
    # for b in struct.pack('>h', regs[5]):
    #     temp.append(b)
    # print(temp)
    # print('NO Slope: ', struct.unpack('>f', bytes(temp))[0])

    # print('Correct: ', struct.pack('>f', 1.00), len(struct.pack('>f', 1.00)))
    # print('Current: ', setRegisters[10: 14])
    # print('Unpacked: ', struct.unpack('>f', setRegisters[10: 14])[0])

    exit(0)

    print('Avg Time: ', struct.unpack('>h', setRegisters[0:2])[0])
    print('Ad Short: ', struct.unpack('>h', setRegisters[2:4])[0])
    print('Ad Long: ', struct.unpack('>h', setRegisters[4:6])[0])
    print('Ad Diff: ', struct.unpack('>h', setRegisters[6:8])[0])
    print('Ad Per: ', struct.unpack('>h', setRegisters[8:10])[0])
    print('NO Slope: ', struct.unpack('<f', setRegisters[10: 14])[0])
    print('NO Zero: ', struct.unpack('>f', setRegisters[16: 20])[0])
    print('NO Analog: ', struct.unpack('>i', setRegisters[20: 24])[0])
    print('NO2 Slope: ', struct.unpack('>f', setRegisters[24: 28])[0])
    print('NO2 Zero: ', struct.unpack('>f', setRegisters[28: 32])[0])
    print('NO2 Analog: ', struct.unpack('>i', setRegisters[32: 36])[0])
    print('Ozone Flow Slope: ', struct.unpack('>f', setRegisters[36: 40])[0])
    print('Cell Flow Slope: ', struct.unpack('>f', setRegisters[40: 44])[0])
    print('Mode: ', struct.unpack('>h', setRegisters[44:46])[0])

    date = ''
    date += chr(setRegisters[47])
    date += chr(setRegisters[49])
    date += '/'
    date += chr(setRegisters[51])
    date += chr(setRegisters[53])
    date += '/'
    date += chr(setRegisters[55])
    date += chr(setRegisters[57])
    print("Date: ", date)

    date = ''
    date += chr(setRegisters[48])
    date += chr(setRegisters[50])
    date += '/'
    date += chr(setRegisters[52])
    date += chr(setRegisters[54])
    date += '/'
    date += chr(setRegisters[56])
    date += chr(setRegisters[58])
    print("Date: ", date)

    date = ''
    date += chr(struct.unpack('>h', setRegisters[48:50])[0])
    date += chr(struct.unpack('>h', setRegisters[50:52])[0])
    date += '/'
    date += chr(struct.unpack('>h', setRegisters[52:54])[0])
    date += chr(struct.unpack('>h', setRegisters[54:56])[0])
    date += '/'
    date += chr(struct.unpack('>h', setRegisters[56:58])[0])
    date += chr(struct.unpack('>h', setRegisters[58:60])[0])
    print("Date: ", date)
    
    time = ''
    #time += chr(setRegisters)


    print('Finished')

    # Total setting registers: 180
    # Total data registers: 79
    # Total number of registers: 259

    # master.ReadOutputCoils(0, 10)

    # master.datatable.LogOutputCoils()
    # master.datatable.LogOutputCoils(0,10)

    # master.ReadInputCoils(0, 10)

    # master.ReadOutputRegisters(0, 100)

    # master.ReadInputRegisters(0, 100)

    # master.ReadOutputCoils(0, 10)
    # master.datatable.LogOutputCoils(0,10)
    # master.WriteOutputCoil(2, False)
    # master.ReadOutputCoils(0, 10)
    # master.datatable.LogOutputCoils(0,10)

    # for i in range(master.datatable.numberOfOutputCoils):
    #     master.datatable.SetOutputCoil(i, True)
    # master.WriteMultipleOutputCoils(0, 10)
    # master.datatable.LogOutputCoils(0, 10)

    # master.ReadOutputCoils(0, 10)
    # master.datatable.LogOutputCoils(0, 10)

    # for i in range(master.datatable.numberOfOutputCoils):
    #     master.datatable.SetOutputCoil(i, False)
    # master.WriteMultipleOutputCoils(0, 10)
    # master.datatable.LogOutputCoils(0, 10)

    # master.ReadOutputCoils(0, 10)
    # master.datatable.LogOutputCoils(0, 10)

    # master.WriteMultipleOuputRegisters(0,100)

    # master.ReadOutputRegisters(0, 100)

    # print('Setting register 25')
    # master.WriteOuputRegister(25, 100)
    # print('Reading register 25')
    # master.ReadOutputRegisters(25, 1)
    # print('Reg 25: ', master.datatable.GetOutputRegister(25))

    # print('Setting register 25')
    # master.WriteOuputRegister(25, 43)
    # print('Reading register 25')
    # master.ReadOutputRegisters(25, 1)
    # print('Reg 25: ', master.datatable.GetOutputRegister(25))

    # for i in range(100):
    #     master.datatable.SetOutputRegister(i, i)
    # master.WriteMultipleOuputRegisters(0,100)
    # for i in range(100):
    #     master.datatable.SetOutputRegister(i, 0)
    # master.ReadOutputRegisters(0,100)
    # master.datatable.LogOutputRegisters()

    
    # master.datatable.LogOutputCoils()
    # master.datatable.LogInputCoils()
    # master.datatable.LogOutputRegisters()
    # master.datatable.LogInputRegisters()
    
    # print('Writing multiple registers')
    # master.WriteMultipleOuputRegisters(0, 100)
    # master.datatable.LogOutputRegisters()

    # print('Finished functions')

    # for i in range(100):
    #     master.datatable.SetOutputRegister(i, i)
    # master.datatable.LogOutputRegisters()
    # print(master.datatable.GetOutputRegister(1))
    # print(master.datatable.GetOutputRegisters(0, 100))
    # for v in master.datatable.GetOutputRegisters(0,100):
    #     print(v)

    # message = tcp_message.ModbusTCPMessage()

    # message.SetTransactionID(1)
    # message.SetProtocol(2)
    # message.SetPackageLength(3)
    # message.SetUnitAddress(4)
    # message.SetFunctionCode(5)
    # message.SetAddress(6)
    # message.SetValueCount(7)
    # message.UpdatePackageLength()

    # message.LogInformation()