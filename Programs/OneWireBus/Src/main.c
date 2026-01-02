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

#define PRINTSTART 1


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer();

	// Inits für Bus
	//init1WireBus();
	char buf[40];
	initialPrint(buf, sizeof(buf));
	lcdGotoXY(0,0);
	lcdPrintlnS(buf);
	while(1) {

		TemperatureSensor sensores[2];
		uint16_t sensorCount = 2;

		{
			TemperatureSensor sensor;
			sensor.rom_number.family = 0x28;
			sensor.rom_number.serial[0] = 0x36;
			sensor.rom_number.serial[1] = 0xFB;
			sensor.rom_number.serial[2] = 0x88;
			sensor.rom_number.serial[3] = 0x0D;
			sensor.rom_number.serial[4] = 0x00;
			sensor.rom_number.serial[5] = 0x00;
			sensor.rom_number.crc = 0x42;

			sensor.isROMValid = checkROMCRC(&sensor.rom_number);

			sensores[0] = sensor;
		}
		{
			TemperatureSensor sensor;
			sensor.rom_number.family = 0x28;
			sensor.rom_number.serial[0] = 0x5A;
			sensor.rom_number.serial[1] = 0x42;
			sensor.rom_number.serial[2] = 0x88;
			sensor.rom_number.serial[3] = 0x0D;
			sensor.rom_number.serial[4] = 0x00;
			sensor.rom_number.serial[5] = 0x00;
			sensor.rom_number.crc = 0x40;

			sensor.isROMValid = checkROMCRC(&sensor.rom_number);

			sensores[1] = sensor;
		}


	
		for(int i = 0; i < sensorCount; i++){
			lcdGotoXY(0, PRINTSTART + i);
			prettyPrint(buf, sizeof(buf), &sensores[i]);
			lcdPrintlnS(buf);
		}

		mySleep(10000000);
	}

}

// EOF
