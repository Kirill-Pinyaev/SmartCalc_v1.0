#include "calculate.h"

int solve_expression(char *expression, double x, double *answer) {
  int return_code = EXIT_SUCCESS;
  Stack_t *stack = NULL;
  return_code = pars(expression, &stack);
  if (!return_code) {
    Stack_t *reversed_stack = NULL;
    reverse_stack(stack, &reversed_stack);
    return_code = calc(reversed_stack, x, answer);
    destroy_stack(reversed_stack);
  }

  destroy_stack(stack);
  return return_code;
}

int calc(Stack_t *stack, double x, double *answer) {
  char symb;
  double num;
  Stack_t *ptr = stack;
  Stack_t *temp_stack = NULL;
  int rc = EXIT_SUCCESS;
  while (ptr != NULL) {
    num = ptr->value;
    symb = ptr->data;
    switch (symb) {
      case '+': {
        double num1, num2;
        char symb1, symb2;

        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb2, &num2);
        push(&temp_stack, 'd', num1 + num2);
        break;
      }
      case '-': {
        double num1, num2;
        char symb1, symb2;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb2, &num2);
        push(&temp_stack, 'd', num2 - num1);
        break;
      }
      case '_': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        push(&temp_stack, 'd', -num1);
        break;
      }
      case '&': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        push(&temp_stack, 'd', +num1);
        break;
      }
      case '*': {
        double num1, num2;
        char symb1, symb2;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb2, &num2);
        push(&temp_stack, 'd', num1 * num2);
        break;
      }
      case '^': {
        double num1, num2;
        char symb1, symb2;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb2, &num2);
        if (fabs(num2) < eps && num1 < 0) {
          rc = EXIT_POW_ZERO_NEG_EXP_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', pow(num2, num1));
        break;
      }
      case '/': {
        double num1, num2;
        char symb1, symb2;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb2, &num2);
        if (fabs(num1) < eps) {
          rc = EXIT_ZERO_DIVISION_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', num2 / num1);
        break;
      }
      case 'm': {
        double num1, num2;
        char symb1, symb2;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb2, &num2);
        if (fabs(num1) < eps) {
          rc = EXIT_ZERO_DIVISION_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', fmod(num2, num1));
        break;
      }
      case 's': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        push(&temp_stack, 'd', sin(num1));
        break;
      }
      case 'q': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (num1 < 0.0) {
          rc = EXIT_NEG_ROOT_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', sqrt(num1));
        break;
      }
      case 't': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (fabs(fmod(num1, PI) - PI / 2) < eps ||
            fabs(fmod(num1, PI) + PI / 2) < eps) {
          rc = EXIT_INF_TAN_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', tan(num1));
        break;
      }
      case 'n': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (num1 <= 0.0) {
          rc = EXIT_NON_POS_LOG_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', log(num1));
        break;
      }
      case 'c': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        push(&temp_stack, 'd', cos(num1));
        break;
      }
      case 'a': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (fabs(num1) > 1.0) {
          rc = EXIT_ARCSIN_RANGE_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', asin(num1));
        break;
      }
      case 'k': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (fabs(num1) > 1.0) {
          rc = EXIT_ARCCOS_RANGE_ERROR;
          ptr = NULL;
          break;
        }
        push(&temp_stack, 'd', acos(num1));
        break;
      }
      case 'g': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        push(&temp_stack, 'd', atan(num1));
        break;
      }
      case 'l': {
        double num1;
        char symb1;
        if (!temp_stack) {
          rc = EXIT_EXPR_PARSING_ERROR;
          ptr = NULL;
          break;
        }
        pop(&temp_stack, &symb1, &num1);
        if (num1 <= 0.0) {
          rc = EXIT_NON_POS_LOG_ERROR;
          ptr = NULL;

          break;
        }
        push(&temp_stack, 'd', log10(num1));
        break;
      }
      case 'd': {
        push(&temp_stack, 'd', num);
        break;
      }
      case 'x': {
        push(&temp_stack, 'd', x);
        break;
      }
      default:
        break;
    }
    if (ptr) ptr = ptr->prev;
  }
  if (!rc) pop(&temp_stack, &symb, &num);
  *answer = num;
  destroy_stack(temp_stack);
  return rc;
}

int get_graph(char *expression, double l, double r, double step,
              double coordinates[][2]) {
  if (l >= r) return EXIT_EDGES_ERROR;
  int return_code = EXIT_SUCCESS;
  Stack_t *stack = NULL;
  return_code = pars(expression, &stack);
  if (!return_code) {
    Stack_t *reversed_stack = NULL;
    reverse_stack(stack, &reversed_stack);
    draw(reversed_stack, l, r, step, coordinates);
    destroy_stack(reversed_stack);
  }

  destroy_stack(stack);
  return return_code;
}

void draw(Stack_t *stack, double l, double r, double step, double coords[][2]) {
  int i = 0;
  for (double x = l; x <= r; x += step) {
    double y = 0;
    int rc = calc(stack, x, &y);
    coords[i][0] = x;
    coords[i][1] = rc ? NAN : y;
    i++;
  }
}
