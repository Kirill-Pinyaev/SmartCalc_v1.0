#include "pars.h"

int pars(char *str, Stack_t **stack) {
  if (*str == '\0') return EXIT_EXPR_PARSING_ERROR;
  int i = 0;
  int flag = 0;
  char prev = '=';
  char tmp = '=';
  Stack_t *temp_stack = NULL;
  int rc = EXIT_SUCCESS;
  while (*(str + i) != '\0') {
    if (*(str + i) == ' ') {
      ++i;
      continue;
    }
    prev = tmp;
    tmp = *(str + i);
    if (*(str + i) == 'm') {
      if (!isdigit(prev) && prev != 'x' && prev != ')') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      i += 3;
      while (temp_stack != NULL &&
             (temp_stack->data == '*' || temp_stack->data == '/' ||
              temp_stack->data == 'm' || temp_stack->data == '^')) {
        char op;
        double num;
        pop(&temp_stack, &op, &num);
        push(stack, op, num);
      }
      push(&temp_stack, 'm', 0.0);
      continue;
    }
    if (*(str + i) == '^') {
      if (!isdigit(prev) && prev != 'x' && prev != ')') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      ++i;
      while (temp_stack != NULL && temp_stack->data == '^') {
        char op;
        double num;
        pop(&temp_stack, &op, &num);
        push(stack, op, num);
      }
      push(&temp_stack, '^', 0.0);
      continue;
    }
    if (*(str + i) == '*') {
      if (!isdigit(prev) && prev != 'x' && prev != ')') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      ++i;
      while (temp_stack != NULL &&
             (temp_stack->data == '*' || temp_stack->data == '/' ||
              temp_stack->data == 'm' || temp_stack->data == '^')) {
        char op;
        double num;
        pop(&temp_stack, &op, &num);
        push(stack, op, num);
      }
      push(&temp_stack, '*', 0.0);
      continue;
    }
    if (*(str + i) == '/') {
      if (!isdigit(prev) && prev != 'x' && prev != ')') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      ++i;
      while (temp_stack != NULL &&
             (temp_stack->data == '*' || temp_stack->data == '/' ||
              temp_stack->data == 'm' || temp_stack->data == '^')) {
        char op;
        double num;
        pop(&temp_stack, &op, &num);
        push(stack, op, num);
      }
      push(&temp_stack, '/', 0.0);
      continue;
    }
    if (*(str + i) == '+') {
      ++i;
      if (prev == '=' || prev == '(') {
        push(&temp_stack, '&', 0.0);
        continue;
      }
      if (!isdigit(prev) && prev != ')' && prev != 'x') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      while (temp_stack != NULL &&
             (temp_stack->data == '-' || temp_stack->data == '+' ||
              temp_stack->data == '*' || temp_stack->data == '/' ||
              temp_stack->data == 'm' || temp_stack->data == '^')) {
        char op;
        double num;
        pop(&temp_stack, &op, &num);
        push(stack, op, num);
      }
      push(&temp_stack, '+', 0.0);
      continue;
    }
    if (*(str + i) == '-') {
      ++i;
      if (prev == '=' || prev == '(') {
        push(&temp_stack, '_', 0.0);
        continue;
      }
      if (!isdigit(prev) && prev != ')' && prev != 'x') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      while (temp_stack != NULL &&
             (temp_stack->data == '-' || temp_stack->data == '+' ||
              temp_stack->data == '*' || temp_stack->data == '/' ||
              temp_stack->data == 'm' || temp_stack->data == '^')) {
        char op;
        double num;
        pop(&temp_stack, &op, &num);
        push(stack, op, num);
      }
      push(&temp_stack, '-', 0.0);
      continue;
    }
    if (*(str + i) == '(') {
      flag += 1;
      if (prev != '*' && prev != '/' && prev != 'm' && prev != '^' &&
          prev != '(' && prev != '-' && prev != '+' && prev != '=' &&
          prev != 's' && prev != 'c' && prev != 'a' && prev != 't' &&
          prev != 'l') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      ++i;
      push(&temp_stack, '(', 0.0);
      continue;
    }
    if (*(str + i) == ')') {
      flag -= 1;
      if (!isdigit(prev) && prev != 'x' && prev != ')') {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      ++i;
      while (temp_stack != NULL && temp_stack->data != '(') {
        char op;
        double num;
        pop(&temp_stack, &op, &num);
        push(stack, op, num);
      }
      if (temp_stack == NULL) {
        rc = EXIT_EXPR_PARSING_ERROR;
        break;
      }
      char op;
      double num;
      pop(&temp_stack, &op, &num);
      if (temp_stack != NULL) {
        if (temp_stack->data == 's' || temp_stack->data == 'q' ||
            temp_stack->data == 'c' || temp_stack->data == 'k' ||
            temp_stack->data == 'a' || temp_stack->data == 'g' ||
            temp_stack->data == 't' || temp_stack->data == 'l' ||
            temp_stack->data == 'n') {
          pop(&temp_stack, &op, &num);
          push(stack, op, num);
        }
      }
      // push(&temp_stack, ')', 0.0);
      continue;
    }
    if (prev != '*' && prev != '/' && prev != '-' && prev != '+' &&
        prev != 'm' && prev != '^' && prev != '(' && prev != '=') {
      rc = EXIT_EXPR_PARSING_ERROR;
      break;
    }

    if (*(str + i) == 's' && *(str + i + 1) == 'i' && *(str + i + 2) == 'n') {
      i += 3;
      push(&temp_stack, 's', 0.0);
      continue;
    }
    if (*(str + i) == 's' && *(str + i + 1) == 'q' && *(str + i + 2) == 'r' &&
        *(str + i + 3) == 't') {
      i += 4;
      push(&temp_stack, 'q', 0.0);
      continue;
    }
    if (*(str + i) == 'c' && *(str + i + 1) == 'o' && *(str + i + 2) == 's') {
      i += 3;
      push(&temp_stack, 'c', 0.0);
      continue;
    }
    if (*(str + i) == 'a' && *(str + i + 1) == 'c' && *(str + i + 2) == 'o' &&
        *(str + i + 3) == 's') {
      i += 4;
      push(&temp_stack, 'k', 0.0);
      continue;
    }
    if (*(str + i) == 'a' && *(str + i + 1) == 's' && *(str + i + 2) == 'i' &&
        *(str + i + 3) == 'n') {
      i += 4;
      push(&temp_stack, 'a', 0.0);
      continue;
    }
    if (*(str + i) == 'a' && *(str + i + 1) == 't' && *(str + i + 2) == 'a' &&
        *(str + i + 3) == 'n') {
      i += 4;
      push(&temp_stack, 'g', 0.0);
      continue;
    }
    if (*(str + i) == 't' && *(str + i + 1) == 'a' && *(str + i + 2) == 'n') {
      i += 3;
      push(&temp_stack, 't', 0.0);
      continue;
    }
    if (*(str + i) == 'l' && *(str + i + 1) == 'o' && *(str + i + 2) == 'g') {
      i += 3;
      push(&temp_stack, 'l', 0.0);
      continue;
    }
    if (*(str + i) == 'l' && *(str + i + 1) == 'n') {
      i += 2;
      push(&temp_stack, 'n', 0.0);
      continue;
    }
    if (*(str + i) == 'x') {
      ++i;
      push(stack, 'x', 0.0);
      continue;
    }
    char *end;
    double value = strtod(str + i, &end);
    if (end == str - i) {
      rc = EXIT_EXPR_PARSING_ERROR;
      break;
    } else {
      i += end - str - i;
      push(stack, 'd', value);
    }
  }
  if (tmp == '=' && i > 0) rc = EXIT_EXPR_PARSING_ERROR;
  if (temp_stack != NULL && temp_stack->data == '(')
    rc = EXIT_EXPR_PARSING_ERROR;
  else {
    while (temp_stack != NULL) {
      char op;
      double num;
      pop(&temp_stack, &op, &num);
      push(stack, op, num);
    }
  }

  destroy_stack(temp_stack);
  if (flag != 0) rc = EXIT_EXPR_PARSING_ERROR;

  return rc;
}
