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
    printStdout("An Error has occured");
    printStdout("\n");
    switch (errcode){
        
        case STACK_OVERFLOW:
            printStdout("Stack Overflow");
            break;

        case STACK_UNDERFLOW:
            printStdout("Stack Underflow");
            break;

        case STACK_EMPTY:
            printStdout("Stack is empty");
            break;

        case INTEGER_OVERFLOW:
            printStdout("Integer Overflow");
            break;

        case INTEGER_UNDERFLOW:
            printStdout("Integer Underflow");
            break;

        case ZERO_DIVISION:
            printStdout("Zero Division");
            break;

        default:
            printStdout(" ");
            break;
    }
    printStdout("\n");
    printStdout("press C to reset");
    T_token currentToken = nextToken();
    while(currentToken.tok != CLEAR){
       currentToken = nextToken();
    }
    stack_reset();
    clearStdout();
    setNormalMode();
}