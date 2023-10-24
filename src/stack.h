#ifndef SHOOL21_STACK_H_
#define SHOOL21_STACK_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct Stack_s Stack_t;

struct Stack_s {
  char data;
  double value;
  Stack_t *prev;
};

void pop(Stack_t **stack, char *symb, double *num);
void push(Stack_t **stack, char symb, double num);
void reverse_stack(Stack_t *old, Stack_t **new);
void destroy_stack(Stack_t *st);

#endif  // SHOOL21_STACK_H_
