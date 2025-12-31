/**
 ******************************************************************
  * @file base_functions.h
  * @author Ozan
  * @date December 2025
  * @brief Dieses Modul implementiert die Basis Funktionen für den One Wire Bus. NUR IM OPEN-DRAIN MODE BENUTZEN!!!
 ******************************************************************
*/

#include "myTimer.h"
#define PINNR 0

void sendBit1(void){
    GPIOD->BSRR = (1U << (PINNR + 16));//bus low
    mySleep(6);//sleep 6 US
    GPIOD->BSRR = (1U << PINNR);//bus frei
    mySleep(64);
}

void sendBit0(void){
    GPIOD->BSRR = (1U << (PINNR + 16));//bus low
    mySleep(60);
    GPIOD->BSRR = (1U << PINNR);//bus frei
    mySleep(10);
}

uint8_t readBit(void){
    GPIOD->BSRR = (1U << (PINNR + 16));//bus low
    mySleep(6);
    GPIOD->BSRR = (1U << PINNR);//bus frei
    mySleep(9);
    uint8_t bit = ((GPIOD->IDR >> PINNR) & 1U);//bit lesen
    mySleep(55);
    return bit;
}

uint8_t sendReset(void){
    GPIOD->BSRR = (1U << (PINNR + 16));//bus low
    mySleep(480);
    GPIOD->BSRR = (1U << PINNR);//bus frei
    mySleep(70);
    uint8_t bit = ((GPIOD->IDR >> PINNR) & 1U);//bit lesen
    mySleep(410);
    return bit;
}

//auf 1 Byte erhöht für einfachere Nutzung

uint8_t readByte(void){
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++){
        byte |= (readBit() << i);//weil LSB zuerst kommt von rechts nach links schreiben
    }
    return byte;
}

void sendByte(uint8_t byte){
    for(int i = 0; i < 8; i++){//1 mal für jedes bit also 8 mal
        (byte & 1U) ? sendBit1() : sendBit0();//wenn LSB 1 > send1, wenn LSB 0 send 0
        byte = byte >> 1;//byte wird um 1 nach rechts verschoben > nächstes LSB rückt nach
    }
}
