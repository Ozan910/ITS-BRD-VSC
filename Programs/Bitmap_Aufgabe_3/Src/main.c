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

#include "BMP_types.h"
#include <stdbool.h>
#include <stdint.h>
#include "LCD_GUI.h"
#include "headers.h"
#include "input.h"
#include "LCD_general.h"
#include <stdio.h>
#include "errorhandler.h"
#include <stdlib.h>
#include "helpers.h"

#define XLCDSTART 0
#define YLCDSTART 319

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	lcdPrintlnS("Waiting for connection...");
	initInput();
	GUI_clear(WHITE);
	lcdPrintlnS("connected!");
	lcdPrintlnS("start transmission");
	lcdPrintlnS("Press S0 to start printing");

	bool s0IsPressed;
	char buf[32];

	RGBQUAD* palette = malloc(256 * sizeof(RGBQUAD));
	ERR_HANDLER(palette == NULL, "malloc failed!");

	while(1){
		s0IsPressed = !(GPIOF->IDR & 1);
		if(s0IsPressed){
			//GUI_drawPoint(c, BLACK, DOT_PIXEL_1X1, DOT_FILL_AROUND);
			GUI_clear(WHITE);//Bilschirm wird geleert
			openNextFile();//nächste datei wird geöffnet
			ERR_HANDLER(readHeaders()==NOK,"fehler");//funktioniert der header zugriff?
			//initialisierung der header
			BITMAPFILEHEADER fileheader;
			BITMAPINFOHEADER infoheader;
			//lesen und speichern der header in variablen
			getFileHeader(&fileheader);
			getInfoHeader(&infoheader);

			//schreiben der nächsten 256 Byte (Farb palette) in den dafuer mit malloc reservierten speicher
			int rc = COMread( (char*)palette, sizeof(RGBQUAD), 256);//palette (feld aus RGBQuad) wird auf char* gecastet weil das eingefordert wird von COMread
			ERR_HANDLER(rc!=256, "palette failed");//wenn nicht genau 256 Byte gelesen wurden -> FEHLER!
			
			Coordinate cord;
			cord.x = XLCDSTART;
			cord.y = YLCDSTART;
			while(1){
				uint8_t byte1 = nextChar();
				uint8_t byte2 = nextChar();

				if(byte1 == 0x00){//Escape Sonderfall oder Abosulute Codierung
					if(byte2 == 0x00){//END OF LINE
						cord.x = XLCDSTART;
						cord.y--;

					}else if(byte2 == 0x01){//END OF MAP
						break;

					}else if(byte2 == 0x02){//MOVE CURSOR
						uint8_t moveX = nextChar();
						uint8_t moveY = nextChar();
						cord.x += moveX;
						cord.y -= moveY;

					}else{//ABSOLUTE MODE
						for(int i=0; i < byte2; i++){//anzahl der absoluten pixel
							uint8_t pixel = nextChar();//jeden absoluten pixel einzeln holen
							drawPixelWithPalette(palette, pixel, cord);//pixel als index für palette nutzen und printen
							cord.x++;
						}
						if(byte2 & 1){//wenn anzahl der absoulten ungerade:
							nextChar();//naechsten char ignorieren
						}
					}
				}else{//ENCODED MODE
					for(int i = 0; i < byte1; i++){
						drawPixelWithPalette(palette, byte2, cord);
						cord.x++;
					}
				}
			}
		}
	}
	free(palette);
}

// EOF