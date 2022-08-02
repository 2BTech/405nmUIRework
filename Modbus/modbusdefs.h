#ifndef MODBUSDEFS_H
#define MODBUSDEFS_H

//Function codes
#define READ_COILS                      0x01
#define READ_DISCRETE_INPUTS            0x02
#define READ_HOLDING_REGISTERS          0x03
#define READ_INPUT_REGISTERS            0x04
#define SET_SINGLE_COIL                 0x05
#define SET_SINGLE_REGISTER             0x06
#define SET_MULTIPLE_COILS              0x0F
#define SET_MULTIPLE_REGISTERS          0x10

// Modbus exception codes
#define ILLEGAL_FUNCTION                0x01
#define ILLEGAL_DATA_ADDRESS            0x02
#define ILLEGAL_DATA_VALUE              0x03
#define SLAVE_DEVICE_FAILURE            0x04

#endif // MODBUSDEFS_H
