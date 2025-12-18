/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Hauptprogramm für BMP Viewer
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
	initITSboard();    
	
	GUI_init(DEFAULT_BRIGHTNESS);   
	TP_Init(false);                 

	lcdPrintlnS("Waiting for connection....");
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
			GUI_clear(WHITE);
			openNextFile();
			if(readHeaders()==NOK){
				ERR_HANDLER(true,"header fehler");
				continue;
			}

			
			BITMAPFILEHEADER fileheader;
			BITMAPINFOHEADER infoheader;
			getFileHeader(&fileheader);
			getInfoHeader(&infoheader);

			if(infoheader.biCompression != 1){
				ERR_HANDLER(true, "only for compressed bitmaps");
				continue;
			}

			// --- FIX 1: Palettengröße
			int colorsToRead = infoheader.biClrUsed;
			if(colorsToRead == 0) colorsToRead = 256;
			if (colorsToRead > 256) colorsToRead = 256;

			int rc = COMread( (char*)palette, sizeof(RGBQUAD), colorsToRead);
			ERR_HANDLER(rc!=colorsToRead, "palette failed");

			// --- FIX 2: Gap Skipping
			long bytesReadSoFar = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (colorsToRead * sizeof(RGBQUAD));
			long pixelStartOffset = fileheader.bfOffBits;
			long gap = pixelStartOffset - bytesReadSoFar;

			if (gap > 0) {
				for (long i = 0; i < gap; i++) {
					nextChar(); 
				}
			}

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
	//Print last line
	cord.x = XLCDSTART;
	drawLine(palette, pixelLine, colorLine, infoheader->biWidth, cord);
	cord.y--;
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

	// Globale Puffer (static, um Stack zu schonen)
	// Wir brauchen maximal 5 Zeilen Puffer für die Eingabe
	static uint8_t inputBuffer[5][2400]; // Max Breite 2400
	// Puffer für das Dekodieren einer einzelnen Zeile
	static uint8_t tempLine[2400];
	// Ausgabepuffer für eine Displayzeile (Farben)
	static uint16_t outputLine[480]; 

    // 1. Skalierungsfaktor berechnen
	float factorX = (float)infoheader->biWidth / (float)LCDWIDTH;
	float factorY = (float)infoheader->biHeight / (float)LCDHEIGHT;
    
    // Nimm den größeren Faktor (Runterskalieren)
	float factor = factorX > factorY ? factorX : factorY;
    
    // Mindestens 1.0
    if (factor < 1.0f) factor = 1.0f; 
    
    // Cast auf Integer für den Box-Algorithmus (Kantenlänge der Box)
	uint32_t scale = (uint32_t)factor;
    // Einfache Rundung: Wenn wir knapp drüber sind (z.B. 2.1), nimm lieber 3
    if ((float)scale < factor) scale++;
    
	if(scale < 1) scale = 1;
	if(scale > 5) scale = 5;

    // Breite des skalierten Bildes auf dem LCD
	uint32_t printWidth = infoheader->biWidth / scale;
    if (printWidth > LCDWIDTH) printWidth = LCDWIDTH;

	Coordinate cord;
	cord.x = XLCDSTART;
	cord.y = YLCDSTART; // Wir starten unten und malen hoch (Bottom-Up)
	
    // static Buffer (oben definiert) nutzen wir hier
	bool finishedReading = false;
	while(!finishedReading){
		uint32_t linesRead = 0;

        // SCHRITT 1: Lese 'scale' Zeilen in den Puffer 
		for(int i = 0; i < scale; i++){
			cord.x = XLCDSTART;
			bool ok = decodeNextLine(infoheader, tempLine, &cord);
			
            // Kopiere tempLine in den inputBuffer an Position i
            // inputBuffer[i] ist eine Zeile der Box
			memcpy(inputBuffer[i], tempLine, infoheader->biWidth * sizeof(uint8_t));
			
            linesRead++;
            if(!ok){
				finishedReading = true;
				break; // Datei zu Ende oder Bild zu Ende
			} 
		}
		
        if (linesRead == 0){ 
			finishedReading = true;
			break;
		}// Nichts mehr gelesen -> Fertig

		// SCHRITT 2: Box-Algorithmus (Verrechnen)
        // Wir gehen jeden Pixel der ZIEL-Zeile durch
        for(int lcdX = 0; lcdX < printWidth; lcdX++) {
            
            long r_sum = 0, g_sum = 0, b_sum = 0;
            int count = 0;

            // Berechne den Bereich im Quellbild (Box X-Koordinaten)
            int startX = lcdX * scale;
            int endX = startX + scale;
            if (endX > infoheader->biWidth) endX = infoheader->biWidth;

            // Iteriere über die geladenen Zeilen (Y)
            for(int y = 0; y < linesRead; y++) {
                // Iteriere über die Pixel der Zeile (X)
                for(int x = startX; x < endX; x++) {
                    uint8_t index = inputBuffer[y][x];
                    RGBQUAD c = palette[index];
                    
                    r_sum += c.rgbRed;
                    g_sum += c.rgbGreen;
                    b_sum += c.rgbBlue;
                    count++;
                }
            }

            // Durchschnitt berechnen
            if (count > 0) {
                RGBQUAD avg;
                avg.rgbRed = r_sum / count;
                avg.rgbGreen = g_sum / count;
                avg.rgbBlue = b_sum / count;
                
                // In 565 konvertieren und in Ausgabepuffer schreiben
                outputLine[lcdX] = rgb24ToRgb565(avg);
            } else {
                outputLine[lcdX] = 0; // Schwarz
            }
        }

        // SCHRITT 3: Zeile ausgeben
        GUI_WriteLine((Coordinate){0, cord.y}, printWidth, outputLine);
        
        // Eine Zeile auf dem Display weiter nach oben
        cord.y--;

		if (cord.y < 0) finishedReading = true;
	}
}
// EOF