#include "displayOutputs.h"
#include "lcd.h"
#include <stdio.h>

void displayOutputs_initialPrint(void){
    lcdGotoXY(1,2);
    lcdPrintlnS("Drehwinkel:");
    lcdGotoXY(1,4);
    lcdPrintlnS("Winkelgeschwindigkeit:");
}

void displayOutputs_printAngle(double angle){
    char str[20];
    sprintf(str, "%.2f", angle);


    lcdGotoXY(12,2);
    lcdPrintReplS(str);
}

void displayOutputs_printAngularSpeed(double speed){
    char str[20];
    sprintf(str, "%.2f", speed);

    lcdGotoXY(23,4);
    lcdPrintReplS(str);
}

void displayOutputs_printError(int errorCode){
    lcdPrintlnS("Ein Fehler ist aufgetreten. Zum resetten: S6")
}