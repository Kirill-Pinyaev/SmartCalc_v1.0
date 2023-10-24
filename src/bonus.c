#include "bonus.h"

int calc_credit(double sum, int period, double bet, int flag,
                double *payment_month, double *overpayment,
                double *total_payment) {
  if ((flag != 0 && flag != 1) || sum <= 0 || bet <= 0.0 || period <= 0)
    return EXIT_CREDIT_INPUT_ERROR;

  double monthly_interest_rate = bet / 100.0 / 12;

  if (flag == 0) {
    double pow_factor = pow(1 + monthly_interest_rate, period);
    double p_month =
        sum * monthly_interest_rate * pow_factor / (pow_factor - 1);
    for (int i = 0; i < period; ++i) payment_month[i] = p_month;
    *total_payment = *payment_month * period;
    *overpayment = *total_payment - sum;
  } else {
    *payment_month = 0.0;
    *total_payment = 0.0;
    *overpayment = 0.0;
    double principal_payment = sum / period;

    for (int i = 0; i < period; i++) {
      double interest_payment = sum * monthly_interest_rate;
      sum -= principal_payment;
      payment_month[i] = principal_payment + interest_payment;
      *overpayment += interest_payment;
      *total_payment += payment_month[i];
    }
  }

  return EXIT_SUCCESS;
}

int calc_deposit(double deposit_amount, int month_term, double interest_rate,
                 double tax_rate, int repayment_periodicity,
                 int interest_capitalization, double replenishment[][2],
                 int replenishment_count, double withdraw[][2],
                 int withdraw_count, double *accrued_interest,
                 double *tax_amount, double *amount_on_deposit) {
  if (deposit_amount <= 0 || month_term <= 0 || interest_rate <= 0 ||
      tax_rate < 0 || repayment_periodicity <= 0 ||
      interest_capitalization < 0 || withdraw_count < 0 ||
      replenishment_count < 0)

    return EXIT_DEPOSIT_INPUT_ERROR;
  for (int i = 0; i < replenishment_count; ++i) {
    if (replenishment[i][1] <= 0 || replenishment[i][1] > month_term)

      return EXIT_DEPOSIT_INPUT_ERROR;

    if (replenishment[i][0] <= 0) return EXIT_DEPOSIT_INPUT_ERROR;
  }

  for (int i = 0; i < withdraw_count; ++i) {
    if (withdraw[i][1] <= 0 || withdraw[i][1] > month_term)
      return EXIT_DEPOSIT_INPUT_ERROR;
    if (withdraw[i][0] <= 0) return EXIT_DEPOSIT_INPUT_ERROR;
  }

  double monthly_interest_rate = interest_rate / 100.0 / 12.0;
  double total_interest = 0.0;
  *tax_amount = 0.0;
  *accrued_interest = 0.0;
  tax_rate /= 100;

  for (int month = 1; month <= month_term; month++) {
    double monthly_withdraw = 0.0;

    for (int i = 0; i < replenishment_count; i++) {
      if (fabs(replenishment[i][1] - month) < eps) {
        deposit_amount += replenishment[i][0];
      }
    }
    for (int i = 0; i < withdraw_count; i++) {
      if (fabs(withdraw[i][1] - month) < eps) {
        monthly_withdraw = withdraw[i][0];
        if (monthly_withdraw > deposit_amount) {
          monthly_withdraw = deposit_amount;
        }
        deposit_amount -= monthly_withdraw;
      }
    }

    total_interest += deposit_amount * monthly_interest_rate;

    if (month % repayment_periodicity == 0) {
      *tax_amount += total_interest * tax_rate;
      total_interest -= total_interest * tax_rate;
      if (interest_capitalization > 0) {
        deposit_amount += total_interest;
      }
      *accrued_interest += total_interest;
      total_interest = 0.0;
    }
  }

  *amount_on_deposit = deposit_amount;

  return EXIT_SUCCESS;
}
