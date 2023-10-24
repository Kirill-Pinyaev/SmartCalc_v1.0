#include "stack.h"

void push(Stack_t **stack, char symb, double num) {
  Stack_t *temp = (Stack_t *)malloc(sizeof(Stack_t));
  temp->data = symb;
  temp->value = num;
  temp->prev = *stack;
  *stack = temp;
}

void pop(Stack_t **stack, char *symb, double *num) {
  Stack_t *temp = (*stack)->prev;
  *symb = (*stack)->data;
  *num = (*stack)->value;
  free(*stack);
  *stack = temp;
}

void reverse_stack(Stack_t *old, Stack_t **new) {
  Stack_t *temp = old;
  while (temp != NULL) {
    push(new, temp->data, temp->value);
    temp = temp->prev;
  }
}

void delete_element(Stack_t *elem) {
  Stack_t *tmp = elem->prev;
  free(elem);
  if (tmp) delete_element(tmp);
}

void destroy_stack(Stack_t *st) {
  if (!st) return;
  if (st->prev) delete_element(st->prev);
  free(st);
}
