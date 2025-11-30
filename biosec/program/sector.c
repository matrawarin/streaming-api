#include <stdio.h>
#include <string.h>
#include "sector.h"

static Sector sectors[MAX_SECTORS];
static int sector_count = 0;

// โหลดค่าเริ่มต้น หรือรีเซ็ต
void sector_init() {
    sector_count = 0;
}

// สร้าง sector ใหม่
void sector_create() {
    if (sector_count >= MAX_SECTORS) {
        printf("Maximum sectors reached!\n");
        return;
    }

    Sector sec;
    sec.id = sector_count + 1;
    printf("Enter sector name: ");
    getchar(); // ล้าง newline
    fgets(sec.name, sizeof(sec.name), stdin);
    sec.name[strcspn(sec.name, "\n")] = 0;

    sectors[sector_count++] = sec;
    printf("Sector created: ID %d, Name %s\n", sec.id, sec.name);
}

// แสดง sector ทั้งหมด
void sector_list() {
    if (sector_count == 0) {
        printf("No sectors available.\n");
        return;
    }

    printf("\n%-5s %-20s\n", "ID", "Name");
    printf("------------------------\n");
    for (int i = 0; i < sector_count; i++) {
        printf("%-5d %-20s\n", sectors[i].id, sectors[i].name);
    }
}

// ค้นหา sector ตาม ID
Sector* sector_find(int id) {
    for (int i = 0; i < sector_count; i++) {
        if (sectors[i].id == id) return &sectors[i];
    }
    return NULL;
}

