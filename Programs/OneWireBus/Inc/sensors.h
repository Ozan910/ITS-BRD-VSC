/**
 ******************************************************************
  * @file sensors.h
  * @author Ozan
  * @date January 2025
  * @brief Header File for sensors module
 ******************************************************************
*/

#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

typedef struct {
    uint8_t family;//8 Bit Family Code
    uint8_t serial[6];//48 Bit Serial Number (unique)
    uint8_t crc;//8 bit CRC
} ROM_Number;

typedef enum{
    UNKNOWN,
    DS18B20,
    DS18S20
} SensorType;

typedef struct{
    int16_t temperature;//first 2 bytes = temp in uint16 lsb first
    uint8_t tHRegister;//or user Byte 1
    uint8_t tLRegister;//or user Byte 2
    uint8_t configRegister;
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t crc;
} Scratchpad;

typedef struct{
    ROM_Number rom_number;//unique ROM Number
    SensorType type;
    Scratchpad scratchpad;
    float measuredTemperature;
}TemperatureSensor;

/**
* @brief returns the sensor type as enum based on the family code
* @param familyCode the 8 Bit family code defining the sensor type
* @return the Sensor Type as enum
*/
SensorType getSensorType(uint8_t familyCode);


/**
* @brief returns the sensor type name as string based on sensor type
* @param sensorType the sensor type
* @param stringBuffer has to be at least 8 char long.
* @return char pointer on string with sensory type name
*/
char* getSensorTypeString(SensorType sensorType, char *stringBuffer);

/**
* @brief converts the int16_t temperatur in float and in scale by dividing by 16.0f
* @param intTemp the 16 bit in temperature
* @return the temperature in float
*/
float getFloatTemp(int16_t intTemp);

#endif
//EOF
