#ifndef MODBUSVALUEDEFS_H
#define MODBUSVALUEDEFS_H

#define NEW_DATA_COIL 0
#define NEW_SET_COIL 1

#define TOTAL_MODBUS_COILS 2


#define DATA_START_ADDR 0
#define DATA_LENGTH 36

#define DATA_SER_NUM_ADDR 0
#define DATA_NO_ADDR 2
#define DATA_NO2_ADDR 4
#define DATA_NOX_ADDR 6
#define DATA_CELL_TEMP_ADDR 8
#define DATA_CELL_PRESS_ADDR 10
#define DATA_CELL_FLOW_ADDR 12
#define DATA_PDV1_ADDR 14
#define DATA_PDV2_ADDR 16
#define DATA_SCRUBBER_TEMP_ADDR 18
#define DATA_OZONE_FLOW_ADDR 20
#define DATA_ERROR_BYTE_ADDR 22
#define DATA_DATE_ADDR 23
#define DATA_TIME_ADDR 29
#define DATA_DUTY_PER_ADDR 35

#define SET_START_ADDR 40
#define SET_LENGTH 36

#define SET_AVG_TIME_ADDR 40 //unsinged char, 1 short
#define SET_DATE_ADDR 41 //char[7], 7 shorts
#define SET_TIME_ADDR 48 //char[7], 7 shorts
#define SET_AD_SHORT_ADDR 55 //unsigned char, 1 short
#define SET_AD_LONG_ADDR 56 //unsigned char, 1 short
#define SET_AD_PERCENT_ADDR 57 //unsigned char, 1 short
#define SET_AD_DIFF_ADDR 58 //unsigned char, 1 short
#define SET_NO_ANALOG_ADDR 59
#define SET_NO2_ANALOG_ADDR 60
#define SET_NO_SLOPE_ADDR 61 //float
#define SET_NO_ZERO_ADDR 63
#define SET_NO2_SLOPE_ADDR 65
#define SET_NO2_ZERO_ADDR 67
#define SET_OZONE_FLOW_SLOPE_ADDR 69
#define SET_CELL_FLOW_SLOPE_ADDR 71
#define SET_MODE_ADDR 73
#define SET_ERROR_CODE_ADDR 74
#define SET_DATE_FORMAT_ADDR 75
#define SET_SETTING_SERIAL_NUMBER_ADDR 76

#define TOTAL_MODBUS_REGISTERS 77


#endif // MODBUSVALUEDEFS_H
