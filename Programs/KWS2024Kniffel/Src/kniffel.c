/**
 ******************************************************************************
 * @file    kniffel.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    20. Jan. 2025
 * @brief   Dieses Modul speichert den Zustand der W�rfel.
 *
 ******************************************************************************
 */

#include "kniffel.h"
#include "main.h"
#include "error.h"
#include "random.h"

KniffelDice wuerfel[MAX_DICE] = {0};

uint8_t getDie(uint8_t die){
    if(die >= MAX_DICE){
        Error_Handler();
    }
    return wuerfel[die].value;
}

bool isKniffel(void){
    uint8_t start = wuerfel[0].value;
    bool isKniffel;
    for(int i=1; i < MAX_DICE; i++){
        isKniffel = start == wuerfel[i].value;
        if(!isKniffel) break;
    }
    return isKniffel;
}

void getSortedDiceValues(uint8_t* sorted, int n){
    for(int i = 0; i < n; i++) sorted[i] = wuerfel[i].value;
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < n - 1 - i; j++){
            if(sorted[j] > sorted[j + 1]){
                uint8_t t = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = t;
            }
        }
    }
}

bool isStreet(void){
    uint8_t sortedValues[MAX_DICE];
    getSortedDiceValues(sortedValues, MAX_DICE);
    bool isStreet = true;
    for(int i = 0; i < MAX_DICE-1; i++){
        if(!(sortedValues[i] == sortedValues[i+1] - 1)) isStreet = false;
    }
    return isStreet;
}

void fixDie(uint8_t die){
    wuerfel[die].state = FIXED;
}

void releaseDice(void){
    for(int i = 0; i < MAX_DICE; i++){
        wuerfel[i].state = RELEASED;
    }
}

void rollDice(void){
    for(int i=0; i < MAX_DICE; i++){
        if(wuerfel[i].state == RELEASED){
            wuerfel[i].value = (nxtRandNumber() % 6) + 1;
        }
    }
}

// EOF