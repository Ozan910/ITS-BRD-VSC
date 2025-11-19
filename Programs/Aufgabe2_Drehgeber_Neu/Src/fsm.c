#include "fsm.h"

// --- INTERNE TYPEN ---

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_ERROR
} FsmState;

// --- STATISCHE VARIABLEN (ZUSTANDSSPEICHER) ---

static FsmState currentState = STATE_A;   // Startannahme: Wir stehen in Phase A
static long stepCounter = 0;              // Der Schrittzähler
static Direction lastDirection = DIR_NONE; // Letzte bekannte Richtung

// --- HILFSFUNKTIONEN ---

static FsmState mapPhaseToState(Phase p) {
    switch (p) {
        case PHASE_A: return STATE_A;
        case PHASE_B: return STATE_B;
        case PHASE_C: return STATE_C;
        case PHASE_D: return STATE_D;
        default: return STATE_A;
    }
}

// --- ÖFFENTLICHE FUNKTIONEN ---

void fsm_init(Phase initialPhase) {
    currentState = mapPhaseToState(initialPhase); 
    stepCounter = 0;
    lastDirection = DIR_NONE;
}

void fsm_update(Phase newPhase) {
    // 1. Prüfung: Wenn wir im Fehlerzustand sind, blockieren wir alles.
    // Der Fehler muss erst durch fsm_resetError() gelöscht werden[cite: 91, 92].
    if (currentState == STATE_ERROR) {
        return;
    }

    // 2. Zustandsübergangs-Logik (Switch über den ALTEN Zustand)
    switch (currentState) {
        
        case STATE_A:
            if (newPhase == PHASE_B) {
                // A -> B: Vorwärts
                stepCounter++;
                lastDirection = DIR_FORWARD;
                currentState = STATE_B;
            } else if (newPhase == PHASE_D) {
                // A -> D: Rückwärts (Zyklus: ... D -> A -> B ...)
                stepCounter--;
                lastDirection = DIR_BACKWARD;
                currentState = STATE_D;
            } else if (newPhase == PHASE_C) {
                // A -> C: Übersprungener Schritt -> Fehler
                currentState = STATE_ERROR;
            }
            // A -> A: Keine Änderung, nichts tun
            break;

        case STATE_B:
            if (newPhase == PHASE_C) {
                // B -> C: Vorwärts 
                stepCounter++;
                lastDirection = DIR_FORWARD;
                currentState = STATE_C;
            } else if (newPhase == PHASE_A) {
                // B -> A: Rückwärts 
                stepCounter--;
                lastDirection = DIR_BACKWARD;
                currentState = STATE_A;
            } else if (newPhase == PHASE_D) {
                // B -> D: Fehler 
                currentState = STATE_ERROR;
            }
            break;

        case STATE_C:
            if (newPhase == PHASE_D) {
                // C -> D: Vorwärts
                stepCounter++;
                lastDirection = DIR_FORWARD;
                currentState = STATE_D;
            } else if (newPhase == PHASE_B) {
                // C -> B: Rückwärts
                stepCounter--;
                lastDirection = DIR_BACKWARD;
                currentState = STATE_B;
            } else if (newPhase == PHASE_A) {
                // C -> A: Fehler
                currentState = STATE_ERROR;
            }
            break;

        case STATE_D:
            if (newPhase == PHASE_A) {
                // D -> A: Vorwärts
                stepCounter++;
                lastDirection = DIR_FORWARD;
                currentState = STATE_A;
            } else if (newPhase == PHASE_C) {
                // D -> C: Rückwärts
                stepCounter--;
                lastDirection = DIR_BACKWARD;
                currentState = STATE_C;
            } else if (newPhase == PHASE_B) {
                // D -> B: Fehler
                currentState = STATE_ERROR;
            }
            break;
            
        default:
            // Sicherheitshalber: Unbekannter Zustand -> Fehler
            currentState = STATE_ERROR;
            break;
    }
}

long fsm_getStepCount(void) {
    return stepCounter;
}

Direction fsm_getLastDirection(void) {
    return lastDirection;
}

bool fsm_isErrorState(void) {
    return (currentState == STATE_ERROR);
}

void fsm_resetError(Phase currentPhase) {
    currentState = mapPhaseToState(currentPhase);
}