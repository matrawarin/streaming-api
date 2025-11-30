#ifndef LEDGER_H
#define LEDGER_H

// Add a ledger entry (debit, credit)
int ledger_add(const char *acc_number, double debit, double credit);

// Show all ledger entries for an account
void ledger_show(const char *acc_number);

// Initialize ledger module
int ledger_init();

#endif

