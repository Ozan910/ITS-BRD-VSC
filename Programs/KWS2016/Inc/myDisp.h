#ifndef MYDISP_H
#define MYDISP_H

#include <stdint.h>


void initDisp(void);

void printError(void);

void clearError(void);

void druckeOp(char operation, uint8_t leftOp, uint8_t rightOp );

void druckeOpMitErg(char operation, uint8_t leftOp, uint8_t rightOp, int8_t erg);

#endif
//eof