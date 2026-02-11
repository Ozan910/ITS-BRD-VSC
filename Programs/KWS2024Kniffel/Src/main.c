/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Die Klausuraufgabe Kniffel.
 *
 ******************************************************************************
 */

#include "main.h"
#include <stdlib.h>
#include "init.h"
#include "io.h"
#include "display.h"
#include "kniffel.h"
#include "test.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "LCD_GUI.h"


int main(void){
   // Initialisierung der HW
   initITSboard();
	 GUI_init(DEFAULT_BRIGHTNESS);
    bool wuerfelButtonsPressed[MAX_DICE] = {0};
    bool releaseButtonPressed = false;
    bool rollButtonPressed = false;
    bool resetButtonPressed = false;
    uint8_t counter = 0;

   KniffelDice lastDrawn[MAX_DICE];
   for(int i=0;i<MAX_DICE;i++){ lastDrawn[i].value = 0xFF; lastDrawn[i].state = 0xFF; }


    clearDisplay();
    rollDice();
    while(1){
      //INPUT
      for(int i = 0; i < MAX_DICE; i++){
         wuerfelButtonsPressed[i] = buttonPressed(i);
      }
      releaseButtonPressed = buttonPressed(5);
      rollButtonPressed = buttonPressed(6);
      resetButtonPressed = buttonPressed(7);
      //RECHNUNG

      if(resetButtonPressed){
         for(int i = 0; i < MAX_DICE; i++){
            wuerfel[i].value = 0;
            wuerfel[i].state = RELEASED;
         }
         counter = 0;
         rollDice();
         continue;
      }

      if(isStreet() || isKniffel()){
         for(int i = 0; i < MAX_DICE; i++){
            wuerfel[i].state = WIN;
         }
      }else{
         if(releaseButtonPressed){
            releaseDice();
         }else{
            for(int i = 0; i < MAX_DICE; i++){
               if(wuerfelButtonsPressed[i]){
                  wuerfel[i].state = FIXED;
               }
            }
         }
      }
      if((!(isStreet() || isKniffel())) && rollButtonPressed){
         counter++;
         rollDice();
      }

      //OUTPUT
      setLEDs(counter % 17);
      for(int i = 0; i < MAX_DICE; i++){
         COLOR c = DIE_COLOR_DEFAULT;
         if (wuerfel[i].state == WIN) c = DIE_COLOR_WIN;
         else if (wuerfel[i].state == FIXED) c = DIE_COLOR_FIXED;

         if (lastDrawn[i].value != wuerfel[i].value || lastDrawn[i].state != wuerfel[i].state){
            displayDie(i, wuerfel[i].value, c);
            lastDrawn[i] = wuerfel[i];
         }
      }
   }  
}
// EOF