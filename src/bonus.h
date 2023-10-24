#ifndef __BONUS_PART_H__
#define __BONUS_PART_H__

#define EXIT_SUCCESS 0
#define EXIT_INPUT_ERROR 2

#include <math.h>
#include <stdio.h>

#define eps 1e-7

#define EXIT_CREDIT_INPUT_ERROR 30
#define EXIT_DEPOSIT_INPUT_ERROR 31

int calc_credit(double sum, int period, double bet, int flag,
                double *payment_month, double *overpayment,
                double *total_payment);
int calc_deposit(double deposit_amount, int month_term, double interest_rate,
                 double tax_rate, int repayment_periodicity,
                 int interest_capitalization, double replenishment[][2],
                 int withdraw_count, double withdraw[][2],
                 int replenishment_count, double *accrued_interest,
                 double *tax_amount, double *amount_on_deposit);

#endif  //__BONUS_PART_H__
