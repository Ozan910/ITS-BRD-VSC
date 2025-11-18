#ifndef STACK_H
#define STACK_H

/**
* @brief resetting the stack
* 
* @return SUCCESS
*/
int stack_reset(void);

/**
* @brief push a value on the stack
* @param val: the value that needs to be pushed
* @return SUCCESS, STACK_OVERFLOW
*/
int stack_push(int val);

/**
* @brief pops a value from the stack
* @param val: &val the adress of the variable the value will be stored in
* @return SUCCESS, STACK_UNDERFLOW
*/
int stack_pop(int *val);

/**
* @brief returns the top value of the stack without removing it
* @param val: &val the adress of the variable the value will be stored in
* @return SUCCESS, STACK_EMPTY
*/
int stack_peek(int *val);

/**
* @brief returns the current size of the stack
* 
* @return SUCCESS
*/
int getCount(void);

/**
* @brief returns an Element from the stack at the given index
* @param index: index (0: Bottom of the Stack, STACK_SIZE: Top of the Stack)
* @param val: &val the adress of the variable the value will be stored in
* @return the element at the requested position
*/
int stack_getElement(int index, int *val);

#endif
//EOF