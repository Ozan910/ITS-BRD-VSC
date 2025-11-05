#ifndef FEHLER_H
#define FEHLER_H
//
#define SUCCESS 0
#define STACK_OVERFLOW -1
#define STACK_UNDERFLOW -2
#define STACK_EMPTY -3
#define INTEGER_OVERFLOW -4
#define INTEGER_UNDERFLOW -5
#define ZERO_DIVISION -6

void refresh(int errcode);
#endif
//EOF