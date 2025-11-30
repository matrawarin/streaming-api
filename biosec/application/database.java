import java.sql.*;

public class Database {
    public static Connection connect() throws SQLException {
        return DriverManager.getConnection("jdbc:sqlite:users.db");
    }

    public static void init() throws SQLException {
        Connection conn = connect();
        Statement stmt = conn.createStatement();
        stmt.execute("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT);");
        stmt.close(); conn.close();
    }
}

