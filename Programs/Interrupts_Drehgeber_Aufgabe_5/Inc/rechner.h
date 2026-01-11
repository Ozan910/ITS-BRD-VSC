#ifndef RECHNER_H
#define RECHNER_H

#include <stdint.h> // Für uint32_t (Timer Ticks)

/**
 * @brief Berechnet den aktuellen Drehwinkel im Bereich 0.0 bis 360.0 Grad.
 * Wandelt Schritte (auch negative) in eine positive Kreisposition um.
 * * @param position Die absolute Anzahl der Schritte seit Start (von der FSM).
 * @return Winkel in Grad (0.0 <= w < 360.0).
 */
double calcAngle(long position);

/**
 * @brief Berechnet die Winkelgeschwindigkeit in Grad pro Sekunde.
 * * @param stepDelta Anzahl der Schritte im gemessenen Zeitfenster.
 * @param timeDeltaTicks Verstrichene Zeit in Timer-Ticks (Differenz zweier Timer-Werte).
 * @return Geschwindigkeit in Grad/Sekunde (Negativ = Rückwärtsdrehung).
 */
double calcAngularSpeed(long stepDelta, uint32_t timeDeltaTicks);

#endif
//eof