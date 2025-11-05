/**
Stack
 */
#include "stack.h"
#include "fehler.h"



 int stack[STACK_SIZE];
 int count = 0;


 int stack_reset(){
     count = 0;
     return SUCCESS;
 }

 int stack_push(int val){
    if(count >= STACK_SIZE){
        return STACK_OVERFLOW;
    }

    stack[count] = val;
    count++;
    return SUCCESS;
 }

 int stack_pop(int *val){
    if(count <= 0){
        return STACK_UNDERFLOW;
    }

    count--;
    *val = stack[count];
    return SUCCESS;
 }

 int stack_peek(int *val){
    if(count <= 0){
        return STACK_EMPTY;
    }

    *val = stack[count - 1];
    return SUCCESS;
 }

 int getCount(){
    return count;
 }
