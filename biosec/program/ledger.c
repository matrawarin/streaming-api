#include <stdio.h>
#include <string.h>
#include "ledger.h"

#define MAX_TRANSACTIONS 1000
#define LEDGER_FILE "ledger.dat"

typedef struct {
    int account_id;
    char type[20]; // "DEPOSIT" / "WITHDRAW"
    double amount;
} Transaction;

static Transaction transactions[MAX_TRANSACTIONS];
static int transaction_count = 0;

// โหลด ledger จากไฟล์
void initialize_ledger() {
    FILE *file = fopen(LEDGER_FILE, "rb");
    if (file) {
        fread(&transaction_count, sizeof(int), 1, file);
        fread(transactions, sizeof(Transaction), transaction_count, file);
        fclose(file);
    } else {
        transaction_count = 0;
    }
}

// บันทึก ledger ลงไฟล์
void save_ledger() {
    FILE *file = fopen(LEDGER_FILE, "wb");
    if (file) {
        fwrite(&transaction_count, sizeof(int), 1, file);
        fwrite(transactions, sizeof(Transaction), transaction_count, file);
        fclose(file);
    }
}

// เพิ่ม transaction ใหม่
void add_transaction(int account_id, const char *type, double amount) {
    if (transaction_count >= MAX_TRANSACTIONS) {
        printf("Ledger full! Cannot add more transactions.\n");
        return;
    }
    transactions[transaction_count].account_id = account_id;
    strncpy(transactions[transaction_count].type, type, sizeof(transactions[transaction_count].type) - 1);
    transactions[transaction_count].type[sizeof(transactions[transaction_count].type) - 1] = '\0';
    transactions[transaction_count].amount = amount;
    transaction_count++;
    save_ledger(); // save every transaction
}

// แสดง ledger ทั้งหมด
void show_ledger() {
    if (transaction_count == 0) {
        printf("Ledger is empty.\n");
        return;
    }

    printf("\n%-10s %-10s %-10s\n", "AccountID", "Type", "Amount");
    printf("-------------------------------\n");
    for (int i = 0; i < transaction_count; i++) {
        printf("%-10d %-10s %.2f\n", 
            transactions[i].account_id, 
            transactions[i].type, 
            transactions[i].amount);
    }
}

