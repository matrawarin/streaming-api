#ifndef LEDGER_H
#define LEDGER_H

void initialize_ledger();
void save_ledger();
void add_transaction(int account_id, const char *type, double amount);
void show_ledger();

#endif

