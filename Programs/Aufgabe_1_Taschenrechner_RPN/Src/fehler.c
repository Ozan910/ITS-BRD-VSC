#include "fehler.h"
#include "display.h"
#include "scanner.h"
#include "stack.h"

void refresh(int errcode){
    if(errcode == SUCCESS){
        return;
    }
    setErrMode();
    clearStdout();
    switch (errcode){
        
        case STACK_OVERFLOW:
            printStdout("Fehler aufgetreten: Stack Overflow. Bitte C druecken zum resetten");
            break;

        case STACK_UNDERFLOW:
            printStdout("Fehler aufgetreten: Stack Underflow. Bitte C druecken zum resetten");
            break;

        case STACK_EMPTY:
            printStdout("Fehler aufgetreten: Stack ist leer. Bitte C druecken zum resetten");
            break;

        case INTEGER_OVERFLOW:
            printStdout("Fehler aufgetreten: Integer Overflow. Bitte C druecken zum resetten");
            break;

        case INTEGER_UNDERFLOW:
            printStdout("Fehler aufgetreten: Integer Underflow. Bitte C druecken zum resetten");
            break;

        case ZERO_DIVISION:
            printStdout("Fehler aufgetreten: Division durch Null. Bitte C druecken zum resetten ");
            break;

        default:
            printStdout("Fehler aufgetreten. Bitte C drücken zum zurücksetzen");
            break;
    }

    T_token currentToken = nextToken();
    while(currentToken.tok != CLEAR){
       currentToken = nextToken();
    }
    stack_reset();
    clearStdout();
    setNormalMode();
}