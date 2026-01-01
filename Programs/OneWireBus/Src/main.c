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
#include "crc.h"
#include "sensors.h"

#define SEARCH_ROM 		0xF0
#define READ_ROM 		0x33
#define MATCH_ROM 		0x55
#define SKIP_ROM 		0xCC
#define CONVERT_T		0x44
#define READ_SCRATCHPAD 0xBE


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer();

	// Inits für Bus
	init1WireBus();
	while(1) {
		if(!sendReset()){
			lcdPrintlnS("Pulse Detected");
		} else {
			continue;
		}

		sendByte(READ_ROM);//sende befehl für Read Rom [33h]
		ROM_Number rom;
		receiveSingleROM(&rom);

		if(checkROMCRC(&rom)){
			lcdPrintlnS("CRC valid");
		}else{
			lcdPrintlnS("invalid CRC");
		}

		char buf[50];
		snprintf(buf, sizeof(buf), "%02X-%02X%02X%02X%02X%02X%02X-%02X", rom.family, rom.serial[0], rom.serial[1], rom.serial[2], rom.serial[3], rom.serial[4], rom.serial[5], rom.crc);

		lcdPrintlnS(buf);

		if(!sendReset()){
			lcdPrintlnS("Pulse Detected");
		} else {
			continue;
		}
		sendByte(SKIP_ROM);
		sendByte(CONVERT_T);
		powerSupply750ms();

		if(!sendReset()){
				lcdPrintlnS("Pulse Detected");
			} else {
				continue;
			}
		sendByte(SKIP_ROM);
		sendByte(READ_SCRATCHPAD);
		Scratchpad scratchpad;
		receiveScratchpad(&scratchpad);
		float temp = getFloatTemp(scratchpad.temperature);
		snprintf(buf, sizeof(buf), "Temperature: %f", temp);
		lcdPrintlnS(buf);

		mySleep(10000000);
		mySleep(10000000);
		mySleep(10000000);
		mySleep(10000000);
	}
}
// EOF
