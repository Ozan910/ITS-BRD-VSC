#include "fsm.h"
#include "stm32f4xx.h"

static uint8_t oldPhase = 0;

static const Direction dirField[][4]=
{   {IDLE, REV, FWD, ERR},
    {FWD, IDLE, ERR, REV},
    {REV, ERR, IDLE, FWD},
    {ERR, FWD, REV, IDLE}};

Direction getDirection(uint8_t phase){
    phase &= 0x03U;
    Direction dir = dirField[oldPhase][phase];
    oldPhase = phase;
    return dir;
}

uint8_t getOldPhase(){
    return oldPhase;
}

void setOldPhase(uint8_t phase){
    oldPhase = phase;
}

void waitForReset(void){
    while(GPIOF->IDR & (1 << 6)){
        //warten bis der benutzer S6 drückt
    }
}