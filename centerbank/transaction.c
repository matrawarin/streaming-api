#include "transaction.h"
#include "account.h"
#include "ledger.h"
#include "audit.h"

void transaction_init() {}

void transaction_deposit(long id, double amount) {
    double bal = account_get_balance(id);
    bal += amount;
    account_update_balance(id, bal);

    ledger_add("DEPOSIT", amount, id, 0);
    audit_log("deposit", "deposit success");

    printf("Deposit %.2f into %ld done.\n", amount, id);
}

void transaction_withdraw(long id, double amount) {
    double bal = account_get_balance(id);
    if (bal < amount) {
        printf("Not enough balance.\n");
        return;
    }
    bal -= amount;
    account_update_balance(id, bal);

    ledger_add("WITHDRAW", amount, id, 0);
    audit_log("withdraw", "withdraw success");

    printf("Withdraw %.2f from %ld done.\n", amount, id);
}

void transaction_transfer(long from, long to, double amount) {
    double balFrom = account_get_balance(from);
    if (balFrom < amount) {
        printf("Not enough balance.\n");
        return;
    }

    account_update_balance(from, balFrom - amount);
    double balTo = account_get_balance(to);
    account_update_balance(to, balTo + amount);

    ledger_add("TRANSFER", amount, from, to);
    audit_log("transfer", "transfer success");

    printf("Transfer %.2f from %ld → %ld ok.\n", amount, from, to);
}

