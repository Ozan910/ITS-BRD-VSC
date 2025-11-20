#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdint.h>

#include "inputs.h" 

// Definition der Richtungen für die LEDs/Logik
typedef enum {
    DIR_NONE,       // Stillstand
    DIR_FORWARD,    // Vorwärts (Rechts)
    DIR_BACKWARD    // Rückwärts (Links)
} Direction;

/**
 * Initialisiert die FSM (setzt Zähler auf 0, Zustand auf initiale Phase).
 */
void fsm_init(Phase initialPhase);

/**
 * Die Hauptfunktion der FSM. Muss zyklisch in der Super-Loop aufgerufen werden.
 * Verarbeitet den Phasenübergang basierend auf dem neuen Input.
 * @param newPhase Die aktuell gemessene Phase (A, B, C oder D).
 */
void fsm_update(Phase newPhase);

/**
 * Gibt die aktuelle Anzahl der gezählten Schritte (Phasenwechsel) zurück.
 * Kann positiv oder negativ sein (Vorwärts-Rückwärtszähler).
 */
long fsm_getStepCount(void);

/**
 * Gibt die letzte erkannte Bewegungsrichtung zurück.
 * Dient zur Ansteuerung der LEDs D22 (Rück) und D23 (Vor).
 */
Direction fsm_getLastDirection(void);

/**
 * Prüft, ob sich die FSM im Fehlerzustand befindet.
 * @return true wenn Fehler aktiv (LED D21 an), sonst false.
 */
bool fsm_isErrorState(void);

/**
 * Löscht den Fehlerzustand (wird durch Taster S6 ausgelöst).
 */
void fsm_resetError(Phase currentPhase);

/**
* @brief returns total steps
*/
long fsm_getTotalSteps(void);

#endif // FSM_H