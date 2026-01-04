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
#include "input.h"
#include "crc.h"

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
    mySetLED(7);
    GPIOD->OTYPER &= ~(1U << PINNR);//IO-Port auf push-pull
    GPIOD->BSRR = (1U << PINNR);//IO-Port auf High schalten

    mySleep(750000);//750ms warten

    GPIOD->OTYPER |= (1U << PINNR);//IO-Port auf open-drain
    myClearLED(7);
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

void sendROMCode(ROM_Number *rom){
    sendByte(rom->family);
    for(int i = 0; i < 6; i++){
        sendByte(rom->serial[i]);
    }
    sendByte(rom->crc);
}

void receiveScratchpad(Scratchpad *scratchpad){
    uint8_t tempLSB = readByte();
    uint8_t tempMSB = readByte();
    scratchpad->temperature = (int16_t) ( ( ((uint16_t)tempMSB) << 8 ) | tempLSB );//2 temp Bytes werden in 1 signed 16 Bit int verwandelt
    
    scratchpad->tHRegister = readByte();
    scratchpad->tLRegister = readByte();
    scratchpad->configRegister = readByte();
    scratchpad->reserved1 = readByte();
    scratchpad->reserved2 = readByte();
    scratchpad->reserved3 = readByte();
    scratchpad->crc = readByte();
}

int findAllROMS(TemperatureSensor *sensors, uint16_t maxSensorCount, uint16_t *actualSensorCount){//RETURN 0 = yippee, return -1 = no Sensors found ☹️, -2
    if(maxSensorCount == 0) return 0;//wenn man keine sensoren haben darf
    
    uint64_t sensorsINTROM[maxSensorCount];
    *actualSensorCount = 0;
    
    int16_t letzteUnbehandelteVerzweigung = -1;
    int16_t neueLetzteUnbehandelteVerzweigung = -1;
    bool allFound = false;
    uint16_t currentROMIdx = 0;
    
    while(!allFound){
        sendReset();
		sendByte(0xF0); 
        neueLetzteUnbehandelteVerzweigung = -1;//zurücksetzen auf den initialwert
        uint64_t currentROM = 0;
        for(uint16_t bitIdx = 0; bitIdx < 64; bitIdx++){
            uint64_t incomingBit = (uint64_t) readBit();//cast auf 64 bit int damit die shifts sicher sind 
            uint64_t complBit = (uint64_t) readBit();
            
            if(incomingBit != complBit){                // 0|1 oder 1|0 also alles gut =)
                currentROM |= (incomingBit << bitIdx);
                incomingBit ? sendBit1() : sendBit0();// wenn wir eine: 1 bekommen -> 1 senden / 0 bekommen -> 0 senden
                
            }else if (incomingBit == 0 && complBit == 0){    // 0|0 also mindestens einer sendet 0 und mindestens einer sendet 1
                if(bitIdx < letzteUnbehandelteVerzweigung){             //Fall 1: aktueller Bit ist kleiner als letzteUnbehandelteVerzweigung, wir kopieren den pfad des letzten 😈
                    if((currentROMIdx > 0) && (sensorsINTROM[currentROMIdx - 1] & (1ULL << bitIdx))){//alles > 0 = true, 0 = false
                        currentROM |= (1ULL << bitIdx);
                        sendBit1();
                    }else{//wenn wir eine 0 senden heißt es um die Verzweigung wurde sich beim Vorgänger nicht gekümmert deswegen weiterreichen
                        neueLetzteUnbehandelteVerzweigung = bitIdx;
                        sendBit0();
                    }
                    
                }else if(bitIdx == letzteUnbehandelteVerzweigung){      //Fall 2: aktueller Bit gleich letzteUnbehandelteVerzweigung, wir entscheiden uns diesmal für 1 (behandeln die verzweigung)
                    currentROM |= (1ULL << bitIdx);
                    sendBit1();
                    
                }else if(bitIdx > letzteUnbehandelteVerzweigung){       //Fall 1: aktueller Bit ist größer als letzteUnbehandelteVerzweigung (wir erkunden neues gebiet, standard antwort = 0)
                    neueLetzteUnbehandelteVerzweigung = bitIdx;
                    sendBit0();
                }
                
            }else if (incomingBit == 1 && complBit == 1){
                //Keine Sensoren an der Leitung. return -1
                return -1;
            }else{
                //Unexpected Control flow. return -2
                return -2;
            }
        }
        //1 rom gefunden =)
        //aufräumen für nächste iteration
        allFound = (neueLetzteUnbehandelteVerzweigung == -1);//wenn keine unbehandelte verzweigung mehr durchgereicht wird sind alle abgearbeitet
        letzteUnbehandelteVerzweigung = neueLetzteUnbehandelteVerzweigung;//weiterreichen für den nächsten
        sensorsINTROM[currentROMIdx] = currentROM;//rom speichern in rom array
        (*actualSensorCount)++;//tatsächlichen sensor count erhöhen|
        currentROMIdx++;//ROM Id inkrementieren                    |die beiden könnten eigentlich zu einer variable zusammengefasst werden.
        if(currentROMIdx >= maxSensorCount) allFound = true;//wenn maxSensorCount Sensoren gefunden wurden, gehen wir raus
        
    }
    //hier werden die INTROMs in ROM structs umgewandelt
    for(uint16_t i = 0; i < *actualSensorCount; i++){
        sensors[i].rom_number.family = ((sensorsINTROM[i] >> (0 * 8)) & 0xFF);//Integer Rom wird um n * 8 Bit nach rechts verschoben und mit 0xFF verundet um nur die n. 8 bits zu lesen
        sensors[i].rom_number.serial[0] = ((sensorsINTROM[i] >> (1 * 8)) & 0xFF);
        sensors[i].rom_number.serial[1] = ((sensorsINTROM[i] >> (2 * 8)) & 0xFF);
        sensors[i].rom_number.serial[2] = ((sensorsINTROM[i] >> (3 * 8)) & 0xFF);
        sensors[i].rom_number.serial[3] = ((sensorsINTROM[i] >> (4 * 8)) & 0xFF);
        sensors[i].rom_number.serial[4] = ((sensorsINTROM[i] >> (5 * 8)) & 0xFF);
        sensors[i].rom_number.serial[5] = ((sensorsINTROM[i] >> (6 * 8)) & 0xFF);
        sensors[i].rom_number.crc = ((sensorsINTROM[i] >> (7 * 8)) & 0xFF);

        sensors[i].isROMValid = checkROMCRC(&sensors[i].rom_number);
    }
    return 0;
}