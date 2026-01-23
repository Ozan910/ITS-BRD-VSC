#include "myDisp.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include "myerr.h"
#include <stdio.h>
#include <string.h>

#define LCD_X_START 0
#define LCD_Y_START 0

#define ERR_OFF_X 0
#define ERR_OFF_Y 10

#define OPERATION_OFF_Y 5

#define OPL_OFF_X 0

#define OP_OFF_X 2

#define OPR_OFF_X 4

#define EQ_OFF_X 6

#define RS_OFF_X 8

#define ASCIINRSTART '0'
#define STRINGBUFSIZE 20
#define ERGBUF 5


static uint8_t lastLeftOp = 0;
static uint8_t lastRightOp = 0;
static char lastOp = '+';

void initDisp(void){
    lcdGotoXY(LCD_X_START, LCD_Y_START);
    lcdPrintlnS("3 Bit UnsignedTaschenrechner");

    lcdGotoXY(LCD_X_START, LCD_Y_START + OPERATION_OFF_Y);
    lcdPrintlnS("0 + 0");
}

void printError(void){
    char buf[STRINGBUFSIZE];
    snprintf(buf, sizeof(buf), "Fehler: %d", getError());
    lcdGotoXY(LCD_X_START + ERR_OFF_X, LCD_Y_START + ERR_OFF_Y);
    lcdPrintS(buf);
}

void clearError(void){
    char buf[STRINGBUFSIZE];
    memset(buf, ' ', sizeof(buf));
    lcdGotoXY(LCD_X_START + ERR_OFF_X, LCD_Y_START + ERR_OFF_Y);
    lcdPrintS(buf);
}

void druckeOp(char operation, uint8_t leftOp, uint8_t rightOp ){
    if(leftOp != lastLeftOp){
        lcdGotoXY(LCD_X_START + OPL_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
        lcdPrintC(leftOp + ASCIINRSTART);
        lastLeftOp = leftOp;
    }
    if(operation != lastOp){
        lcdGotoXY(LCD_X_START + OP_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
        lcdPrintC(operation);
        lastOp = operation;
    }
    if(rightOp != lastRightOp){
        lcdGotoXY(LCD_X_START + OPR_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
        lcdPrintC(rightOp + ASCIINRSTART);
        lastRightOp = rightOp;
    }
    lcdGotoXY(LCD_X_START + EQ_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
    lcdPrintlnS("     ");
}

void druckeOpMitErg(char operation, uint8_t leftOp, uint8_t rightOp, int8_t erg){
    if(leftOp != lastLeftOp){
        lcdGotoXY(LCD_X_START + OPL_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
        lcdPrintC(leftOp + ASCIINRSTART);
        lastLeftOp = leftOp;
    }
    if(operation != lastOp){
        lcdGotoXY(LCD_X_START + OP_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
        lcdPrintC(operation);
        lastOp = operation;
    }
    if(rightOp != lastRightOp){
        lcdGotoXY(LCD_X_START + OPR_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
        lcdPrintC(rightOp + ASCIINRSTART);
        lastRightOp = rightOp;
    }
    lcdGotoXY(LCD_X_START + EQ_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
    lcdPrintC('=');
    char buf[ERGBUF];
    snprintf(buf, sizeof(buf), "%d", erg);
    lcdGotoXY(LCD_X_START + RS_OFF_X, LCD_Y_START + OPERATION_OFF_Y);
    lcdPrintS(buf);
}