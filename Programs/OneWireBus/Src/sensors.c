/**
 ******************************************************************
  * @file sensors.c
  * @author Ozan
  * @date January 2025
  * @brief Dieses Modul ist für die TemperaturSensoren für den One Wire Bus
 ******************************************************************
*/

#include "sensors.h"
#include <string.h>
#include <stdio.h>


char* getSensorTypeString(uint8_t familyCode, char *stringBuffer){
    switch(familyCode){
        case 0x10:
            strncpy(stringBuffer, "DS18S20", 8);
            break;

        case 0x28:
            strncpy(stringBuffer, "DS18B20", 8);
            break;

        default:
            strncpy(stringBuffer, "UNKNOWN", 8);
            break;
    }
    return stringBuffer;
}

float getFloatTemp(TemperatureSensor *sensor){
    int16_t preConv = sensor->scratchpad.temperature;
    if(sensor->rom_number.family == 0x10){
        int16_t temp_read = preConv >> 1;
        uint8_t countRemain = sensor->scratchpad.reserved2;
        uint8_t countPerC = sensor->scratchpad.reserved3;

        if(countPerC == 0) return (float)temp_read;//falls irgendwie countPerC 0 sein sollte

        return (float)temp_read - 0.25f + ((float)(countPerC - countRemain) / (float)countPerC);

    }else{
        return (preConv / 16.0f);
    }
}

void prettyPrint(char* buf, uint16_t bufferSize, TemperatureSensor *temperatureSensor){
    if(temperatureSensor->isROMValid && temperatureSensor->isScratchpadValid){
        uint8_t familyCode = temperatureSensor->rom_number.family;
        char model[8];
        getSensorTypeString(familyCode, model);
        snprintf(
            buf, bufferSize,
            "%-7s 0x%02X-%02X%02X%02X%02X%02X%02X-%02X %-10.5f",
            model,
            temperatureSensor->rom_number.crc,
            temperatureSensor->rom_number.serial[5],
            temperatureSensor->rom_number.serial[4],
            temperatureSensor->rom_number.serial[3],
            temperatureSensor->rom_number.serial[2],
            temperatureSensor->rom_number.serial[1],
            temperatureSensor->rom_number.serial[0],
            temperatureSensor->rom_number.family,
            getFloatTemp(temperatureSensor)
        );
    }else if(temperatureSensor->isROMValid){
        uint8_t familyCode = temperatureSensor->rom_number.family;
        char model[8];
        getSensorTypeString(familyCode, model);
        snprintf(
            buf, bufferSize,
            "%-7s 0x%02X-%02X%02X%02X%02X%02X%02X-%02X %-10s",
            model,
            temperatureSensor->rom_number.crc,
            temperatureSensor->rom_number.serial[5],
            temperatureSensor->rom_number.serial[4],
            temperatureSensor->rom_number.serial[3],
            temperatureSensor->rom_number.serial[2],
            temperatureSensor->rom_number.serial[1],
            temperatureSensor->rom_number.serial[0],
            temperatureSensor->rom_number.family,
            "disconnect"
        );
    }else{
        snprintf(buf, bufferSize, "%s", "Invalid ROM, CRC failed");
    }
}

void initialPrint(char *buf, uint16_t size) {
	snprintf(buf, size, "%-7s %-20s %-10s", "Sensor", "PDROM", "Temp. [C]");
}