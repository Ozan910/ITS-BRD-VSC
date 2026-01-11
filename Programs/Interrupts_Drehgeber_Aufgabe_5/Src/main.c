/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "fsm.h"
#include "rechner.h"
#include "ledOutputs.h"
#include "displayOutputs.h"
#include "fehler.h" 
#include "timer.h"   
#include "stm32f4xx.h"

#define REFRESH_RATE_TICKS (250000 * TICKS_PER_US)
#define MAXREFRESH_RATE_TICKS (500000 * TICKS_PER_US)

static inline uint8_t readPhase(){
  return GPIOF->IDR & 0x03U;
}


int main(){
  initITSboard();
  GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
  displayOutputs_initialPrint();
  initTimer();

  int32_t position = 0;
  int32_t lastPosition = 0;
  uint32_t stepCount = 0;
  uint32_t lastStepCount = 0;
  uint32_t currentTimeTicks = 0;
  Direction dir = IDLE;
  Direction lastDir = IDLE;
  setOldPhase(readPhase());//die ersten beiden bits des von GPIOF also FPIN0 und FPIN1
  uint32_t lastTimeTicks = getTimeStamp();
  
  
  while(1){//superloop
  //fehler abfangen
  if(dir == ERR){
    ledOutputs_setError();
    displayOutputs_printError();

    waitForReset();

    ledOutputs_clearError();
    displayOutputs_clearError();

    setOldPhase(readPhase());
    dir = IDLE;
    lastDir = IDLE;
    lastStepCount = 0;
    stepCount = 0;
    lastPosition = 0;
    position = 0;
    lastTimeTicks = getTimeStamp();
    continue;
  }

  //input
  dir = getDirection(readPhase());
  currentTimeTicks = getTimeStamp();

  //auf state reagieren
  switch (dir){
    case FWD:
      position++;
      stepCount++;
      break;

    case REV:
      position--;
      stepCount++;
      break;

    default:
      break;
  }
  //rechnen wenn minimum erreicht ist
  uint32_t timePassed = currentTimeTicks - lastTimeTicks;
  if((timePassed >= REFRESH_RATE_TICKS && position != lastPosition) || (timePassed >= MAXREFRESH_RATE_TICKS)){
    //rechnen
    double angle = calcAngle(position);
    double speed = calcAngularSpeed((stepCount - lastStepCount), (currentTimeTicks - lastTimeTicks));
    //buffer füllen
    displayOutputs_printAngle(angle);
    displayOutputs_printAngularSpeed(speed);
    //variablen aktualisieren
    lastPosition = position;
    lastTimeTicks = currentTimeTicks;
    lastStepCount = stepCount;
  }
  //output
  if(dir != lastDir){
    switch (dir){
      case FWD:
        GPIOD->BSRR = (0xFFU << 16);//löscht alle LEDs
        ledOutputs_setForward();
        break;

      case REV:
        GPIOD->BSRR = (0xFFU << 16);//löscht alle LEDs
        ledOutputs_setBackward();
        break;

      case IDLE:
        GPIOD->BSRR = (0xFFU << 16);//löscht alle LEDs
        break;

        default:
          break;
    }
    lastDir = dir;
  }
  displayOutputs_printNextChar();
  }
} 