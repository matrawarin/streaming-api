import java.util.Scanner;

public class Main {

    private static Scanner sc = new Scanner(System.in);
    private static Database db = new Database();

    public static void main(String[] args) {
        try {
            // เชื่อมต่อ database และสร้างตารางถ้ายังไม่มี
            if (!db.dbConnect("fund.db")) {
                System.out.println("Database connection failed!");
                return;
            }
            db.dbInit();

            int choice;
            do {
                showMenu();
                choice = getIntInput("Enter your choice: ");

                switch (choice) {
                    case 1:
                        createAccount();
                        break;
                    case 2:
                        deposit();
                        break;
                    case 3:
                        withdraw();
                        break;
                    case 4:
                        listAccounts();
                        break;
                    case 5:
                        showLedger();
                        break;
                    case 0:
                        System.out.println("Exiting program. Goodbye!");
                        break;
                    default:
                        System.out.println("Invalid choice. Please try again.");
                }

            } while (choice != 0);

        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            db.dbClose();
            sc.close();
        }
    }

    private static void showMenu() {
        System.out.println("\n==============================");
        System.out.println("      FUND MANAGEMENT SYSTEM  ");
        System.out.println("==============================");
        System.out.println("1. Create Account");
        System.out.println("2. Deposit");
        System.out.println("3. Withdraw");
        System.out.println("4. Show Accounts");
        System.out.println("5. Show Ledger");
        System.out.println("0. Exit");
    }

    private static int getIntInput(String prompt) {
        System.out.print(prompt);
        while (!sc.hasNextInt()) {
            sc.next(); // ล้างค่าไม่ใช่ int
            System.out.print("Invalid input. " + prompt);
        }
        return sc.nextInt();
    }

    private static double getDoubleInput(String prompt) {
        System.out.print(prompt);
        while (!sc.hasNextDouble()) {
            sc.next(); // ล้างค่าไม่ใช่ double
            System.out.print("Invalid input. " + prompt);
        }
        return sc.nextDouble();
    }

    private static void createAccount() throws Exception {
        sc.nextLine(); // ล้าง newline
        System.out.print("Enter account name: ");
        String name = sc.nextLine();
        Account acc = new Account(0, name);
        if (db.dbInsertAccount(acc)) {
            System.out.println("Account created successfully!");
        } else {
            System.out.println("Failed to create account.");
        }
    }

    private static void deposit() throws Exception {
        int id = getIntInput("Enter account ID to deposit: ");
        double amount = getDoubleInput("Enter amount to deposit: ");
        Account acc = db.dbGetAccount(id);
        if (acc == null) {
            System.out.println("Account not found!");
            return;
        }
        acc.deposit(amount);
        db.dbUpdateAccountBalance(acc.getId(), acc.getBalance());
        db.dbInsertTransaction(acc.getId(), "DEPOSIT", amount);
        System.out.println("Deposit successful! New balance: " + acc.getBalance());
    }

    private static void withdraw() throws Exception {
        int id = getIntInput("Enter account ID to withdraw: ");
        double amount = getDoubleInput("Enter amount to withdraw: ");
        Account acc = db.dbGetAccount(id);
        if (acc == null) {
            System.out.println("Account not found!");
            return;
        }
        if (amount > acc.getBalance()) {
            System.out.println("Insufficient balance! Current balance: " + acc.getBalance());
            return;
        }
        acc.withdraw(amount);
        db.dbUpdateAccountBalance(acc.getId(), acc.getBalance());
        db.dbInsertTransaction(acc.getId(), "WITHDRAW", amount);
        System.out.println("Withdrawal successful! New balance: " + acc.getBalance());
    }

    private static void listAccounts() throws Exception {
        db.dbListAccounts();
    }

    private static void showLedger() throws Exception {
        db.dbShowLedger();
    }
}

