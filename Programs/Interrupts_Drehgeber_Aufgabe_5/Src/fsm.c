#include "fsm.h"
#include "stm32f4xx.h"

void waitForReset(void){
    while(GPIOF->IDR & (1 << 6)){
        //warten bis der benutzer S6 drückt
    }
}