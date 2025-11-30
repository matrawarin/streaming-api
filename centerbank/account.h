#ifndef ACCOUNT_H
#define ACCOUNT_H

// Create new account
int account_create(const char *acc_number, const char *owner_name, double initial_balance);

// Delete account
int account_delete(const char *acc_number);

// Get account balance
double account_get_balance(const char *acc_number);

// Show account information
void account_show(const char *acc_number);

// Initialize account module
int account_init();

#endif

