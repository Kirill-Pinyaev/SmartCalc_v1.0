#ifndef __CALCULATION_H__
#define __CALCULATION_H__

#include <math.h>

#define PI 3.14159265358979323846

#include "bonus.h"
#include "pars.h"
#include "stack.h"

#define eps 1e-7

#define EXIT_POW_ZERO_NEG_EXP_ERROR 10
// The power of zero is undefined for a negative exponent

#define EXIT_ZERO_DIVISION_ERROR 11
// Division by zero is undefined

#define EXIT_NEG_ROOT_ERROR 12
// Taking the root of a negative number is not allowed

#define EXIT_INF_TAN_ERROR 13
// At this point the tangent is undefined

#define EXIT_NON_POS_LOG_ERROR 14
// The logarithm is undefined for non-positive numbers

#define EXIT_ARCCOS_RANGE_ERROR 15
// Inverse cosine is undefined for values outside [-1, 1]

#define EXIT_ARCSIN_RANGE_ERROR 16
// Inverse cosine is undefined for values outside [-1, 1]

#define EXIT_EXPR_PARSING_ERROR 20

#define EXIT_EDGES_ERROR 40

int calc(Stack_t *stack, double x, double *answer);
int solve_expression(char *expression, double x, double *answer);
int get_graph(char *expression, double l, double r, double step,
              double coordinates[][2]);
void draw(Stack_t *stack, double l, double r, double step, double coords[][2]);
//  void button_draw_graph(GtkWidget *button, PlotData *plotData);
//  void go_flag(GtkWidget *button, UserData *userData);
// gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data);
//  void draw_axes_and_grid(cairo_t *cr, double width, double height, double
//  min_x,
//                          double max_x, double min_y, double max_y, int
//                          count);
#endif  // __CALCULATION_H__
