/**
 * kyc.c
 * KYC management for banking system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "kyc.h"
#include "db.h"

// Initialize KYC module
int kyc_init() {
    // Currently nothing special to init
    return 1;
}

// Add or update KYC info
int kyc_add(const char *acc_number, const char *first_name, const char *last_name,
            const char *dob, const char *id_number, const char *address,
            const char *phone, const char *email) {
    
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO kyc(acc_number, first_name, last_name, dob, id_number, address, phone, email) "
                      "VALUES(?, ?, ?, ?, ?, ?, ?, ?) "
                      "ON CONFLICT(acc_number) DO UPDATE SET "
                      "first_name=excluded.first_name, last_name=excluded.last_name, "
                      "dob=excluded.dob, id_number=excluded.id_number, address=excluded.address, "
                      "phone=excluded.phone, email=excluded.email;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, acc_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, first_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, last_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, dob, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, address, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, email, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert/update KYC: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

// Add or update face template
int kyc_add_face(const char *acc_number, const unsigned char *face_data, size_t face_size) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE kyc SET face_template=? WHERE acc_number=?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_blob(stmt, 1, face_data, (int)face_size, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, acc_number, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to update face template: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

// Show KYC info
void kyc_show(const char *acc_number) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT first_name, last_name, dob, id_number, address, phone, email FROM kyc WHERE acc_number=?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, acc_number, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) == SQLITE_ROW) {
        printf("===== KYC Info =====\n");
        printf("First Name: %s\n", sqlite3_column_text(stmt,0));
        printf("Last Name: %s\n", sqlite3_column_text(stmt,1));
        printf("DOB: %s\n", sqlite3_column_text(stmt,2));
        printf("ID Number: %s\n", sqlite3_column_text(stmt,3));
        printf("Address: %s\n", sqlite3_column_text(stmt,4));
        printf("Phone: %s\n", sqlite3_column_text(stmt,5));
        printf("Email: %s\n", sqlite3_column_text(stmt,6));
    } else {
        printf("No KYC info found for account %s\n", acc_number);
    }

    sqlite3_finalize(stmt);
}

