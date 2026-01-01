/**
  * @file crc.h
  * @author Franz Korf, HAW Hamburg 
  * @date April 2016
  * @brief Header file of crc Module. 
  */

#ifndef _CRC_H
#define _CRC_H

#include <stdbool.h>
#include "sensors.h"

/**
 * @brief This function executes CRC check
 *
 * @param size size of var length array arr
 *
 * @param arr This buffer contains input for CRR check.
 *            arr[0] : LSB , arr[size - 1] : MSB
 *
 * @retval Result of CRC check
 */
 bool checkCRC(const unsigned int size, const unsigned char arr[size]);

 /**
* @brief checks if a ROM_Number has a valid crc using the checkCRC() function
* @param rom pointer on the ROM_Number struct that will be checked
* @return true if the crc ist valid, false if not
*/
bool checkROMCRC(ROM_Number *rom);

/**
* @brief checks if a scratchpad has a valid crc using the checkCRC() function
* @param rom pointer on the Scratchpad struct that will be checked
* @return true if the crc ist valid, false if not
*/
bool checkScratchCRC(Scratchpad *scratchpad);

#endif // _CRC_H
// EOF