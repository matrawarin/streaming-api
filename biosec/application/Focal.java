import java.sql.DriverManager
import java.util.Scanner

fun main() {
    val conn = DriverManager.getConnection("jdbc:sqlite:users.db")
    val stmt = conn.createStatement()
    stmt.execute("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT);")

    val sc = Scanner(System.`in`)
    println("1. Register\n2. Login\nChoose:")
    val choice = sc.nextInt(); sc.nextLine()

    print("Username: "); val username = sc.nextLine()
    print("Password: "); val password = sc.nextLine()

    if(choice == 1){
        val ps = conn.prepareStatement("INSERT INTO users(username,password) VALUES(?,?)")
        ps.setString(1, username)
        ps.setString(2, password)
        ps.executeUpdate()
        println("Register successful!")
    } else if(choice == 2){
        val ps = conn.prepareStatement("SELECT * FROM users WHERE username=? AND password=?")
        ps.setString(1, username)
        ps.setString(2, password)
        val rs = ps.executeQuery()
        if(rs.next()) println("Login successful!") else println("Login failed!")
    }
    conn.close()
    sc.close()
}

