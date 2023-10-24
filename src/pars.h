#ifndef __EXPRESSION_PARSING_H__
#define __EXPRESSION_PARSING_H__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

#define EXIT_EXPR_PARSING_ERROR 20

int pars(char *str, Stack_t **stack);

#endif  // __EXPRESSION_PARSING_H__
