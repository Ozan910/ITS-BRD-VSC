/**
 ******************************************************************************
 * @file    display.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Dieses Modul implementiert die LCD Ausgabe der W�rfel.
 ******************************************************************************
 */

#include "display.h"
#include "main.h"
#include "LCD_GUI.h"
#include "error.h"
// Dieses Feld speichert die Positionen der W�rfel. Die Position eines W�rfels
// ist durch seine linke obere und rechte untere Ecke beschrieben.

const struct {
	 Coordinate tl;
	 Coordinate br;
	} dicePosData[NO_DICE] = { {.tl = {.x = 382, .y = 40}, .br = {.x = 465, .y = 123}},
		                         {.tl = {.x = 292, .y = 40}, .br = {.x = 375, .y = 123}},
														 {.tl = {.x = 202, .y = 40}, .br = {.x = 285, .y = 123}},
														 {.tl = {.x = 112, .y = 40}, .br = {.x = 195, .y = 123}},
														 {.tl = {.x = 22,  .y = 40}, .br = {.x = 105, .y = 123}}};

// Von der linken oberen Ecke eines W�rfels ausgehend, beschreiben folgende Konstanten 
// den Offset der Mittelpunkte der einzelnen Positionen der W�rfelaugen / W�rfelpunkte.
//
//           |-------------------------|
//           |                         |
//           |  Pos1             Pos2  |
//           |                         |
//           |  Pos3    Pos4     Pos5  |
//           |                         |
//           |  Pos6             Pos7  |
//           |                         |
//           |-------------------------|
											
const Coordinate offsetPos1 = {.x = 1 * 21, .y = 1 * 21};
const Coordinate offsetPos2 = {.x = 3 * 21, .y = 1 * 21};
const Coordinate offsetPos3 = {.x = 1 * 21, .y = 2 * 21};
const Coordinate offsetPos4 = {.x = 2 * 21, .y = 2 * 21};
const Coordinate offsetPos5 = {.x = 3 * 21, .y = 2 * 21};
const Coordinate offsetPos6 = {.x = 1 * 21, .y = 3 * 21};
const Coordinate offsetPos7 = {.x = 3 * 21, .y = 3 * 21};

const Coordinate val1[1] = {offsetPos4};
const Coordinate val2[2] = {offsetPos6, offsetPos2};
const Coordinate val3[3] = {offsetPos6, offsetPos4, offsetPos2};
const Coordinate val4[4] = {offsetPos1, offsetPos2, offsetPos6, offsetPos7};
const Coordinate val5[5] = {offsetPos1, offsetPos2, offsetPos6, offsetPos7, offsetPos4};
const Coordinate val6[6] = {offsetPos1, offsetPos2, offsetPos3, offsetPos5, offsetPos6, offsetPos7};

#define RADIUS_DICE_EYE       6   // 

void clearDisplay(void){
	GUI_clear(BACKGROUND_DEFAULT);
}

void displayDie(uint8_t die, uint8_t value, COLOR c){
	if(die > 5 || value > 6 || value < 1){
		Error_Handler();
	}
	Coordinate diceTL = dicePosData[die].tl;
	Coordinate diceBR = dicePosData[die].br;
	GUI_drawRectangle(diceTL, diceBR, BACKGROUND_DEFAULT, true, DOT_PIXEL_1X1);
	GUI_drawRectangle(diceTL, diceBR, c, false, DOT_PIXEL_1X1);

	Coordinate* diceValues = 0;
	switch (value){
		case 1: 
			diceValues = (Coordinate *) val1;
			break;
		case 2: 
			diceValues = (Coordinate *) val2;
			break;
		case 3: 
			diceValues = (Coordinate *) val3;
			break;
		case 4: 
			diceValues = (Coordinate *) val4;
			break;
		case 5: 
			diceValues = (Coordinate *) val5;
			break;
		case 6: 
			diceValues = (Coordinate *) val6;
			break;
	}
	if (diceValues == 0) return;

	for(int i = 0; i < value; i++){
		Coordinate currentValCen;
		currentValCen.x = dicePosData[die].tl.x + diceValues[i].x;
		currentValCen.y = dicePosData[die].tl.y + diceValues[i].y;
		GUI_drawCircle(currentValCen, RADIUS_DICE_EYE, c, true, DOT_PIXEL_1X1);
	}
}


// EOF