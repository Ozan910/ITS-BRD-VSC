/**
 *******************************************************************
  * @file myTimer.c
  * @author Ozan
  * @date December 2025
  * @brief Dieses Modul implementiert myTimer mit der sleep Funktion.	
 *******************************************************************
*/

#include "myTimer.h"
#include "timer.h"

void mySleep(uint32_t duration){
    uint32_t start = getTimeStamp();
    uint32_t waitFor = duration * TICKS_PER_US;
    
    while((getTimeStamp() - start) < waitFor){
        //ZzZzZz
    }
}