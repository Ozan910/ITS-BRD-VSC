/**
* @file inputs.h
* @brief Liest die aktuelle Phase des Drehgebers und den Taster S6
*/
#ifndef INPUTS_H
#define INPUTS_H
#include <stdbool.h>

/**
* @enum Phase
* @brief Die vier möglichen Phasen des Drehgebers (a, b, c, d)
*/
typedef enum {
    PHASE_A,// 00
    PHASE_D,// 01
    PHASE_B,// 10
    PHASE_C// 11
} Phase;

/** 
* @brief Liest die Eingangspins IN0 und IN1 und bestimmt die aktuelle Phase des Drehgebers
*
* @return Phase  Eine der vier Phasen PHASE_A, PHASE_B, PHASE_C, PHASE_D
*/
Phase inputs_newPhase(void);

/**
* @brief Prüft, ob der Taster S6 aktuell gedrückt ist.
* 
* @return true  Wenn der Taster S6 gedrückt ist
* @return false  Wenn der Taster S6 nicht gedrückt ist
*/
bool inputs_isS6Pressed(void);

#endif
//EOF