#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "transaction.h"
#include "ledger.h"

int main() {
    int choice;

    // โหลดข้อมูลเริ่มต้น หรือสร้างระบบใหม่
    initialize_system();

    do {
        printf("\n==============================\n");
        printf("      FUND MANAGEMENT SYSTEM  \n");
        printf("==============================\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Show Accounts\n");
        printf("5. Show Ledger\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        if(scanf("%d", &choice) != 1) {
            // ป้องกัน input ผิด
            while(getchar() != '\n'); // ล้าง buffer
            choice = -1;
        }

        switch(choice) {
            case 1:
                create_account();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                list_accounts();
                break;
            case 5:
                show_ledger();
                break;
            case 0:
                save_and_exit();
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please select again.\n");
        }

    } while(choice != 0);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "transaction.h"
#include "ledger.h"

int main() {
    int choice;

    // โหลดข้อมูลเริ่มต้น หรือสร้างระบบใหม่
    initialize_system();

    do {
        printf("\n==============================\n");
        printf("      FUND MANAGEMENT SYSTEM  \n");
        printf("==============================\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Show Accounts\n");
        printf("5. Show Ledger\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        if(scanf("%d", &choice) != 1) {
            // ป้องกัน input ผิด
            while(getchar() != '\n'); // ล้าง buffer
            choice = -1;
        }

        switch(choice) {
            case 1:
                create_account();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                list_accounts();
                break;
            case 5:
                show_ledger();
                break;
            case 0:
                save_and_exit();
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please select again.\n");
        }

    } while(choice != 0);

    return 0;
}

