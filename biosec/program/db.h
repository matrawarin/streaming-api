#ifndef DB_H
#define DB_H

#include "account.h"

// Database functions
int db_connect(const char *db_file);
void db_close();
void db_init(); // สร้างตารางถ้ายังไม่มี
int db_insert_account(Account acc);
int db_update_account_balance(int account_id, double new_balance);
int db_insert_transaction(int account_id, const char *type, double amount);

#endif

