public class Account {
    private int id;
    private String name;
    private double balance;
    private int sectionId; // กองทุนหรือแผนก
    private int sectorId;  // Sector ของกองทุน
    private int codeId;    // รหัสประเภทบัญชี

    // Constructor
    public Account(int id, String name) {
        this.id = id;
        this.name = name;
        this.balance = 0.0;
        this.sectionId = 0;
        this.sectorId = 0;
        this.codeId = 0;
    }

    // Constructor แบบเต็ม
    public Account(int id, String name, double balance, int sectionId, int sectorId, int codeId) {
        this.id = id;
        this.name = name;
        this.balance = balance;
        this.sectionId = sectionId;
        this.sectorId = sectorId;
        this.codeId = codeId;
    }

    // Deposit
    public void deposit(double amount) {
        if (amount <= 0) {
            System.out.println("Invalid amount for deposit.");
            return;
        }
        this.balance += amount;
    }

    // Withdraw
    public void withdraw(double amount) {
        if (amount <= 0) {
            System.out.println("Invalid amount for withdrawal.");
            return;
        }
        if (amount > this.balance) {
            System.out.println("Insufficient balance.");
            return;
        }
        this.balance -= amount;
    }

    // Getters & Setters
    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public double getBalance() {
        return balance;
    }

    public int getSectionId() {
        return sectionId;
    }

    public void setSectionId(int sectionId) {
        this.sectionId = sectionId;
    }

    public int getSectorId() {
        return sectorId;
    }

    public void setSectorId(int sectorId) {
        this.sectorId = sectorId;
    }

    public int getCodeId() {
        return codeId;
    }

    public void setCodeId(int codeId) {
        this.codeId = codeId;
    }

    // แสดงข้อมูลบัญชี
    public void display() {
        System.out.printf("ID: %d, Name: %s, Balance: %.2f, SectionID: %d, SectorID: %d, CodeID: %d\n",
                id, name, balance, sectionId, sectorId, codeId);
    }
}

