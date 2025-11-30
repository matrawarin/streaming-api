#include "db.h"
#include <stdio.h>

sqlite3 *db;

void db_init() {
    if (sqlite3_open("database.db", &db)) {
        printf("Cannot open database.\n");
        exit(1);
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "id INTEGER PRIMARY KEY,"
        "balance REAL DEFAULT 0.0);"

        "CREATE TABLE IF NOT EXISTS ledger ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type TEXT, amount REAL, account_from INTEGER, account_to INTEGER,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);"

        "CREATE TABLE IF NOT EXISTS audit ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "action TEXT, detail TEXT,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    char *err;
    sqlite3_exec(db, sql, 0, 0, &err);
}
void db_close() { sqlite3_close(db); }

