import java.sql.*;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws SQLException {
        Database.init();
        Scanner sc = new Scanner(System.in);

        System.out.println("1. Register\n2. Login\nChoose:");
        int choice = sc.nextInt(); sc.nextLine();

        System.out.print("Username: "); String username = sc.nextLine();
        System.out.print("Password: "); String password = sc.nextLine();

        Connection conn = Database.connect();
        if(choice == 1){
            PreparedStatement ps = conn.prepareStatement("INSERT INTO users(username,password) VALUES(?,?)");
            ps.setString(1, username);
            ps.setString(2, password);
            ps.executeUpdate();
            System.out.println("Register successful!");
        } else if(choice == 2){
            PreparedStatement ps = conn.prepareStatement("SELECT * FROM users WHERE username=? AND password=?");
            ps.setString(1, username);
            ps.setString(2, password);
            ResultSet rs = ps.executeQuery();
            if(rs.next()) System.out.println("Login successful!");
            else System.out.println("Login failed!");
        }
        conn.close();
        sc.close();
    }
}

