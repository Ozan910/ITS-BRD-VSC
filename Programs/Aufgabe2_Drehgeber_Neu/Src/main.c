/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#include <stdio.h>   // Für snprintf
#include <stdint.h>
#include <stdbool.h>

#include "fsm.h"
#include "rechner.h"
#include "inputs.h"
#include "ledOutputs.h"
#include "displayOutputs.h"
#include "fehler.h" 
#include "timer.h"   

// --- KONSTANTEN ---
#define REFRESH_RATE_TICKS (500000 * TICKS_PER_US)


// --- GLOBALE VARIABLEN (BUFFER) ---
static char displayBuffer[32]; 
static int  bufferReadIndex = 0; 
static bool errorWasActive = false; // Um den Zustandswechsel zu erkennen

// --- HILFSFUNKTIONEN ---

// Setzt die LEDs D22/D23 basierend auf der Richtung
void updateDirectionLEDs(Direction dir) {
    if (dir == DIR_FORWARD) {
        ledOutputs_setForward();
        ledOutputs_clearBackward();
    } else if (dir == DIR_BACKWARD) {
        ledOutputs_clearForward();
        ledOutputs_setBackward();
    }
}

// --- MAIN PROGRAMM ---
/* ANFANG MAIN KOMMENTAR

int main(void) {
    initITSboard();
    initTimer();
    


    // 1. Start-Zustand Display
    displayOutputs_clear();
    displayOutputs_initialPrint();

    // 2. Synchronisierung der FSM
    Phase startPhase = inputs_newPhase();
    fsm_init(startPhase);

    // 3. Variablen für die Rechen-Logik
    uint32_t lastTimeTicks = getTimeStamp();
    long lastSteps = 0;

    // Puffer initial leeren
    displayBuffer[0] = '\0';

    // --- SUPER LOOP ---
    while (1) {
        // ============================================================
        // TEIL A: Der "Fast Path"
        // ============================================================
        
        Phase currentPhase = inputs_newPhase();
        
        // S6 Check (Reset)
        if (inputs_isS6Pressed()) {
             fsm_resetError(currentPhase);
        }

        // FSM füttern
        fsm_update(currentPhase);

        // ============================================================
        // TEIL B: Logik-Weiche (Fehler vs. Normal)
        // ============================================================

        if (fsm_isErrorState()) {           
            //Nur beim ERSTEN Eintritt in den Fehler tun wir was
            if (!errorWasActive) {
                displayOutputs_clear();
                displayOutputs_printError(); // Zeigt Fehlertext
                ledOutputs_setError();       // D21 an
                
                errorWasActive = true;       // Merken: Wir sind im Fehler
            }
            // Wir warten effektiv, bis S6 gedrückt wird (oben im Loop).

        } else {
            // --- NORMALBETRIEB ---

            //Wenn wir gerade aus dem Fehler kommen
            if (errorWasActive) {
                displayOutputs_initialPrint(); // Layout wiederherstellen
                ledOutputs_clearError();       // D21 aus
    
                lastSteps = fsm_getStepCount();
                lastTimeTicks = getTimeStamp();
                
                errorWasActive = false;
            }

            // 1. LEDs aktualisieren (Schnell)
            long steps = fsm_getStepCount();
            ledOutputs_setStepCount((uint8_t)steps); // Binärzähler D8-D15
            
            Direction dir = fsm_getLastDirection();
            updateDirectionLEDs(dir); // D22/D23

            // 2. Zeitmessung & Rechnen
            uint32_t currentTime = getTimeStamp();
            uint32_t timeDelta = currentTime - lastTimeTicks; 

            if (timeDelta >= REFRESH_RATE_TICKS) {
                
                // A) Delta berechnen
                long stepDelta = steps - lastSteps;
                
                // B) Mathematik
                double angle = calcRotationalAngle(steps);
                double speed = calcAngularSpeed(stepDelta, timeDelta);

				// C) Ausgabe ((((((((ERSATZ FÜR BUFFER))))))))
                displayOutputs_printAngle(angle);
                displayOutputs_printAngularSpeed(speed);
				
				//Für Buffer
                //snprintf(displayBuffer, sizeof(displayBuffer), "%6.1f|%6.1f", angle, speed);
                
                // D) Referenzen aktualisieren
                lastSteps = steps;
                lastTimeTicks = currentTime;
                bufferReadIndex = 0; // Buffer bereit zum Senden machen
            }

            	// 3. Gepufferte Ausgabe (Nur 1 Zeichen pro Loop!)
            	// Das verhindert, dass die FSM blockiert wird.
            //char charToSend = displayBuffer[bufferReadIndex];
            
            //if (charToSend != '\0') {
					// Funktion für Char an richtige Stelle im Display
                	// displayOutputs_writeNextChar(charToSend); <--- HYPOTHETISCH
                
                //bufferReadIndex++;
                
                	// Wenn wir das Ende erreicht haben, hören wir auf
                //if (displayBuffer[bufferReadIndex] == '\0') {
                    // Fertig gesendet
                //}
            }
        }
}


ENDE MAIN KOMMENTAR*/
// /* ANFANG TEST
int main(void){

    initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

    uint8_t counterr = 253;
    displayOutputs_initialPrint();
    while(1){
        if(inputs_isS6Pressed()){
            ledOutputs_setForward();
            ledOutputs_setError();
            ledOutputs_setBackward();
        }
        ledOutputs_clearBackward();
        ledOutputs_clearForward();
        ledOutputs_clearError();
        ledOutputs_setStepCount(counterr);
    }
}
//    ENDE TEST*/