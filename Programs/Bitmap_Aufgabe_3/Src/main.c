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
			GUI_clear(WHITE);
			openNextFile();
			ERR_HANDLER(readHeaders()==NOK,"fehler");

			BITMAPFILEHEADER fileheader;
			BITMAPINFOHEADER infoheader;

			getFileHeader(&fileheader);
			getInfoHeader(&infoheader);

			lcdGotoXY(1,1);
			lcdPrintlnS("Header gelesen!");

			snprintf(buf, sizeof(buf), "width: %d", infoheader.biWidth);
			lcdGotoXY(1, 2);
			lcdPrintS(buf);

			snprintf(buf, sizeof(buf), "height: %d", infoheader.biHeight);
			lcdGotoXY(1, 3);
			lcdPrintS(buf);

			int rc = COMread( (char*)palette, sizeof(RGBQUAD), 256);
			ERR_HANDLER(rc!=256, "palette failed");

			
			int pn = 0;
			while(pn < 256){
				RGBQUAD test24 = palette[pn];
				COLOR test565;
				test565 = rgb24ToRgb565(test24);

				GUI_drawPoint(c, test565, DOT_PIXEL_5X5, DOT_FILL_AROUND);
				
				snprintf(buf, sizeof(buf), "pn:%d R:%d G:%d B:%d", pn, test24.rgbRed, test24.rgbGreen, test24.rgbBlue);
				pn++;
				lcdGotoXY(1,4);
				lcdPrintS(buf);

				while((GPIOF->IDR & 1));
				HAL_Delay(100);
			}

			lcdPrintlnS("FERTIG!");

			HAL_Delay(10000);
		}
	}
	free(palette);
}

// EOF
