#ifndef DB_H
#define DB_H

#include <sqlite3.h>

// Global database handle
extern sqlite3 *db;

// Initialize database connection (open file)
int db_init(const char *db_path);

// Close database connection
void db_close();

// Execute SQL command without expecting results
int db_exec(const char *sql);

// Check if a table exists
int db_table_exists(const char *table_name);

// Create all banking system tables
int db_create_schema();

#endif

