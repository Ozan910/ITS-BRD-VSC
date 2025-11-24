#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdint.h>

#include "inputs.h" 

/**
 * @brief Definition der Richtungen für die LEDs und Logik.
 */
typedef enum {
    DIR_NONE,       /**< Stillstand */
    DIR_FORWARD,    /**< Vorwärts (Rechts) */
    DIR_BACKWARD    /**< Rückwärts (Links) */
} Direction;

/**
 * @brief Initialisiert die Finite State Machine (FSM).
 * * Setzt interne Zähler auf 0 und den Zustand auf die übergebene Phase.
 * * @param initialPhase Die initiale Phase, mit der die FSM starten soll.
 */
void fsm_init(Phase initialPhase);

/**
 * @brief Die Hauptfunktion der FSM.
 * * Muss zyklisch in der Super-Loop aufgerufen werden. 
 * Verarbeitet den Phasenübergang basierend auf dem neuen Input.
 * * @param newPhase Die aktuell gemessene Phase (A, B, C oder D).
 */
void fsm_update(Phase newPhase);

/**
 * @brief Gibt die aktuelle Anzahl der gezählten Schritte (Phasenwechsel) zurück.
 * * Der Wert kann positiv oder negativ sein, weil Step nach vorne ist steps +1 und rückwärts -1.
 * * @return Aktueller Zählerstand (long).
 */
long fsm_getStepCount(void);

/**
 * @brief Gibt die letzte erkannte Bewegungsrichtung zurück.
 * * Diese Information dient zur Ansteuerung der LEDs D22 (Rück) und D23 (Vor).
 * * @return Die ermittelte Richtung (DIR_NONE, DIR_FORWARD oder DIR_BACKWARD).
 */
Direction fsm_getLastDirection(void);

/**
 * @brief Prüft, ob sich die FSM im Fehlerzustand befindet.
 * * @return true wenn ein Fehler aktiv ist (LED D21 an), sonst false.
 */
bool fsm_isErrorState(void);

/**
 * @brief Löscht den Fehlerzustand der FSM.
 * * Diese Funktion wird durch den Taster S6 ausgelöst.
 * * @param currentPhase Die aktuelle Phase, um den Zustand korrekt zurückzusetzen.
 */
void fsm_resetError(Phase currentPhase);

/**
 * @brief Gibt die Gesamtzahl aller Schritte zurück.
 * * @return Die absolute Anzahl der Schritte unabhängig von rückwärts/vorwärts, 
 * * also ++ für jeden Step (Total Steps).
 */
long fsm_getTotalSteps(void);

#endif // FSM_H