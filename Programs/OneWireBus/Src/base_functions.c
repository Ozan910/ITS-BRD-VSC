/**
 ******************************************************************
  * @file base_functions.c
  * @author Ozan
  * @date December 2025
  * @brief Dieses Modul implementiert die Basis Funktionen für den One Wire Bus. NUR IM OPEN-DRAIN MODE BENUTZEN!!!
 ******************************************************************
*/

#include "base_functions.h"
#include "myTimer.h"
#include "stm32f4xx.h"

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

void powerSupply750ms(void){
    GPIOD->OTYPER &= ~(1U << PINNR);//IO-Port auf push-pull
    GPIOD->BSRR = (1U << PINNR);//IO-Port auf High schalten

    mySleep(750000);//750ms warten

    GPIOD->OTYPER |= (1U << PINNR);//IO-Port auf open-drain
}


void init1WireBus(void){
    GPIOD->MODER = (GPIOD->MODER & ~MODER_MASK_PIN_1) | OUTPUT_MASK_PIN_1;//setting MODER of GPIOD Pin 1 to Output
    GPIOD->OTYPER &= ~(1U << 1);//setting OTYPER of GPIOD Pin 1 to Push-Pull mode
    GPIOD->BSRR = (1U << 1);//setting IO Port Pin 1 GPIOD on high

    GPIOD->MODER = (GPIOD->MODER & ~MODER_MASK_PIN_0) | OUTPUT_MASK_PIN_0;//setting MODER of GPIOD Pin 0 to Output
    GPIOD->OTYPER |= (1U << PINNR);//setting OTYPER of GPIOD Pin 0 to Open-Drain mode
    GPIOD->BSRR = (1U << PINNR);//sets the bus on high (through pull up)
}

void receiveSingleROM(ROM_Number *rom){//ROM kommt LSB -> MSB
    rom->family = readByte();//erste 8 Bit die gelesen werden family code
    for (int i = 0; i < 6; i++){
        rom->serial[i] = readByte();//nächste 48 Bit serial number
    }
    rom->crc = readByte();//letzte 8 Bit sind CRC
}

void receiveScratchpad(Scratchpad *scratchpad){
    uint8_t tempLSB = readByte();
    uint8_t tempMSB = readByte();
    scratchpad->temperature = (int16_t) ( ( ((uint16_t)tempMSB) << 8 ) | tempLSB );
    
    scratchpad->thRegister = readByte();
    scratchpad->tLRegister = readByte();
    scratchpad->configRegister = readByte();
    scratchpad->reserved1 = readByte();
    scratchpad->reserved2 = readByte();
    scratchpad->reserved3 = readByte();
    scratchpad->crc = readByte();
}