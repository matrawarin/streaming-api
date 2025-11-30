#ifndef TRANSACTION_H
#define TRANSACTION_H

// Deposit money into account
int transaction_deposit(const char *acc_number, double amount);

// Withdraw money from account
int transaction_withdraw(const char *acc_number, double amount);

// Transfer between two accounts
int transaction_transfer(const char *from_acc, const char *to_acc, double amount);

// Initialize transaction system
int transaction_init();

#endif

