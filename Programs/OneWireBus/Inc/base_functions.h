/**
 ******************************************************************
  * @file base_functions.h
  * @author Ozan
  * @date December 2025
  * @brief Header file of base_functions Module. ONLY USE IN OPEN-DRAIN MODE!!!
 ******************************************************************
*/

#ifndef BASE_FUNCTIONS_H
#define BASE_FUNCTIONS_H

#include <stdint.h>

/**
* @brief sends a 1 to the bus
*/
void sendBit1(void);

/**
* @brief sends a 0 to the bus
*/
void sendBit0(void);

/**
* @brief reads a bit from the bus
* @return the bit that has been read (0 or 1)
*/
uint8_t readBit(void);

/**
* @brief resets the bus and reads the presence pulse
* @return state of the bus if 0: presence detected, else no slaves on bus
*/
uint8_t sendReset(void);

/**
* @brief reads 1 Byte from the bus by using readBit
* @return the received Byte
*/
uint8_t readByte(void);

/**
* @brief sends 1 Byte to the bus by using sendBit1 and sendBit0
* @param byte the byte to be sent
*/
void sendByte(uint8_t byte);

#endif
//EOF