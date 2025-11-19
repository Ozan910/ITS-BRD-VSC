/** 
* @file displayOutputs.h
* @brief Ausgabe von Winkel, Winkelgeschw. und Fehlern auf dem LCD
*/
#ifndef DISPLAYOUTPUTS_H
#define DISPLAYOUTPUTS_H


/**
* @brief Gibt statische Texte aus 
*/
void displayOutputs_initialPrint(void);


/** 
* @brief Aktualisiert die Anzeige des aktuellen Drehwinkels
*
* @param angle Drehwinkel in Grad
*/
void displayOutputs_printAngle(double angle);


/** 
* @brief Aktualisiert die Anzeige der aktuellen Winkelgeschwindigkeit
*
* @param speed Winkelgeschwindigkeit in Grad pro Sekunde
*/
void displayOutputs_printAngularSpeed(double speed);


/** 
* @brief Zeigt den aktuellen Fehlerzustand auf dem LCD an
*
* @param errorCode Fehlercode (siehe fehler.h)
*/
void displayOutputs_printError(int errorCode);


/**
* @brief clears the display
*/
void displayOutputs_clearError(void);

/**
* @brief Gibt pro Aufruf ein Zeichen auf dem Display aus (erst Winkel dann Speed)
*
*/
void displayOutputs_printNextChar(void);

#endif
//EOF