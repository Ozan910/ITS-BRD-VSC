/**
 *******************************************************************
  * @file interrupts.h
  * @author Ozan
  * @date January 2026
  * @brief Header file of the interrupts module
 *******************************************************************
*/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include "fsm.h"

/**
* @brief sends pointers on variables where the interrupt is supposed to write in
* @param pdir: adress of the variable where the Direction will be stored
* @param ppos: adress of the variable where the Position will be stored
* @param piTmr: adress of the variable where the TimeStamp will be stored
*/
void sendAdresses(volatile Direction* pdir, volatile int32_t* ppos, volatile uint32_t* piTmr);

/**
* @brief initiates the Interrupts for GPIOG 0 and 1
*/
void initInterrupts(void);


/** 
* @brief returns the value of the old phase
* @return the old phase
*/
uint8_t getOldPhase(void);

/** 
* @brief sets the value of the oldPhase (ONLY USE BEFORE INITINTERRUPTS AND IN ERRORHANDLER BEFORE DEMASKING)
* @param phase the value OldPhase will be set to
*/
void setOldPhase(uint8_t phase);


#endif
//eof