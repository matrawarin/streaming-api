import java.util.ArrayList;
import java.util.List;

public class Section {

    private int id;
    private String name;

    // Constructor
    public Section(int id, String name) {
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

    // แสดงข้อมูล Section
    public void display() {
        System.out.printf("Section ID: %d, Name: %s\n", id, name);
    }

    // --- Static list สำหรับจัดการ Section ทั้งหมด ---
    private static List<Section> sections = new ArrayList<>();
    private static int nextId = 1;

    // สร้าง Section ใหม่
    public static Section createSection(String name) {
        Section sec = new Section(nextId++, name);
        sections.add(sec);
        System.out.println("Section created: " + name);
        return sec;
    }

    // แสดง Section ทั้งหมด
    public static void listSections() {
        if (sections.isEmpty()) {
            System.out.println("No sections available.");
            return;
        }
        System.out.println("\n--- Sections ---");
        for (Section sec : sections) {
            sec.display();
        }
    }

    // ค้นหา Section ตาม ID
    public static Section findSection(int id) {
        for (Section sec : sections) {
            if (sec.getId() == id) return sec;
        }
        return null;
    }
}

