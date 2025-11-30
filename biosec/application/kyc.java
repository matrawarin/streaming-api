public class KYC {

    private int accountId;
    private String fullName;
    private String nationalId;   // เลขบัตรประชาชน / Passport
    private String address;
    private String phone;
    private String email;

    // Constructor
    public KYC(int accountId, String fullName, String nationalId, String address, String phone, String email) {
        this.accountId = accountId;
        this.fullName = fullName;
        this.nationalId = nationalId;
        this.address = address;
        this.phone = phone;
        this.email = email;
    }

    // Getters & Setters
    public int getAccountId() {
        return accountId;
    }

    public String getFullName() {
        return fullName;
    }

    public String getNationalId() {
        return nationalId;
    }

    public String getAddress() {
        return address;
    }

    public String getPhone() {
        return phone;
    }

    public String getEmail() {
        return email;
    }

    public void setFullName(String fullName) {
        this.fullName = fullName;
    }

    public void setNationalId(String nationalId) {
        this.nationalId = nationalId;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    // แสดงข้อมูล KYC
    public void display() {
        System.out.println("=== KYC Information ===");
        System.out.println("Account ID : " + accountId);
        System.out.println("Full Name  : " + fullName);
        System.out.println("National ID: " + nationalId);
        System.out.println("Address    : " + address);
        System.out.println("Phone      : " + phone);
        System.out.println("Email      : " + email);
    }
}

