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

SensorType getSensorType(uint8_t familyCode){
    switch(familyCode){
        case 0x10:
            return DS18S20;

        case 0x28:
            return DS18B20;

        default:
            return UNKNOWN;
    }
}

char* getSensorTypeString(SensorType sensorType, char *stringBuffer){
    switch(sensorType){
        case DS18S20:
            strncpy(stringBuffer, "DS18S20", 8);
            break;

        case DS18B20:
            strncpy(stringBuffer, "DS18B20", 8);
            break;

        default:
            strncpy(stringBuffer, "UNKNOWN", 8);
            break;
    }
    return stringBuffer;
}

float getFloatTemp(int16_t intTemp){
    return (intTemp / 16.0f);
}

