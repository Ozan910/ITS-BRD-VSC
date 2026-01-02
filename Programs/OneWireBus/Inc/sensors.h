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
#include <stdbool.h>

typedef struct {
    uint8_t family;//8 Bit Family Code
    uint8_t serial[6];//48 Bit Serial Number (unique)
    uint8_t crc;//8 bit CRC
} ROM_Number;

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
    bool isROMValid;
    Scratchpad scratchpad;
    bool isScratchpadValid;
}TemperatureSensor;

/**
* @brief returns the sensor type name as string based on sensor type
* @param sensorType the sensor type
* @param stringBuffer has to be at least 8 char long.
* @return char pointer on string with sensory type name
*/
char* getSensorTypeString(uint8_t familyCode, char *stringBuffer);

/**
* @brief converts the int16_t temperatur in float and in scale by dividing by 16.0f
* @param intTemp the 16 bit in temperature
* @return the temperature in float
*/
float getFloatTemp(int16_t intTemp);

/**
* @brief formatiertes printen für Family, ROM und Messwert. Schreibt infos in gegebenen String. 
* @param buf der Buffer wo der String abgelegt wird. at least 40 long
* @param bufferSize größe des Buffers
* @param TemperatureSensor der Sensor der geprintet werden soll
*/
void prettyPrint(char* buf, uint16_t bufferSize, TemperatureSensor *temperatureSensor);

/**
* @brief schreibt den initialPrint in den buffer
* @param buf der String buffer (min 40 groß)
* @param size die größe des buffers
*/
void initialPrint(char *buf, uint16_t size);

#endif
//EOF