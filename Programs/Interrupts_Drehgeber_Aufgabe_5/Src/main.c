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
  uint32_t currentTimeTicks = 0;
  Direction dir = IDLE;
  Direction lastDir = IDLE;
  Phase lastPhase = readPhase();
  Phase phase;
  setOldPhase(lastPhase);//die ersten beiden bits des von GPIOF also FPIN0 und FPIN1
  uint32_t lastTimeTicks = getTimeStamp();
  
  
  while(1){//superloop
  //fehler abfangen
  if(dir == ERR){
    ledOutputs_setError();
    displayOutputs_printError();

    waitForReset();

    ledOutputs_clearError();
    displayOutputs_clearError();

    dir = IDLE;
    lastDir = IDLE;
    lastPosition = 0;
    position = 0;
    lastPhase = readPhase();
    setOldPhase(lastPhase);
    lastTimeTicks = getTimeStamp();
    continue;
  }

  //input
  phase = readPhase();
  dir = getDirection(phase);
  currentTimeTicks = getTimeStamp();

  //auf state reagieren
  switch (dir){
    case FWD:
      position++;
      break;

    case REV:
      position--;
      break;

    default:
      break;
  }
  //rechnen wenn minimum erreicht ist
  uint32_t timePassed = currentTimeTicks - lastTimeTicks;
  if((timePassed >= REFRESH_RATE_TICKS && phase != lastPhase) || (timePassed >= MAXREFRESH_RATE_TICKS)){
    //rechnen
    double angle = calcAngle(position);
    double speed = calcAngularSpeed((position - lastPosition), (currentTimeTicks - lastTimeTicks));
    //buffer füllen
    displayOutputs_printAngle(angle);
    displayOutputs_printAngularSpeed(speed);
    //variablen aktualisieren
    lastPosition = position;
    lastTimeTicks = currentTimeTicks;
  }
  //output
  ledOutputs_setStepCount(position);
  if(dir != lastDir){
    switch (dir){
      case FWD:
        GPIOE->BSRR = (0xFFU << 16);//löscht alle LEDs
        ledOutputs_setForward();
        break;

      case REV:
        GPIOE->BSRR = (0xFFU << 16);//löscht alle LEDs
        ledOutputs_setBackward();
        break;
        
        default:
          break;
    }
    lastDir = dir;
  }
  lastPhase = phase;
  displayOutputs_printNextChar();
  }
} 