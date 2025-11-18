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
#include "displayOutputs.h"
#include "ledOutputs.h"
#include "inputs.h"
#include "fehler.h"


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	
	displayOutputs_initialPrint();
	double i = 0.0;

	while(1) {
		if(inputs_isS6Pressed()){
			displayOutputs_printAngle(i);
			displayOutputs_printAngularSpeed(i);
			i++;
		}
	}
}

// EOF
