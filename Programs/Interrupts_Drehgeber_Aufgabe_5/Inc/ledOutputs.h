/** 
* @file ledOutputs.h
* @brief Steuert die LEDs für Richtung, Fehler und Schrittzähler
*/
#ifndef LEDOUTPUTS_H
#define LEDOUTPUTS_H
#include <stdint.h>


/** 
* @brief Schaltet die LED für Vorwärtslauf ein (D23).
*/
void ledOutputs_setForward(void);


/** 
* @brief Schaltet die LED für Vorwärtslauf aus (D23).
*/
void ledOutputs_clearForward(void);


/** 
* @brief Schaltet die LED für Rückwärtslauf ein (D22).
*/
void ledOutputs_setBackward(void);


/** 
* @brief Schaltet die LED für Rückwärtslauf aus (D22).
*/
void ledOutputs_clearBackward(void);


/** 
* @brief Schaltet die LED für Fehler ein (D21).
*/
void ledOutputs_setError(void);


/** 
* @brief Schaltet die LED für Fehler aus (D21).
*/
void ledOutputs_clearError(void);


/** 
* @brief Gibt die gegeben Zahl (unsigned Integer 8 bit) binär auf den LEDs D8-D15 aus
*
* @param count Schrittzähler als 8 Bit-Zahl
*/
void ledOutputs_setStepCount(uint8_t count);


#endif
//EOF