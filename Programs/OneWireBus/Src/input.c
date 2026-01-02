/**
 *******************************************************************
  * @file input.c
  * @author Ozan
  * @date January 2026
  * @brief Dieses Modul implementiert das input Modul für Schalter eingaben und LED ausgaben
 *******************************************************************
*/

#include "input.h"
#include "stm32f4xx.h"


bool inputs_isS0Pressed(void){
    return !(GPIOF->IDR & (1 << 0));// wenn gedrückt dann 0 => durch ! => 1 = true
}                                   // wenn nicht    dann 1 => durch ! => 0 = false

void mySetLED(int ledId){
    if(ledId > 7 || ledId < 0) return;
    GPIOE->BSRR = (1 << ledId);
}

void myClearLED(int ledId){
    if(ledId > 7 || ledId < 0) return;
    GPIOE->BSRR = (1 << (ledId + 16));
}