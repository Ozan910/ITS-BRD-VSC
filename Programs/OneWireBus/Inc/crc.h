/**
  * @file crc.h
  * @author Franz Korf, HAW Hamburg 
  * @date April 2016
  * @brief Header file of crc Module. 
  */

#ifndef _CRC_H
#define _CRC_H

#include <stdbool.h>

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

#endif // _CRC_H
// EOF