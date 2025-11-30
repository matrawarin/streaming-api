import java.util.ArrayList;
import java.util.List;

public class Code {

    private int id;
    private String name;

    // Constructor
    public Code(int id, String name) {
        this.id = id;
        this.name = name;
    }

    // Getters & Setters
    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void display() {
        System.out.printf("Code ID: %d, Name: %s\n", id, name);
    }

    // --- Static list สำหรับจัดการ Code ทั้งหมด ---
    private static List<Code> codes = new ArrayList<>();
    private static int nextId = 1;

    // สร้าง Code ใหม่
    public static Code createCode(String name) {
        Code c = new Code(nextId++, name);
        codes.add(c);
        System.out.println("Code created: " + name);
        return c;
    }

    // แสดง Code ทั้งหมด
    public static void listCodes() {
        if (codes.isEmpty()) {
            System.out.println("No codes available.");
            return;
        }
        System.out.println("\n--- Codes ---");
        for (Code c : codes) {
            c.display();
        }
    }

    // ค้นหา Code ตาม ID
    public static Code findCode(int id) {
        for (Code c : codes) {
            if (c.getId() == id) return c;
        }
        return null;
    }
}

