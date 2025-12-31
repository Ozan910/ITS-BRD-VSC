/**
 *******************************************************************
  * @file myTimer.h
  * @author Ozan
  * @date December 2025
  * @brief Header file of myTimer Module. 
 *******************************************************************
*/

#ifndef MYTIMER_H
#define MYTIMER_H

#include <stdint.h>

/**
* @brief program sleeps for a given amount of micro seconds. only use after using initTimer() from the timer module
* @param duration the amount of micro seconds
*/
void mySleep(uint32_t duration);

#endif
//EOF