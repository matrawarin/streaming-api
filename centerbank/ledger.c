#include "ledger.h"
#include "db.h"
#include <stdio.h>

void ledger_init() {}

void ledger_add(const char *type, double amount, long from, long to) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db,
        "INSERT INTO ledger(type, amount, account_from, account_to)"
        "VALUES(?,?,?,?);",
        -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, type, -1, 0);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_int64(stmt, 3, from);
    sqlite3_bind_int64(stmt, 4, to);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void ledger_show() {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db,
        "SELECT type, amount, account_from, account_to, timestamp FROM ledger;",
        -1, &stmt, 0);

    printf("\n--- Ledger Report ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%s  amount=%.2f  from=%d  to=%d  time=%s\n",
            sqlite3_column_text(stmt, 0),
            sqlite3_column_double(stmt, 1),
            sqlite3_column_int(stmt, 2),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_text(stmt, 4));
    }
    sqlite3_finalize(stmt);
}

