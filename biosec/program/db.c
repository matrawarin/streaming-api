#include <stdio.h>
#include <sqlite3.h>
#include "db.h"

static sqlite3 *db = NULL;

// เชื่อมต่อ SQLite
int db_connect(const char *db_file) {
    if (sqlite3_open(db_file, &db) != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

// ปิด connection
void db_close() {
    if (db) sqlite3_close(db);
}

// สร้างตารางบัญชีและ ledger ถ้ายังไม่มี
void db_init() {
    const char *sql_account = "CREATE TABLE IF NOT EXISTS accounts ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "name TEXT NOT NULL, "
                              "balance REAL NOT NULL DEFAULT 0);";

    const char *sql_ledger = "CREATE TABLE IF NOT EXISTS ledger ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "account_id INTEGER, "
                             "type TEXT, "
                             "amount REAL, "
                             "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
                             "FOREIGN KEY(account_id) REFERENCES accounts(id));";

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql_account, 0, 0, &err_msg) != SQLITE_OK) {
        printf("SQL error (accounts): %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    if (sqlite3_exec(db, sql_ledger, 0, 0, &err_msg) != SQLITE_OK) {
        printf("SQL error (ledger): %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// เพิ่มบัญชีใหม่
int db_insert_account(Account acc) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO accounts (name, balance) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;
    sqlite3_bind_text(stmt, 1, acc.name, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, acc.balance);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}

// อัพเดต balance
int db_update_account_balance(int account_id, double new_balance) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE accounts SET balance=? WHERE id=?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;
    sqlite3_bind_double(stmt, 1, new_balance);
    sqlite3_bind_int(stmt, 2, account_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}

// เพิ่ม transaction ลง ledger
int db_insert_transaction(int account_id, const char *type, double amount) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO ledger (account_id, type, amount) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;
    sqlite3_bind_int(stmt, 1, account_id);
    sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, amount);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}

