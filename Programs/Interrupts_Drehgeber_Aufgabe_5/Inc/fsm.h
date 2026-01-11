#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    PHASE_A,// 00
    PHASE_D,// 01
    PHASE_B,// 10
    PHASE_C// 11
} Phase;

typedef enum {
    IDLE, FWD, REV, ERR
} Direction;

Direction getDirection(uint8_t phase);

uint8_t getOldPhase(void);

void setOldPhase(uint8_t phase);

void waitForReset(void);

#endif
//eof