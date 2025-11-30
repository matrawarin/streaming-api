import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Database {
    private Connection conn;

    // เชื่อมต่อ SQLite
    public boolean dbConnect(String dbFile) {
        try {
            conn = DriverManager.getConnection("jdbc:sqlite:" + dbFile);
            System.out.println("Connected to database successfully.");
            return true;
        } catch (SQLException e) {
            System.out.println("Database connection failed: " + e.getMessage());
            return false;
        }
    }

    // ปิด connection
    public void dbClose() {
        try {
            if (conn != null) conn.close();
        } catch (SQLException e) {
            System.out.println("Error closing database: " + e.getMessage());
        }
    }

    // สร้างตาราง accounts และ ledger
    public void dbInit() throws SQLException {
        String sqlAccounts = "CREATE TABLE IF NOT EXISTS accounts ("
                + "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                + "name TEXT NOT NULL,"
                + "balance REAL DEFAULT 0,"
                + "section_id INTEGER,"
                + "sector_id INTEGER,"
                + "code_id INTEGER"
                + ");";

        String sqlLedger = "CREATE TABLE IF NOT EXISTS ledger ("
                + "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                + "account_id INTEGER,"
                + "type TEXT,"
                + "amount REAL,"
                + "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
                + "FOREIGN KEY(account_id) REFERENCES accounts(id)"
                + ");";

        Statement stmt = conn.createStatement();
        stmt.execute(sqlAccounts);
        stmt.execute(sqlLedger);
        stmt.close();
        System.out.println("Database tables initialized.");
    }

    // เพิ่มบัญชี
    public boolean dbInsertAccount(Account acc) throws SQLException {
        String sql = "INSERT INTO accounts (name, balance, section_id, sector_id, code_id) VALUES (?, ?, ?, ?, ?);";
        PreparedStatement pstmt = conn.prepareStatement(sql);
        pstmt.setString(1, acc.getName());
        pstmt.setDouble(2, acc.getBalance());
        pstmt.setInt(3, acc.getSectionId());
        pstmt.setInt(4, acc.getSectorId());
        pstmt.setInt(5, acc.getCodeId());
        int result = pstmt.executeUpdate();
        pstmt.close();
        return result > 0;
    }

    // ดึงบัญชีจาก ID
    public Account dbGetAccount(int id) throws SQLException {
        String sql = "SELECT * FROM accounts WHERE id = ?;";
        PreparedStatement pstmt = conn.prepareStatement(sql);
        pstmt.setInt(1, id);
        ResultSet rs = pstmt.executeQuery();
        Account acc = null;
        if (rs.next()) {
            acc = new Account(
                    rs.getInt("id"),
                    rs.getString("name"),
                    rs.getDouble("balance"),
                    rs.getInt("section_id"),
                    rs.getInt("sector_id"),
                    rs.getInt("code_id")
            );
        }
        rs.close();
        pstmt.close();
        return acc;
    }

    // อัปเดต balance
    public boolean dbUpdateAccountBalance(int id, double newBalance) throws SQLException {
        String sql = "UPDATE accounts SET balance = ? WHERE id = ?;";
        PreparedStatement pstmt = conn.prepareStatement(sql);
        pstmt.setDouble(1, newBalance);
        pstmt.setInt(2, id);
        int result = pstmt.executeUpdate();
        pstmt.close();
        return result > 0;
    }

    // เพิ่ม transaction ลง ledger
    public boolean dbInsertTransaction(int accountId, String type, double amount) throws SQLException {
        String sql = "INSERT INTO ledger (account_id, type, amount) VALUES (?, ?, ?);";
        PreparedStatement pstmt = conn.prepareStatement(sql);
        pstmt.setInt(1, accountId);
        pstmt.setString(2, type);
        pstmt.setDouble(3, amount);
        int result = pstmt.executeUpdate();
        pstmt.close();
        return result > 0;
    }

    // แสดงบัญชีทั้งหมด
    public void dbListAccounts() throws SQLException {
        String sql = "SELECT * FROM accounts;";
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sql);
        System.out.printf("\n%-5s %-20s %-10s %-10s %-10s %-10s\n", "ID", "Name", "Balance", "Section", "Sector", "Code");
        System.out.println("---------------------------------------------------------------");
        while (rs.next()) {
            System.out.printf("%-5d %-20s %-10.2f %-10d %-10d %-10d\n",
                    rs.getInt("id"),
                    rs.getString("name"),
                    rs.getDouble("balance"),
                    rs.getInt("section_id"),
                    rs.getInt("sector_id"),
                    rs.getInt("code_id"));
        }
        rs.close();
        stmt.close();
    }

    // แสดง ledger ทั้งหมด
    public void dbShowLedger() throws SQLException {
        String sql = "SELECT * FROM ledger;";
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sql);
        System.out.printf("\n%-10s %-10s %-10s %-20s\n", "ID", "AccountID", "Type", "Amount");
        System.out.println("-------------------------------------------------");
        while (rs.next()) {
            System.out.printf("%-10d %-10d %-10s %.2f\n",
                    rs.getInt("id"),
                    rs.getInt("account_id"),
                    rs.getString("type"),
                    rs.getDouble("amount"));
        }
        rs.close();
        stmt.close();
    }

    // ดึง transaction ทั้งหมด (List)
    public List<Transaction> dbGetTransactions() throws SQLException {
        String sql = "SELECT * FROM ledger;";
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sql);
        List<Transaction> txList = new ArrayList<>();
        while (rs.next()) {
            Transaction tx = new Transaction(
                    rs.getInt("account_id"),
                    rs.getString("type"),
                    rs.getDouble("amount")
            );
            txList.add(tx);
        }
        rs.close();
        stmt.close();
        return txList;
    }
}

