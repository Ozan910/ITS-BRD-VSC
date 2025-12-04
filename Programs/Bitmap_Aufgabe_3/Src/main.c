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
#include "main.h"
#include <string.h>

#define LCDWIDTH 480
#define LCDHEIGHT 320

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
	ERR_HANDLER(palette == NULL, "malloc palette failed!");

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

			if((infoheader.biWidth <= 480) && (infoheader.biHeight <= 320)){
				drawOriginalSize(&infoheader, palette);
			}else if((infoheader.biWidth <= (480 * 5)) && (infoheader.biHeight <= (320 * 5))){
				scaleAndPrint(&infoheader, palette);
			}else{
				ERR_HANDLER(true, "bitmap too big");
			}
		}
	}
	free(palette);
}
	

void drawOriginalSize(const BITMAPINFOHEADER* infoheader, const RGBQUAD* palette){
	Coordinate cord;
	cord.x = XLCDSTART;
	cord.y = YLCDSTART;

	uint8_t pixelLine[infoheader->biWidth];
	uint16_t colorLine[infoheader->biWidth];

	while(decodeNextLine(infoheader, pixelLine, &cord)){
		cord.x = XLCDSTART;
		drawLine(palette, pixelLine, colorLine, infoheader->biWidth, cord);
		cord.y--;
	}
}

bool decodeNextLine(const BITMAPINFOHEADER* infoheader, uint8_t* pixelLine, Coordinate* cord){
		while(1){
		uint8_t byte1 = nextChar();
		uint8_t byte2 = nextChar();

		if(byte1 == 0x00){//Escape Sonderfall oder Abosulute Codierung
			if(byte2 == 0x00){//END OF LINE
				return true;


			}else if(byte2 == 0x01){//END OF MAP
				return false;

			}else if(byte2 == 0x02){//MOVE CURSOR
				uint8_t moveX = nextChar();
				uint8_t moveY = nextChar();
				cord->x += moveX;
				cord->y -= moveY;

			}else{//ABSOLUTE MODE
				for(int i=0; i < byte2; i++){//anzahl der absoluten pixel
					uint8_t pixel = nextChar();//jeden absoluten pixel einzeln holen
					//drawPixelWithPalette(palette, pixel, cord);//pixel als index für palette nutzen und printen
					pixelLine[cord->x] = pixel;
					cord->x++;
				}
				if(byte2 & 1){//wenn anzahl der absoulten ungerade:
					nextChar();//naechsten char ignorieren
				}
			}
		}else{//ENCODED MODE
			for(int i = 0; i < byte1; i++){
				//drawPixelWithPalette(palette, byte2, cord);
				pixelLine[cord->x] = byte2;
				cord->x++;
			}
		}
	}
}

void scaleAndPrint(const BITMAPINFOHEADER* infoheader, const RGBQUAD* palette){
	float factorX = (float)LCDWIDTH / (float)infoheader->biWidth;
	float factorY = (float)LCDHEIGHT / (float)infoheader->biHeight;
	float factor = factorX < factorY ? factorX : factorY;
	factor = 1.0f/factor;
	uint32_t scale = (uint32_t)factor;
	if(scale < factor) scale++;
	if(scale < 1) scale = 1;
	if(scale > 5) scale = 5;


	uint32_t printWidth = infoheader->biWidth / scale;
	Coordinate cord;
	cord.x = XLCDSTART;
	cord.y = YLCDSTART;
	uint8_t pixelLine[infoheader->biWidth];
	uint16_t colorLine[printWidth];
	uint8_t buffer[5][infoheader->biWidth];

	while(1){
		uint32_t linesRead = 0;

		for(int i = 0; i < scale; i++){
			cord.x = XLCDSTART;
			bool ok = decodeNextLine(infoheader, pixelLine, &cord);
			if(!ok) break;
			memcpy(buffer[i], pixelLine, infoheader->biWidth * sizeof(uint8_t));
			linesRead++;
		}
		if (linesRead == 0) break;

		for(uint32_t x = 0; x < printWidth; x++){

			uint32_t sumRed = 0;
			uint32_t sumGreen = 0;
			uint32_t sumBlue = 0;
			uint32_t count = 0;

			for(uint32_t y = 0; y < linesRead; y++){
				for(uint32_t k = 0; k < scale; k++){

					uint32_t indexX =  x * scale + k;
					if(indexX >= infoheader->biWidth) break;

					uint8_t pixelIndex = buffer[y][indexX];
					RGBQUAD col = palette[pixelIndex];

					sumRed += col.rgbRed;
					sumGreen += col.rgbGreen;
					sumBlue += col.rgbBlue;
					count++;
				}
			}
			if(count == 0){
				colorLine[x] = 0;
			} else {
				RGBQUAD col;
				col.rgbRed = sumRed / count;
				col.rgbGreen = sumGreen / count;
				col.rgbBlue = sumBlue / count;
				colorLine[x] = rgb24ToRgb565(col);
			}
			
		}

		cord.x = XLCDSTART;
		GUI_WriteLine(cord, printWidth, colorLine);
		cord.y--;

		if(linesRead < scale){
			break;
		}
	
	}
}


// EOF