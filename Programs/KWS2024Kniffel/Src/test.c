/**
 ******************************************************************************
 * @file    test.c
 * @author  Franz Korf
 *
 * @date    20. Jan. 2025
 *
 * @brief  Testfunktionen zur Ueberpruefung der einzelnen Aufgaben.
 *
 ******************************************************************************
 */
 
#include "test.h"
#include "main.h"
#include "delay.h"
#include "error.h"
#include <stdio.h>
#include <stdint.h>
#include "LCD_GUI.h"

#define DELAY    500     // Delay in ms zwischen Teiltests

#ifdef AUFGABE3
#include "io.h"

void testIO(void){
   // Schalte 0 bis 16 LEDs der Reihe nach an
   for(int i = 0; i <= 16; i++){
      setLEDs(i);
      delay(DELAY);
   }
   
   // Schalte alle LEDs aus
   setLEDs(0);
   delay(DELAY); 
   
   // Schalte 10 LEDs an
   setLEDs(10);
   delay(DELAY); 
   
   // Schalte 3 LEDs an
   setLEDs(3);
   delay(DELAY);
	 
	 // Test der Funktion buttonPressed
	 // Durch Druecken von Schalter i, werden i + 1 LEDs angeschaltet.
	 // Sind mehrere Taster gedrueckt, wird die Anzahl der LEDs 
	 // angeschaltet, die dem grössten Taster entspricht.
	 while(1){
	    int max = 0;
	    for (int i = 0; i < NO_BUTTONS; i++){
			   if (buttonPressed(i)){
					  max = i + 1;
				 }
				 setLEDs(max);
		  }
			delay(DELAY);
   }	 
}
#endif

#ifdef AUFGABE4
#include "display.h"

void testDisplay(void){
   clearDisplay();
   // Das Display ist blau
	 delay(DELAY);
	
	 
	 // Alle Würfel sind magenta. Von rechts nach links haben sie die Werte 1 2 3 4 5
	 for (int die = 0; die < NO_DICE; die++){
	    displayDie(die, die+1, DIE_COLOR_FIXED);
	    delay(DELAY);
	 }
	 
	 // Die beiden linken Würfel sind gelb und haben 6 Augen
	 displayDie(4, 6, DIE_COLOR_WIN);
	 delay(DELAY);
	 displayDie(3, 6, DIE_COLOR_WIN);
	 delay(DELAY);
	 
	 // Der rechte Würfel ist grün und hat 3 Augen
	 displayDie(0, 3, DIE_COLOR_DEFAULT);
	 delay(DELAY);
	 
   while(1) {}
}
#endif

#ifdef AUFGABE5 
#include "kniffel.h"
#include "display.h"

/**
  * @brief Diese Funktion druckt einen Fehlertext auf dem Bildschirm
  *        und hält im Error Handler an.
  *
  * @param s Text, der gedruckt wird.
  */
static void myError(char *s){
	 Coordinate tl = {1,1};
	 GUI_disStr(tl, s, &Font16, WHITE, RED);
	 Error_Handler();
}

static uint8_t testInfoDice[NO_DICE];   // Hilfsspeicher, der die aktuellen Würfelwerte speichert

/**
  * @brief Diese Funktion aktualisiert tesInfoDice mit den aktuellen 
	*        Würfelwerten.
  */
static void updateDiceInfo(void){
	 for (int die = 0; die < NO_DICE; die++){
		  uint8_t v = getDie(die);
		  if ((v < 1) || (v > 6)) {
				 myError("Unzulaessiger Wuerfelwert.");
			}
			testInfoDice[die] = v;
	 }
}

// Fixiere alle Wuerfel und teste, dass sie sich beim Wuerfeln nicht veraendern.
static void testFix1(void){
	 const int noTests = 3;
	 rollDice();
	 updateDiceInfo();
	 releaseDice();
	 for (int die = 0; die < NO_DICE; die++){
		  fixDie(die); 
	 }
	 for(int i = 0; i < noTests; i++){
      rollDice();
		  for (int die = 0; die < NO_DICE; die++){
				  if (testInfoDice[die] != getDie(die)) {
						 myError("Fixierung Wuerfel fehlerhaft.");
					}
		  }
	 }
}

// Teste die Fixierung einzelner Wuerfel
static void testFix2(void){
	 const int noTests = 3;
	 releaseDice();
	 rollDice();
	 updateDiceInfo();
	 for (int die = 0; die < NO_DICE; die++){
		  releaseDice();
		  rollDice();
		  updateDiceInfo();
		  fixDie(die);
		  for(int i = 0; i < noTests; i++){
				 rollDice();
         if (testInfoDice[die] != getDie(die)) {
						 myError("Fixierung Wuerfel fehlerhaft.");
				 }
		  }
	 }
}

