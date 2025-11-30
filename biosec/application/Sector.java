import java.util.ArrayList;
import java.util.List;

public class Sector {

    private int id;
    private String name;

    // Constructor
    public Sector(int id, String name) {
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
        System.out.printf("Sector ID: %d, Name: %s\n", id, name);
    }

    // --- Static list สำหรับจัดการ Sector ทั้งหมด ---
    private static List<Sector> sectors = new ArrayList<>();
    private static int nextId = 1;

    // สร้าง Sector ใหม่
    public static Sector createSector(String name) {
        Sector sec = new Sector(nextId++, name);
        sectors.add(sec);
        System.out.println("Sector created: " + name);
        return sec;
    }

    // แสดง Sector ทั้งหมด
    public static void listSectors() {
        if (sectors.isEmpty()) {
            System.out.println("No sectors available.");
            return;
        }
        System.out.println("\n--- Sectors ---");
        for (Sector sec : sectors) {
            sec.display();
        }
    }

    // ค้นหา Sector ตาม ID
    public static Sector findSector(int id) {
        for (Sector sec : sectors) {
            if (sec.getId() == id) return sec;
        }
        return null;
    }
}

