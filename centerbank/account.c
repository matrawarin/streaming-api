#include "account.h"
#include "db.h"

void account_init() {}

void account_create(long id) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "INSERT INTO accounts(id) VALUES(?);", -1, &stmt, 0);
    sqlite3_bind_int64(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("Created account %ld\n", id);
}

double account_get_balance(long id) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT balance FROM accounts WHERE id=?;", -1, &stmt, 0);
    sqlite3_bind_int64(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        double bal = sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);
        return bal;
    }
    sqlite3_finalize(stmt);
    return -1;
}

void account_update_balance(long id, double amount) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "UPDATE accounts SET balance=? WHERE id=?;", -1, &stmt, 0);
    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_int64(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void account_show_balance(long id) {
    printf("Balance of %ld = %.2f\n", id, account_get_balance(id));
}

