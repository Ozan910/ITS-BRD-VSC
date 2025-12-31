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
#include "timer.h"
#include "myTimer.h"
#include "base_functions.h"
#include <stdio.h>


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer();

	// Inits für Bus
	init1WireBus();
	while(1) {
		int rc;
		rc = sendReset();

		if(!rc){
			lcdPrintlnS("Pulse Detected");
		} else {
			continue;
		}

		sendByte(0x33);//sende befehl für Read Rom [33h]
		ROM_Number rom;
		rom = receiveSingleROM();

		bool ok = checkROMCRC(&rom);
		if(ok){
			lcdPrintlnS("CRC valid");
		}else{
			lcdPrintlnS("invalid CRC");
		}

		char buf[40];
		snprintf(buf, sizeof(buf), "%02X-%02X%02X%02X%02X%02X%02X-%02X", rom.family, rom.serial[0], rom.serial[1], rom.serial[2], rom.serial[3], rom.serial[4], rom.serial[5], rom.crc);

		lcdPrintlnS(buf);

		mySleep(10000000);
	}
}
// EOF
