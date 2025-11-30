#include <stdio.h>
#include <stdlib.h>
#include "db.h"
#include "ledger.h"
#include "account.h"
#include "transaction.h"
#include "audit.h"

void menu() {
    printf("\n--- Central Bank Financial System ---\n");
    printf("1. Create account\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Transfer\n");
    printf("5. Show account balance\n");
    printf("6. Ledger report\n");
    printf("0. Exit\n");
}

int main() {
    db_init();
    ledger_init();
    account_init();
    transaction_init();
    audit_init();

    int choice;
    while (1) {
        menu();
        printf("Select: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        long id, id2;
        double amount;

        switch (choice) {
            case 1:
                printf("Enter new account id: ");
                scanf("%ld", &id);
                account_create(id);
                break;

            case 2:
                printf("Account ID: ");
                scanf("%ld", &id);
                printf("Amount: ");
                scanf("%lf", &amount);
                transaction_deposit(id, amount);
                break;

            case 3:
                printf("Account ID: ");
                scanf("%ld", &id);
                printf("Amount: ");
                scanf("%lf", &amount);
                transaction_withdraw(id, amount);
                break;

            case 4:
                printf("From ID: ");
                scanf("%ld", &id);
                printf("To ID: ");
                scanf("%ld", &id2);
                printf("Amount: ");
                scanf("%lf", &amount);
                transaction_transfer(id, id2, amount);
                break;

            case 5:
                printf("Account ID: ");
                scanf("%ld", &id);
                account_show_balance(id);
                break;

            case 6:
                ledger_show();
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    db_close();
    return 0;
}

