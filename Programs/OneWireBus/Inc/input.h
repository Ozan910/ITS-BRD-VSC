/**
 *******************************************************************
  * @file input.h
  * @author Ozan
  * @date January 2026
  * @brief Header file of the input module
 *******************************************************************
*/

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

/**
* @brief Prüft, ob der Taster S6 aktuell gedrückt ist.
* 
* @return true  Wenn der Taster S6 gedrückt ist
* @return false  Wenn der Taster S6 nicht gedrückt ist
*/
bool inputs_isS0Pressed(void);

void mySetLED(int ledId);

void myClearLED(int ledId);

#endif
//eof