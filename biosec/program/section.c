#include <stdio.h>
#include <string.h>
#include "section.h"

static Section sections[MAX_SECTIONS];
static int section_count = 0;

// โหลดค่าเริ่มต้น หรือสร้างใหม่
void section_init() {
    section_count = 0;
}

// สร้าง section ใหม่
void section_create() {
    if (section_count >= MAX_SECTIONS) {
        printf("Maximum sections reached!\n");
        return;
    }

    Section sec;
    sec.id = section_count + 1;
    printf("Enter section name: ");
    getchar(); // ล้าง newline
    fgets(sec.name, sizeof(sec.name), stdin);
    sec.name[strcspn(sec.name, "\n")] = 0;

    sections[section_count++] = sec;
    printf("Section created: ID %d, Name %s\n", sec.id, sec.name);
}

// แสดง section ทั้งหมด
void section_list() {
    if (section_count == 0) {
        printf("No sections available.\n");
        return;
    }

    printf("\n%-5s %-20s\n", "ID", "Name");
    printf("------------------------\n");
    for (int i = 0; i < section_count; i++) {
        printf("%-5d %-20s\n", sections[i].id, sections[i].name);
    }
}

// ค้นหา section ตาม ID
Section* section_find(int id) {
    for (int i = 0; i < section_count; i++) {
        if (sections[i].id == id) return &sections[i];
    }
    return NULL;
}

