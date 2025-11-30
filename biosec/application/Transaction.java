public class Transaction {

    private int accountId;
    private String type; // "DEPOSIT" หรือ "WITHDRAW"
    private double amount;

    // Constructor
    public Transaction(int accountId, String type, double amount) {
        this.accountId = accountId;
        this.type = type;
        this.amount = amount;
    }

    // Getters
    public int getAccountId() {
        return accountId;
    }

    public String getType() {
        return type;
    }

    public double getAmount() {
        return amount;
    }

    // แสดงธุรกรรม
    public void display() {
        System.out.printf("AccountID: %d | Type: %s | Amount: %.2f\n", accountId, type, amount);
    }

    // ประมวลผลธุรกรรมกับบัญชี
    public void process(Account account) {
        if (!account.getIdEquals(accountId)) {
            System.out.println("Transaction account ID mismatch.");
            return;
        }

        if (type.equalsIgnoreCase("DEPOSIT")) {
            account.deposit(amount);
        } else if (type.equalsIgnoreCase("WITHDRAW")) {
            if (amount > account.getBalance()) {
                System.out.println("Insufficient balance for withdrawal.");
            } else {
                account.withdraw(amount);
            }
        } else {
            System.out.println("Invalid transaction type.");
        }
    }
}

