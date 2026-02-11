#include "io.h"
#include "error.h"
#include "stm32f4xx_hal.h"

#define MAX_LED_COUNT 16

void setLEDs(uint8_t noLeds){
    if (noLeds > MAX_LED_COUNT){
        Error_Handler();
    }

    uint16_t ledMaske = 0;
    for(int i = 0; i<noLeds; i++){
        ledMaske |= (1<<i);
    }   

    GPIOD->BSRR = (0xFF << 16);
    GPIOE->BSRR = (0xFF << 16);

    GPIOD->BSRR = (ledMaske & 0xFF);
    GPIOE->BSRR = (ledMaske >> 8);
}

bool buttonPressed(uint8_t button){
    if(button > 7){
        Error_Handler();
    }
    return !(GPIOF->IDR & (1u << button));
}

// EOF