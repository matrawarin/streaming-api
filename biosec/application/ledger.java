import java.util.ArrayList;
import java.util.List;

public class Ledger {

    public static class Transaction {
        private int accountId;
        private String type; // "DEPOSIT" or "WITHDRAW"
        private double amount;

        public Transaction(int accountId, String type, double amount) {
            this.accountId = accountId;
            this.type = type;
            this.amount = amount;
        }

        public int getAccountId() {
            return accountId;
        }

        public String getType() {
            return type;
        }

        public double getAmount() {
            return amount;
        }

        public void display() {
            System.out.printf("AccountID: %d, Type: %s, Amount: %.2f\n", accountId, type, amount);
        }
    }

    private List<Transaction> transactions;

    public Ledger() {
        transactions = new ArrayList<>();
    }

    // เพิ่ม Transaction ใหม่
    public void addTransaction(int accountId, String type, double amount) {
        Transaction tx = new Transaction(accountId, type, amount);
        transactions.add(tx);
        System.out.println("Transaction added successfully.");
    }

    // แสดง Transaction ทั้งหมด
    public void showLedger() {
        if (transactions.isEmpty()) {
            System.out.println("Ledger is empty.");
            return;
        }
        System.out.println("\n--- Ledger ---");
        for (Transaction tx : transactions) {
            tx.display();
        }
    }

    // ดึง Transaction ทั้งหมด (สำหรับ Database integration)
    public List<Transaction> getTransactions() {
        return transactions;
    }
}

