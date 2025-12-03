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

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch



	lcdPrintlnS("Waiting for connection...");
	initInput();
	GUI_clear(WHITE);
	lcdPrintlnS("connected!");

	bool s0IsPressed;
	POINT x = 10;
	POINT y = 200;
	Coordinate c;
	c.x = x;
	c.y = y;
	
	char buf[32];

	RGBQUAD* palette = malloc(256 * sizeof(RGBQUAD));
	ERR_HANDLER(palette == NULL, "malloc failed!");

	while(1){
		s0IsPressed = !(GPIOF->IDR & (1));
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

			//debug
			lcdGotoXY(1,1);
			lcdPrintlnS("Header gelesen!");

			snprintf(buf, sizeof(buf), "width: %d", infoheader.biWidth);
			lcdGotoXY(1, 2);
			lcdPrintS(buf);

			snprintf(buf, sizeof(buf), "height: %d", infoheader.biHeight);
			lcdGotoXY(1, 3);
			lcdPrintS(buf);


			//schreiben der nächsten 256 Byte (Farb palette) in den dafuer mit malloc reservierten speicher
			int rc = COMread( (char*)palette, sizeof(RGBQUAD), 256);//palette (feld aus RGBQuad) wird auf char* gecastet weil das eingefordert wird von COMread
			ERR_HANDLER(rc!=256, "palette failed");//wenn nicht genau 256 Byte gelesen wurden -> FEHLER!

			
			int pn = 0;//paletten nummer
			while(pn < 256){//test iteriert durch alle paletten farben durch, zeigt nummer, R, G, B an und einen 5x5 pixel in der Farbe nach dem umwandeln in 565
				RGBQUAD test24 = palette[pn];
				COLOR test565;
				test565 = rgb24ToRgb565(test24);//umwandeln von RGB24 zu RGB565

				GUI_drawPoint(c, test565, DOT_PIXEL_5X5, DOT_FILL_AROUND);//zeichnet den 5x5 Pixel
				
				snprintf(buf, sizeof(buf), "pn:%d R:%d G:%d B:%d", pn, test24.rgbRed, test24.rgbGreen, test24.rgbBlue);//infos zum Pixel
				pn++;
				lcdGotoXY(1,4);
				lcdPrintS(buf);

				while((GPIOF->IDR & 1));//wartet auf Tasteneingabe S0 um zum nächsten pixel zu gehen
				HAL_Delay(100);//kleiner delay, besser beim debuggen
			}

			lcdPrintlnS("FERTIG!");

			HAL_Delay(10000);
		}
	}
	free(palette);
}

// EOF
