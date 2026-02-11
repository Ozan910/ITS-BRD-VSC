#ifndef _KNIFFEL_H
#define _KNIFFEL_H

/**
 ******************************************************************************
 * @file    kniffel.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    20. Jan. 2025
 * @brief   Interface des Moduls zum Handling der W�rfel.
 ******************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#define MAX_DICE 5

typedef enum {
  RELEASED, WIN, FIXED 
} DiceState;

typedef struct {
  uint8_t value;
  DiceState state;
} KniffelDice;

extern KniffelDice wuerfel[MAX_DICE];


/**
  * @brief Diese Funktion liefert den aktuellen Wert eines W�rfels.
  *
  * @param  die Der W�rfel, dessen Wert bestimmt wird.
  *	
  * @retval Der Wert des W�rfels.
  */
uint8_t getDie(uint8_t die);

/**
  * @brief Diese Funktion �berpr�ft, ob alle W�rfel den Wert 1 haben - also
	*        ein Kniffel vorliegt.
  *
  * @retval true gdw. ein Kniffel liegt vor.
  */
bool isKniffel(void);

/**
  * @brief Diese Funktion �berpr�ft, ob die Werte der W�rfel eine Strasse ergeben
  *        also eintweder 1 2 3 4 5 oder 2 3 4 5 6 sind.
  *
  * @retval true gdw. eine Strasse liegt vor.
  */
bool isStreet(void);

/**
  * @brief Diese Funktion fixiert den W�rfel mit der Nummer die. Ein fixierter
	*        W�rfel wird beim n�chsten Wurf nicht geworfen / gew�rfelt.
  *
  * @param  die Die Nummer des W�rfels, der fixiert wird.
  */
void fixDie(uint8_t die);

/**
  * @brief Diese Funktion l�st die Fixierung aller W�rfel auf, so dass alle 
	*        W�rfel beim n�chsten Wurf mit gew�rfelt werden.
  */
void releaseDice(void);

/**
  * @brief Diese Funktion �berpr�ft, ob der W�rfel mit der Nummer die fixiert ist.
  *
  * @param  die Die Nummer des W�rfels, der �berpr�ft wird.
  *	
  * @retval true gdw. der W�rfel mit der Nummer die ist fixiert.
  */
bool isFixed(uint8_t die);

/**
  * @brief Diese Funktion w�rfelt die W�rfel, die nicht fixiert sind. Somit
	*        erhalten diese W�rfel einen neuen Wert.
  */
void rollDice(void);

#endif
// EOF
