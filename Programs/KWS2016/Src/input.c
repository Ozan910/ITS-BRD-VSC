#include "input.h"
#include "stm32f4xx.h"
#include "myTimer.h"
#include "myerr.h"
#include "timer.h"


#define TICKS_PRELLEN 5000 * TICKS_PER_US
#define VERSUCHE_PRELLEN 3

uint8_t readAllButtons(){
    return GPIOF->IDR & 0xFF;
}

uint8_t getDebouncedButtons(){
    for(int i = 0; i < VERSUCHE_PRELLEN; i++){
        uint8_t try1 = readAllButtons();
        mySleep(TICKS_PRELLEN);
        uint8_t try2 = readAllButtons();
        if(try1 == try2){
            return try1;
        } 
    }
    setError(IO_ERR);
    return 0xFF;
}


