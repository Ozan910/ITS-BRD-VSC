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
#include "interrupts.h"

#define REFRESH_RATE_TICKS (250000 * TICKS_PER_US)
#define MAXREFRESH_RATE_TICKS (500000 * TICKS_PER_US)

#define EXTI01_MASKE ( (1u<<0) | (1u<<1) )

static inline uint8_t readPhase(){
  return GPIOG->IDR & 0x03U;
}

static inline void maskIRQ(){
  EXTI->IMR &= ~EXTI01_MASKE;
}

static inline void demaskIRQ(){
  EXTI->IMR |= EXTI01_MASKE;
}

int main(){
  initITSboard();
  GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

  displayOutputs_initialPrint();
  initTimer();

  volatile int32_t isr_position = 0;
  int32_t position = 0;
  int32_t lastPosition = 0;

  volatile Direction isr_dir = IDLE;
  Direction dir = IDLE;
  Direction lastDir = IDLE;

  setOldPhase(readPhase());//die ersten beiden bits des von GPIOG also GPIN0 und GPIN1

  volatile uint32_t isr_timeTicks = 0;
  uint32_t rechnerTimeTicks = 0;
  uint32_t lastRechnerTimeTicks = 0;

  uint32_t currentTimeTicks = 0;
  uint32_t lastTimeTicks = getTimeStamp();
  
  sendAdresses(&isr_dir, &isr_position, &isr_timeTicks);
  initInterrupts();
  
  while(1){//superloop

    //input


    //snapshot von den isr daten

    for(int i = 0; i < 10; i++){//check
      rechnerTimeTicks = isr_timeTicks;
      position = isr_position;
      dir = isr_dir;
      uint32_t testTimeTicks = isr_timeTicks;
      int32_t testPosition = isr_position;
      if(testTimeTicks == rechnerTimeTicks && testPosition == position){
        break;
      }
      if(i >= 9){
        dir = ERR;
      }
    }
    currentTimeTicks = getTimeStamp();

    //fehler abfangen
    if(dir == ERR){
      maskIRQ();
      ledOutputs_setError();
      displayOutputs_printError();

      waitForReset();//poll until S0 is pressed

      ledOutputs_clearError();
      displayOutputs_clearError();

      //variablen neu initialisieren
      isr_position = 0;
      position = 0;
      lastPosition = 0;

      isr_dir = IDLE;
      dir = IDLE;
      lastDir = IDLE;

      setOldPhase(readPhase());

      lastTimeTicks = getTimeStamp();
      lastRechnerTimeTicks = lastTimeTicks;

      demaskIRQ();

      continue;
    }

    //rechnen wenn minimum erreicht ist
    uint32_t timePassed = currentTimeTicks - lastTimeTicks;
    if((timePassed >= REFRESH_RATE_TICKS && position != lastPosition) || (timePassed >= MAXREFRESH_RATE_TICKS)){
      //rechnen
      double angle = calcAngle(position);
      double speed = calcAngularSpeed((position - lastPosition), (rechnerTimeTicks - lastRechnerTimeTicks));
      //buffer füllen
      displayOutputs_printAngle(angle);
      displayOutputs_printAngularSpeed(speed);
      //variablen aktualisieren
      lastPosition = position;
      lastTimeTicks = currentTimeTicks;
      lastRechnerTimeTicks = rechnerTimeTicks;
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
    displayOutputs_printNextChar();
  }
}