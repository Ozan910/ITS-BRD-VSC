#include "displayOutputs.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define X_ANGLE 12
#define Y_ANGLE 2
#define X_SPEED 23
#define Y_SPEED 5

static char angleBuffer[20] = "";
static int angleLength = 0;
static int angleIndex = 0;
static bool angleHasNext = false;

static char speedBuffer[20] = "";
static int speedLength = 0;
static int speedIndex = 0;
static bool speedHasNext = false;


void displayOutputs_initialPrint(void){
    lcdGotoXY(1,2);
    lcdPrintlnS("Drehwinkel:\0");
    lcdGotoXY(1,3);
    lcdPrintlnS("(in Grad)\0");
    lcdGotoXY(1,5);
    lcdPrintlnS("Winkelgeschwindigkeit:\0");
    lcdGotoXY(1,6);
    lcdPrintlnS("(in Grad / Sekunde)\0");
    lcdGotoXY(1,8);
    lcdPrintlnS("if an error occurrs, press S6 to reset\0");
}

void displayOutputs_printAngle(double angle){
    sprintf(angleBuffer, "%3.5f", angle);
    
    angleLength = strlen(angleBuffer);
    angleIndex = 0;
    angleHasNext = true;
}

void displayOutputs_printAngularSpeed(double speed){
    sprintf(speedBuffer, "%.2f", speed);

    speedLength = strlen(speedBuffer);
    speedIndex = 0;
    speedHasNext = true;
}

void displayOutputs_printError(int errorCode){
    lcdGotoXY(18,11);
    lcdPrintS("ERROR\0");
}

void displayOutputs_clearError(void){
    lcdGotoXY(18,11);
    lcdPrintS("     \0");
}

void displayOutputs_printNextChar(void){
    if(angleHasNext){
        if(angleIndex == 0){
            lcdGotoXY(X_ANGLE, Y_ANGLE);
        }
        lcdGotoXY(X_ANGLE + angleIndex,Y_ANGLE);
        lcdPrintC(angleBuffer[angleIndex]);
        angleIndex++;

        if(angleIndex >= angleLength){
            angleHasNext = false;
        }

    }else if(speedHasNext){

        if(speedIndex == 0){
            lcdGotoXY(X_SPEED, Y_SPEED);
        }
        lcdGotoXY(X_SPEED + speedIndex,Y_SPEED);
        lcdPrintC(speedBuffer[speedIndex]);
        speedIndex++;

        if(speedIndex >= speedLength){
            speedHasNext = false;
        }
    }
}