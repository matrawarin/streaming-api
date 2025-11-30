#ifndef ACCOUNT_H
#define ACCOUNT_H

typedef struct {
    int id;
    char name[50];
    double balance;
} Account;

void create_account();
void list_accounts();
Account* find_account(int id);
void initialize_system();
void save_and_exit();

#endif

