#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"

#define DATA_FILE "accounts.dat"   // ไฟล์เก็บข้อมูลบัญชี
#define MAX_ACCOUNTS 100

Account accounts[MAX_ACCOUNTS];
int account_count = 0;

// โหลดข้อมูลบัญชีจากไฟล์
void initialize_system() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file) {
        fread(&account_count, sizeof(int), 1, file);
        fread(accounts, sizeof(Account), account_count, file);
        fclose(file);
    } else {
        account_count = 0; // ถ้าไฟล์ไม่มี เริ่มจากศูนย์
    }
}

// บันทึกข้อมูลบัญชีลงไฟล์
void save_and_exit() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file) {
        fwrite(&account_count, sizeof(int), 1, file);
        fwrite(accounts, sizeof(Account), account_count, file);
        fclose(file);
        printf("Accounts saved successfully.\n");
    } else {
        printf("Error saving accounts!\n");
    }
}

// สร้างบัญชีใหม่
void create_account() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached!\n");
        return;
    }

    Account new_acc;
    new_acc.id = account_count + 1;
    printf("Enter account name: ");
    getchar(); // ล้าง newline จาก buffer
    fgets(new_acc.name, sizeof(new_acc.name), stdin);
    // ลบ newline ที่ fgets ใส่ให้
    new_acc.name[strcspn(new_acc.name, "\n")] = 0;
    new_acc.balance = 0.0;

    accounts[account_count] = new_acc;
    account_count++;

    printf("Account created successfully! ID: %d, Name: %s\n", new_acc.id, new_acc.name);
}

// แสดงบัญชีทั้งหมด
void list_accounts() {
    if (account_count == 0) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n%-5s %-20s %-10s\n", "ID", "Name", "Balance");
    printf("-------------------------------------\n");
    for (int i = 0; i < account_count; i++) {
        printf("%-5d %-20s %.2f\n", accounts[i].id, accounts[i].name, accounts[i].balance);
    }
}

// ค้นหาบัญชีด้วย ID
Account* find_account(int id) {
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == id) {
            return &accounts[i];
        }
    }
    return NULL;
}

