/**
 *******************************************************************
  * @file myList.h
  * @author Ozan
  * @date December 2025
  * @brief Header file of myList Module. 
 *******************************************************************
*/

#ifndef MYLIST_H
#define MYLIST_H

#define MAX_SENSORS 16
#include "sensors.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct{
TemperatureSensor list[MAX_SENSORS];
uint16_t size;
} SensorList;

/**
* @brief offer an element to the list. will be added if its not already contained (PK ROM)
* @param list the List thats worked on
* @param sensor the Element worked with
* @return true: if the list didnt contain the element and took it, false: if the list already contained the element or was already full
*/
bool offerToList(SensorList *list, TemperatureSensor *sensor);


/** 
* @brief check if a List already contains a sensor, check by ROM (PK)
* @param list the List thats worked on
* @param sensor the Element worked with
* @return true: if the list cotains the given element, false: if it doesnt
*/
bool listContains(SensorList *list, TemperatureSensor *sensor);


/** 
* @brief if the list contains the given elemenen it will be deleted, checked by ROM (PK)
* @param list the List thats worked on
* @param sensor the Element worked with
* @return true: if the listed had the given element and it was removed, false: if the list didnt contain the given element
*/
bool deleteIfContains(SensorList *list, TemperatureSensor *sensor);


#endif
//EOF