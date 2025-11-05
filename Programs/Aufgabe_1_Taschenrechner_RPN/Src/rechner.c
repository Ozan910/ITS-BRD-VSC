#include "rechner.h"
#include <stdbool.h>
#include "stack.h"
#include "fehler.h"
#include "display.h"

#define INT_MAX 2147483647
#define INT_MIN -2147483648


int numToString(int num, char *result){
    //Wenn Zahl = 0
    if (num == 0){
        result[0] = '0';
        result[1] = 0;
        return SUCCESS;
    }

    //checken ob die Zahl negativ ist
    bool isNegative = (num < 0);
    if(isNegative) num *= -1;

    //länge der Zahl zählen
    int numLength = 0;
    int temp = num;
    while (temp > 0){
        temp /= 10;
        numLength++;
    }

    //Wenn zahl negativ ist dann wird die zahl im array um 1 nach rechts verschoben und ein '-' vorne ran gehangen
    if(isNegative) {
        numLength++;
        result[0] = '-';
    }

    //0 am ende des "Strings" schreiben
    result[numLength] = 0;
    numLength--;

    //Zahl als Character von hinten nach vorne durch Modulo Division und addition von '0' in Character Array schreiben
    int numStart = (isNegative) ? 1 : 0;
    while (numLength >= numStart){
        result[numLength] = (num % 10) + '0';
        num /= 10;
        numLength--;
    }
    return SUCCESS;
}


//addiert die letzten 2 zahlen auf dem stack und pusht das ergebnis auf den stack
int addition(){
    int x;
    int y;

    int ret1 = stack_pop(&x);
    if(ret1 != 0) {
        return ret1;
    }
    int ret2 = stack_pop(&y);
    if(ret2 != 0) {
        return ret2;
    }

    //Bereichsüberschreitungen prüfen
    if(x > 0 && y > (INT_MAX - x)){//überprüft ob Integer Overflow stattfindet
        return INTEGER_OVERFLOW;
    }
    if(x < 0 && y < (INT_MIN - x)){//überprüft ob Integer Underflow stattfindet
        return INTEGER_UNDERFLOW;
    }

    stack_push(x + y);
    return SUCCESS;

}
//subtrahiert die letzte zahl auf dem stack mit der vorletzten und pusht das ergebnis auf den stack
int substraction(){
    int x;
    int y;

    int ret1 = stack_pop(&x);
    if(ret1 != 0) {
        return ret1;
    }
    int ret2 = stack_pop(&y);
    if(ret2 != 0) {
        return ret2;
    }

    //Bereichsüberschreitungen prüfen
    if (x < 0 && y > (INT_MAX + x)){
        return INTEGER_OVERFLOW;
    }
    if(x > 0 && y < (INT_MIN + x)){
        return INTEGER_UNDERFLOW;
    }

    stack_push(y - x);
    return SUCCESS;

}
//multipliziert die letzten 2 zahlen auf dem stack und pusht das ergebnis auf den stack
int multiply(){
    int x;
    int y;

    int ret1 = stack_pop(&x);
    if(ret1 != 0) {
        return ret1;
    }
    int ret2 = stack_pop(&y);
    if(ret2 != 0) {
        return ret2;
    }

    if(x > 0 && y > (INT_MAX / x)){
        return INTEGER_OVERFLOW;
    }
    if(x < 0 && y > (INT_MIN / x)){
        return INTEGER_UNDERFLOW;
    }
    if(x > 0 && y < (INT_MIN / x)){
        return INTEGER_UNDERFLOW;
    }
    if(x < 0 && y < (INT_MAX / x)){
        return INTEGER_OVERFLOW;
    }
    if(x == -1 && y == INT_MIN){
        return INTEGER_OVERFLOW;
    }
    if(y == -1 && x == INT_MIN){
        return INTEGER_OVERFLOW;
    }

    stack_push(x * y);
    return SUCCESS;

}


//teil den 2. wert auf dem stack durch den obersten und pusht das ergebnis auf den stack///int min durch -1///0
int divide(){
    int x;
    int y;

    int ret1 = stack_pop(&x);
    if(ret1 != 0) {
        return ret1;
    }
    int ret2 = stack_pop(&y);
    if(ret2 != 0) {
        return ret2;
    }

    if(x == 0){
        return ZERO_DIVISION;
    }
    if(x == -1 && y == INT_MIN){
        return INTEGER_OVERFLOW;
    }

    stack_push(y / x);
    return SUCCESS;
}


//fügt ein duplikat der obersten eintrags in den stack////overflow
int duplicate(){
    int x;
    int ret1 = stack_peek(&x);
    if(ret1 != 0){
        return ret1;
    }
    int ret2 = stack_push(x);
    if (ret2 != 0){
        return ret2;
    }
    return SUCCESS;
}


//tauscht die letzten beiden einträgge auf dem stack
int swap(){
    int x;
    int y;

    int ret1 = stack_pop(&x);
    if(ret1 != 0) {
        return ret1;
    }
    int ret2 = stack_pop(&y);
    if(ret2 != 0) {
        return ret2;
    }

    stack_push(x);
    stack_push(y);
    return SUCCESS;
}

//print funktion
void myPrint(int number){
    char str[16];
    numToString(number, str);
    printStdout(str);
}