// Nicht fixierte Wuefel aendern schliesslich ihren Wert
static void testFix3(void){
	 const int noTests = 10;
	 releaseDice();
	 updateDiceInfo();
	 bool dieChanged[NO_DICE];  // Info, ob ein Wuerfelwert sich nach 10 Wuerfen schliesslich geaendert hat.
	 for (int die = 0; die < NO_DICE; die++){
		   dieChanged[die] = false;
	 }
	 for (int i = 0; i < noTests; i++){
		  rollDice();
		  for (int die = 0; die < NO_DICE; die++){
			    dieChanged[die] = dieChanged[die] || (testInfoDice[die] != getDie(die));
	    }		 
	 }
	 for (int die = 0; die < NO_DICE; die++){
		  if (!dieChanged[die]){
				 myError("Ist es moeglich, dass nicht alle Wuerfel beim Wuerfeln gewuerfelt werden?"); 
			} 
	 }
}

// Test der Funktion isKniffel
static void testIsKniffel(void){
	 releaseDice();
	 // fixiere Wuerfel, die 1 sind und wuerfel solange, bis alle Wuerfel 1 sind
	 for (int die = 0; die < NO_DICE; die++){
			while (1 != getDie(die)){
				 rollDice();
			}
			fixDie(die);
			bool allOne = true;
			for (int x = 0; x < NO_DICE; x++){
         allOne = allOne && (1 == getDie(x));
      }
      if (!allOne) {
         if (isKniffel()){
					  myError("Kniffel wurde faelschlicherweise erkannt.              "); 
				 }
		  }
	 }
	 // Kniffel liegt vor
	 if (!isKniffel()) {
		  myError("Kniffel wurde nicht erkannt.                "); 
	 }
}

// Test, Strasse liegt nicht vor
static void testIsStreet1(void){
	 if (NO_DICE != 5){
		  myError("testIsStreet1 muss mit 5 Wuerfeln durchgefuehrt werden."); 
	 }
	 // Wuerfel solange, bis mindestens zwei Wuerfel den selben Wert haben. Dann
	 // liegt keine Strasse vor.
	 releaseDice();
	 bool done = false;
	 while(!done){
		  rollDice();
		  for (uint8_t die1 = 0; die1 < NO_DICE; die1++){
				 for (uint8_t die2 = die1 + 1; die2 < NO_DICE; die2++){
					   done = done || (getDie(die1) == getDie(die2));
			   }
			}
	 }
	 if (isStreet()) {
		  myError("isStreet erkennt faelschlicherweise eine Strasse.             ");
	 }
}

// Test, Strasse liegt vor.
static void testIsStreet2(void){
	 if (NO_DICE != 5){
		  myError("testIsStreet2 muss mit 5 Wuerfeln durchgefuehrt werden."); 
	 }
	 // fixiere die Würfel von startVal an, bis eine Strasse vorliegt
	 for (uint8_t startVal = 1; startVal <= 2; startVal++){
		 	 releaseDice();
		   // fixiere die Wuerfel
		   for (uint8_t val = startVal; val < startVal + 5; val ++){
				  bool done = false;
				  while (!done){
						 rollDice();
						 for (uint8_t die = 0; die < NO_DICE; die++){
							  if (!done && (getDie(die) == val)){
									 fixDie(die);
									 done = true;
								}
						 }
				  }
			 }
			 if (!isStreet()){
				  myError("isStreet erkennt eine Strasse nicht.                       ");
			 }
	 }
}
	
void testKniffel(void){
	 GUI_init(DEFAULT_BRIGHTNESS);
	 rollDice();
	 updateDiceInfo();
	
	 // Test der Fixierung der Würfel
	 Coordinate tl = {1,1};

	 testFix1();
	 GUI_disStr(tl, "Test 1 von Modul kniffel abgeschlossen.", &Font16, WHITE, BLACK);
   delay(DELAY);
	 testFix2();
	 GUI_disStr(tl, "Test 2 von Modul kniffel abgeschlossen.", &Font16, WHITE, BLACK);
   delay(DELAY);
	 testFix3();
	 GUI_disStr(tl, "Test 3 von Modul kniffel abgeschlossen.", &Font16, WHITE, BLACK);
   delay(DELAY);
	 testIsKniffel();
	 GUI_disStr(tl, "Test IsKniffel von Modul kniffel abgeschlossen.", &Font16, WHITE, BLACK);
   delay(DELAY);
	 testIsStreet1();
	 GUI_disStr(tl, "Test IsStreet1 von Modul kniffel abgeschlossen.    ", &Font16, WHITE, BLACK);
   delay(DELAY);
	 testIsStreet2();
	 GUI_disStr(tl, "Test IsStreet2 von Modul kniffel abgeschlossen.    ", &Font16, WHITE, BLACK);
   delay(DELAY);
	 GUI_disStr(tl, "Test von Modul kniffel abgeschlossen.              ", &Font16, WHITE, BLACK);
   while(1) {}
}
#endif
// EOF
