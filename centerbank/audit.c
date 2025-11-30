#include "audit.h"
#include "db.h"

void audit_init() {}

void audit_log(const char *action, const char *detail) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db,
        "INSERT INTO audit(action, detail) VALUES(?,?);",
        -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, action, -1, 0);
    sqlite3_bind_text(stmt, 2, detail, -1, 0);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

