#ifndef _DISPLAY_H
#define _DISPLAY_H
/**
 ******************************************************************************
 * @file    display.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Interface des Moduls zur Ausgabe der W�rfel auf dem LCD Display. 
 ******************************************************************************
 */
#include "LCD_GUI.h"

// Farben der W�rfel
#define DIE_COLOR_FIXED          MAGENTA
#define DIE_COLOR_WIN            YELLOW
#define DIE_COLOR_DEFAULT        GREEN     
#define BACKGROUND_DEFAULT       BLUE

/**
  * @brief Diese Funktion f�llt den ganzen Bildschirm mit blau.
  */
void clearDisplay(void);

/**
  * @brief Diese Funktion zeichnet einen W�rfel auf dem Bildschirm.
  *
  * @param  die Die Nummer des W�rfels, die die Position auf dem Bildschirm 
	*             festlegt. 0 ist der W�rfel rechts au�en. 4 ist der W�rfel
	*             links au�en.
	* @param  val Der Wert, der auf dem W�rfel dargestellt wird.
  * @param  c   Die Farbe des W�rfels.
  */
void displayDie(uint8_t die, uint8_t value, COLOR c);

#endif
// EOF
