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
#include <stdbool.h>


#define PINNR 0
//PINNR HAS TO BE 0!!!
#define MODER_MASK_PIN_0 (0x03U << (2 * 0))
#define OUTPUT_MASK_PIN_0 (0x01U << (2 * 0))
#define MODER_MASK_PIN_1 (0x03U << (2 * 1))
#define OUTPUT_MASK_PIN_1 (0x01U << (2* 1))

typedef struct {
    uint8_t family;//8 Bit Family Code
    uint8_t serial[6];//48 Bit Serial Number (unique)
    uint8_t crc;//8 bit CRC
} ROM_Number;

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

/**
* @brief used to suppy the parasite sensors with power for 750 ms. this function sets the bus to push-pull and HIGH, supplies the power then sets it back to open drain. 3.3V will be on the bus for 750ms. GPIOD, PINNR (PD0)
*/
void powerSupply750ms(void);

/**
* @brief receives the 64 Bit ROM of a single sensor
* @return returns a ROM_Number struct containing the family code, serial number and crc of the ROM received
*/
ROM_Number receiveSingleROM(void);

/**
* @brief initiates the Pins for the 1 Wire Bus by Setting PD1 to MODER Output, OTYPER push-pull mode and on HIGH and by Setting PD0 to MODER Output, OTYPER open-drain mode.
*/
void init1WireBus(void);

/**
* @brief checks if a ROM_Number has a valid crc using the checkCRC() function
* @param rom pointer on the ROM_Number struct that will be checked
* @return true if the crc ist valid, false if not
*/
bool checkROMCRC(ROM_Number *rom);

#endif
//EOF