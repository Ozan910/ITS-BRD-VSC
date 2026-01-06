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
#include "input.h"
#include "LCD_GUI.h"
#include <string.h>

#define SEARCH_ROM 		0xF0
#define READ_ROM 		0x33
#define MATCH_ROM 		0x55
#define SKIP_ROM 		0xCC
#define CONVERT_T		0x44
#define READ_SCRATCHPAD 0xBE

#define PRINTSTART 1
#define MAX_SENSORS 8
#define STRING_BUFFERSIZE 40


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer();

	// Inits für Bus
	init1WireBus();
	char buf[STRING_BUFFERSIZE];
	static char printedLines[MAX_SENSORS][STRING_BUFFERSIZE];

	while(1) {
		GUI_clear(WHITE);
		memset(printedLines, ' ', sizeof(printedLines));
		initialPrint(buf, sizeof(buf));//AnfgangsPrint für die tabelle
		lcdGotoXY(0,0);
		lcdPrintlnS(buf);

		TemperatureSensor sensors[MAX_SENSORS];//Automatische Erkennung der Sensoren durch SearchROM 🤯 
		uint16_t sensorCount = 0;

		int rc = findAllROMS(sensors, MAX_SENSORS, &sensorCount);
		if(rc == -1){
			lcdGotoXY(0,16);
			lcdPrintlnS("KEINE SENSOREN ERKANNT!");
		}else if(rc < 0){
			lcdGotoXY(0,17);
			lcdPrintlnS("Unerwarteter Fehler aufgetreten");
		}


		while(!inputs_isS0Pressed()){//Zyklische Abfrage der Temperaturen und Print findet hier statt. Auf button press wird der ganze loop ausgeführt um neue sensoren zu erkennen

			

			if(!sendReset()){//wenn min 1 sensor erkannt wird: hier alle auf einmal messen lassen
				sendByte(SKIP_ROM);
				sendByte(CONVERT_T);
				powerSupply750ms();
			}

			//hier beginnt zyklische abfrage
			for(int i = 0; i < sensorCount; i++){
				/*
				if(sendReset()){//0 wenn puls erkannt: also hier rein wenn keine connection hier einzel messungen
					continue;
				}
				sendByte(MATCH_ROM);
				sendROMCode(&sensors[i].rom_number);//sende 8 byte rom code lsb nach msb
				sendByte(CONVERT_T);

				powerSupply750ms();//750ms 3.3 V
				*/

				if(sendReset()){//0 wenn puls erkannt: also hier rein wenn keine connection
					for(int i = 0; i < sensorCount; i++){
						sensors[i].isScratchpadValid = false;
					}
				}else{
					sendByte(MATCH_ROM);
					sendROMCode(&sensors[i].rom_number);
					sendByte(READ_SCRATCHPAD);

					Scratchpad scratchpad;
					receiveScratchpad(&scratchpad);
					if(checkScratchCRC(&scratchpad)){
						sensors[i].scratchpad = scratchpad;
						sensors[i].isScratchpadValid = true;
					}else {
						sensors[i].isScratchpadValid = false;
					}
				}
				
				lcdGotoXY(0, PRINTSTART + i);
				prettyPrint(buf, sizeof(buf), &sensors[i]);
				for(int j = 0; j < STRING_BUFFERSIZE; j++){
					char currentChar = buf[j];
					if(currentChar == '\0'){
						currentChar = ' ';
					}
					if(currentChar != printedLines[i][j]){
						lcdGotoXY(j, i+1);
						lcdPrintC(currentChar);
						printedLines[i][j] = currentChar;
					}
				}

			}
			//mySleep(10000000);//10s delay für debug zwecke
		}
		
		lcdGotoXY(5, 15);
		lcdPrintlnS("break hat geklappt");//für debug zwecke check ob bei S0 rausspringt.
		//mySleep(5000000);
		
		
	}
}

// EOF