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

#include "myerr.h"
#include "myDisp.h"
#include "timer.h"
#include "input.h"
#include <stdint.h>
#include "myTimer.h"

#define NUM_OP 4

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

  	// Begruessungstext	
	initTimer();
	initDisp();

	// Test in Endlosschleife
	uint8_t buttonInput = 0xFF;
	uint8_t leftOp = 0;
	uint8_t rightOp = 0;
	static const char operationArray[NUM_OP] = {'+', '-', '*', '/'};
	uint8_t currentOpIdx = 0;
	uint8_t calcFlag = 0;
	uint8_t errFlag = 0;
	int8_t result = 0;
	uint8_t test = 0;
	while(1) {
		//input
		buttonInput = ~getDebouncedButtons();
		if((buttonInput == 0x00) && (getError() != EOK)){
			errFlag = 1;
		}
		//berechnen
		if(!(buttonInput & 1)){//configure
			test = ((buttonInput >> 5) & 7);
			leftOp ^= test;
			rightOp ^= ((buttonInput >> 2) & 7);
			if(buttonInput & (1U << 1)){
				currentOpIdx++;
				currentOpIdx %= NUM_OP;
			}
		}else{//calculate
			calcFlag = 1;
			switch (operationArray[currentOpIdx]){
				case '+':
					result = leftOp + rightOp;
					break;

				case '-':
					result = leftOp - rightOp;
					break;

				case '*':
					result = leftOp * rightOp;
					break;

				case '/':
					if(rightOp == 0){
						errFlag = 1;
						setError(DIV_ERR);
					}else{
						result = leftOp / rightOp;
					}
					break;

				default:
						errFlag = 1;
						setError(INTERNAL_ERR);
					break;
			}
		}
		//output
		if(errFlag){
			printError();
			leftOp = 0;
			rightOp = 0;
			currentOpIdx = 0;
			druckeOp(leftOp, operationArray[currentOpIdx], rightOp);
			errFlag = 0;
			setError(EOK);
		}else if(calcFlag){
			calcFlag = 0;
			clearError();
			druckeOpMitErg(operationArray[currentOpIdx], leftOp, rightOp, result);
		}else{
			clearError();
			druckeOp(operationArray[currentOpIdx], leftOp, rightOp);
		}
		mySleep(200000 * TICKS_PER_US);
	}
}

// EOF
