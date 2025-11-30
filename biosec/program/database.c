#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

sqlite3 *db;

int init_db() {
    int rc = sqlite3_open("users.db", &db);
    if(rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    char *sql = "CREATE TABLE IF NOT EXISTS users("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "username TEXT UNIQUE,"
                "password TEXT);";
    char *err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    return 1;
}

int register_user(const char* username, const char* password) {
    char sql[256];
    sprintf(sql, "INSERT INTO users (username, password) VALUES ('%s','%s');", username, password);
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        printf("Error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    return 1;
}

int login_user(const char* username, const char* password) {
    sqlite3_stmt *res;
    char sql[256];
    sprintf(sql, "SELECT * FROM users WHERE username='%s' AND password='%s';", username, password);
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if(rc != SQLITE_OK) return 0;
    rc = sqlite3_step(res);
    int success = rc == SQLITE_ROW;
    sqlite3_finalize(res);
    return success;
}

