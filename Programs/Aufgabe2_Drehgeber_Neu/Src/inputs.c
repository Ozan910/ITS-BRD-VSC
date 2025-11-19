#include "inputs.h"
#include "stm32f4xx.h"

Phase inputs_newPhase(void){
    return (GPIOF->IDR & 0x3);//gibt die beiden werte von IN0 und IN1 wieder in binär 2 stellig: (IN1-> XX <- IN0)
}                             //Wenn 00 dann a, wenn 01 dann d, wenn 11 dann c, wenn 10 dann b ( enums sind entsprehcend defined )

bool inputs_isS6Pressed(void){
    return !(GPIOF->IDR & (1 << 6));// wenn gedrückt dann 0 => durch ! => 1 = true
}                                   // wenn nicht    dann 1 => durch ! => 0 = false