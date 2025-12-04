#include "bmp_logic.h"
#include "LCD_GUI.h"
#include "input.h"       
#include "helpers.h"     
#include <stdbool.h>
#include <stdio.h>    

#define XLCDSTART 0
#define YLCDSTART 319

// --- SPEICHER ---
// Puffer für 5 Zeilen Input (Speichert Paletten-Indizes)
// Wir nehmen 2400 Byte an, falls das Bild breiter ist als 480 Pixel (Sicherheit)
static uint8_t inputRows[5][2400]; 

// Puffer für 1 Zeile Output (Speichert fertige RGB565 Farben für LCD)
static COLOR outputRow[480];


// -------------------------------------------------------------------------
// Hilfsfunktion: Laden einer Zeile
// -------------------------------------------------------------------------
// Diese Funktion holt genau EINE Zeile aus der Datei, dekodiert die RLE-Daten
// und speichert die Pixel-Indizes in den Puffer.
static bool loadNextBMPRow(uint8_t* targetBuffer, int width) {
    int x = 0; // Aktuelle X-Position im Puffer
    
    // Puffer leeren (Sicherheit)
    for(int i=0; i<width; i++) targetBuffer[i] = 0;

    // --- RLE Dekodierung ---
    while (x < width) {

			uint8_t pixelLine[width];

			while(1){
				uint8_t byte1 = nextChar();
				uint8_t byte2 = nextChar();

				if(byte1 == 0x00){//Escape Sonderfall oder Abosulute Codierung
					if(byte2 == 0x00){//END OF LINE
                        break;

					}else if(byte2 == 0x01){//END OF MAP
                        return true;

					}else if(byte2 == 0x02){//MOVE CURSOR
						uint8_t moveX = nextChar();
                        nextChar();//y ignorieren
						x += moveX;

					}else{//ABSOLUTE MODE
						for(int i=0; i < byte2; i++){//anzahl der absoluten pixel
							uint8_t pixel = nextChar();//jeden absoluten pixel einzeln holen
							targetBuffer[x] = pixel;
							x++;
						}
						if(byte2 & 1){//wenn anzahl der absoulten ungerade:
							nextChar();//naechsten char ignorieren
						}
					}
				}else{//ENCODED MODE
					for(int i = 0; i < byte1; i++){
						//drawPixelWithPalette(palette, byte2, cord);
						targetBuffer[x] = byte2;
						x++;
					}
				}
			}
    return false; 
    }
    return false;
}
// -------------------------------------------------------------------------
// Hauptfunktion: Skalierung und Ausgabe
// -------------------------------------------------------------------------
void drawScaledImage(BITMAPINFOHEADER* info, RGBQUAD* palette) {
    int srcWidth = info->biWidth;
    int srcHeight = info->biHeight;

    // 1. Skalierungsfaktor berechnen
    float scaleX = (float)srcWidth / 480.0f;
    float scaleY = (float)srcHeight / 320.0f;
    float scale = (scaleX > scaleY) ? scaleX : scaleY; // Seitenverhältnis beibehalten
    
    if (scale < 1.0f) scale = 1.0f; // Keine Vergrößerung, nur Verkleinerung

    int linesReadFromFile = 0;
    bool endOfFileReached = false;

    // 2. Über alle Display-Zeilen iterieren (0 bis 319)
    for (int lcdY = 0; lcdY < 320; lcdY++) {
        
        // Bereich im Originalbild berechnen, der dieser Display-Zeile entspricht
        int startY_src = (int)(lcdY * scale);
        int endY_src   = (int)((lcdY + 1) * scale);
        if (endY_src >= srcHeight) endY_src = srcHeight - 1;

        // 3. Puffer füllen: Nötige Zeilen aus der Datei nachladen
        while (linesReadFromFile <= endY_src && !endOfFileReached) {
            // Wir nutzen Modulo 5 für den Ringpuffer (nur 5 Zeilen im RAM halten)
            endOfFileReached = loadNextBMPRow(inputRows[linesReadFromFile % 5], srcWidth);
            linesReadFromFile++;
        }

        // 4. Mittelwert berechnen (Box-Algorithmus)
        for (int lcdX = 0; lcdX < 480; lcdX++) {
            
            // X-Bereich im Originalbild berechnen
            int startX_src = (int)(lcdX * scale);
            int endX_src   = (int)((lcdX + 1) * scale);
            if (endX_src >= srcWidth) endX_src = srcWidth - 1;

            long r_sum = 0, g_sum = 0, b_sum = 0;
            int count = 0;

            // Box durchlaufen und Farbwerte summieren
            for (int y = startY_src; y < endY_src; y++) {
                for (int x = startX_src; x < endX_src; x++) {
                    // Zugriff auf den Puffer
                    uint8_t idx = inputRows[y % 5][x];
                    RGBQUAD c = palette[idx]; 
                    
                    r_sum += c.rgbRed;
                    g_sum += c.rgbGreen;
                    b_sum += c.rgbBlue;
                    count++;
                }
            }

            // Durchschnitt berechnen und konvertieren
            if (count > 0) {
                RGBQUAD avg;
                avg.rgbRed = r_sum / count;
                avg.rgbGreen = g_sum / count;
                avg.rgbBlue = b_sum / count;
                outputRow[lcdX] = rgb24ToRgb565(avg);
            } else {
                // Fallback: Nimm den ersten Pixel der Box
                uint8_t idx = inputRows[startY_src % 5][startX_src];
                outputRow[lcdX] = rgb24ToRgb565(palette[idx]);
            }
        }

        // 5. Zeile ausgeben
        Coordinate start;
        start.x = 0;
       
        start.y = 319 - lcdY; 
        
        GUI_WriteLine(start, 480, outputRow);
    }


}   
    void drawWithoutChecks(BITMAPINFOHEADER infoheader, RGBQUAD* palette){
			Coordinate cord;
			cord.x = XLCDSTART;
			cord.y = YLCDSTART;

			uint8_t pixelLine[infoheader.biWidth];
			uint16_t colorLine[infoheader.biWidth];

			while(1){
				uint8_t byte1 = nextChar();
				uint8_t byte2 = nextChar();

				if(byte1 == 0x00){//Escape Sonderfall oder Abosulute Codierung
					if(byte2 == 0x00){//END OF LINE
						cord.x = XLCDSTART;
						drawLine(palette, pixelLine, colorLine, infoheader.biWidth, cord);
						cord.y--;

					}else if(byte2 == 0x01){//END OF MAP
						break;

					}else if(byte2 == 0x02){//MOVE CURSOR
						uint8_t moveX = nextChar();
						uint8_t moveY = nextChar();
						cord.x += moveX;
						//cord.y -= moveY;

					}else{//ABSOLUTE MODE
						for(int i=0; i < byte2; i++){//anzahl der absoluten pixel
							uint8_t pixel = nextChar();//jeden absoluten pixel einzeln holen
							//drawPixelWithPalette(palette, pixel, cord);//pixel als index für palette nutzen und printen
							pixelLine[cord.x] = pixel;
							cord.x++;
						}
						if(byte2 & 1){//wenn anzahl der absoulten ungerade:
							nextChar();//naechsten char ignorieren
						}
					}
				}else{//ENCODED MODE
					for(int i = 0; i < byte1; i++){
						//drawPixelWithPalette(palette, byte2, cord);
						pixelLine[cord.x] = byte2;
						cord.x++;
					}
				}
			}
    }
