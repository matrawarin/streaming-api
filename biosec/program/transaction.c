#include <stdio.h>
#include "account.h"
#include "ledger.h"

// ฝากเงิน
void deposit() {
    int id;
    double amount;
    printf("Enter account ID to deposit: ");
    scanf("%d", &id);

    Account *acc = find_account(id);
    if (!acc) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }

    acc->balance += amount;
    printf("Deposit successful! New balance: %.2f\n", acc->balance);

    // บันทึกลง ledger
    add_transaction(acc->id, "DEPOSIT", amount);
}

// ถอนเงิน
void withdraw() {
    int id;
    double amount;
    printf("Enter account ID to withdraw: ");
    scanf("%d", &id);

    Account *acc = find_account(id);
    if (!acc) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }

    if (amount > acc->balance) {
        printf("Insufficient balance! Current balance: %.2f\n", acc->balance);
        return;
    }

    acc->balance -= amount;
    printf("Withdrawal successful! New balance: %.2f\n", acc->balance);

    // บันทึกลง ledger
    add_transaction(acc->id, "WITHDRAW", amount);
}

