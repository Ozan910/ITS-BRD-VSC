#ifndef STACK_H
#define STACK_H
#define STACK_SIZE 14

int stack_reset(void);
int stack_push(int val);
int stack_pop(int *val);
int stack_peek(int *val);
int getCount(void);

#endif
//EOF