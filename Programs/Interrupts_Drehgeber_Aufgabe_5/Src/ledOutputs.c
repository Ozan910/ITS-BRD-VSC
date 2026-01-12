#include "ledOutputs.h"
#include "stm32f4xx.h"

#define LED_FORWARD 7
#define LED_BACKWARD 6
#define LED_ERROR 5


static void mySetLED(int ledId){
    if (ledId != LED_FORWARD && ledId != LED_BACKWARD && ledId != LED_ERROR){
        return;
    }
    GPIOE->BSRR = (1 << ledId);
}

static void myClearLED(int ledId){
    if (ledId != LED_FORWARD && ledId != LED_BACKWARD && ledId != LED_ERROR){
        return;
    }
    GPIOE->BSRR = (1 << (ledId + 16));
}

void ledOutputs_setForward(void){
    mySetLED(LED_FORWARD);
}

void ledOutputs_clearForward(void){
    myClearLED(LED_FORWARD);
}

void ledOutputs_setBackward(void){
    mySetLED(LED_BACKWARD);
}

void ledOutputs_clearBackward(void){
    myClearLED(LED_BACKWARD);
}

void ledOutputs_setError(void){
    mySetLED(LED_ERROR);
}

void ledOutputs_clearError(void){
    myClearLED(LED_ERROR);
}

void ledOutputs_setStepCount(uint32_t count){
    uint8_t bits = (uint8_t) count;
    GPIOD->BSRR = (0xFF << 16);//löscht alle LEDs

    GPIOD->BSRR = bits;//setzt aktuelle step zah als LEDs 
}
