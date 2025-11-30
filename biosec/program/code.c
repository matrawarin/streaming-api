#include <stdio.h>
#include <string.h>
#include "code.h"

static Code codes[MAX_CODES];
static int code_count = 0;

// โหลดค่าเริ่มต้น หรือรีเซ็ต
void code_init() {
    code_count = 0;
}

// สร้าง code ใหม่
void code_create() {
    if (code_count >= MAX_CODES) {
        printf("Maximum codes reached!\n");
        return;
    }

    Code c;
    c.id = code_count + 1;

    printf("Enter code (e.g., ACC001): ");
    getchar(); // ล้าง newline
    fgets(c.code, sizeof(c.code), stdin);
    c.code[strcspn(c.code, "\n")] = 0;

    printf("Enter description: ");
    fgets(c.description, sizeof(c.description), stdin);
    c.description[strcspn(c.description, "\n")] = 0;

    codes[code_count++] = c;
    printf("Code created: ID %d, Code %s, Description %s\n", c.id, c.code, c.description);
}

// แสดง code ทั้งหมด
void code_list() {
    if (code_count == 0) {
        printf("No codes available.\n");
        return;
    }

    printf("\n%-5s %-15s %-20s\n", "ID", "Code", "Description");
    printf("----------------------------------------\n");
    for (int i = 0; i < code_count; i++) {
        printf("%-5d %-15s %-20s\n", codes[i].id, codes[i].code, codes[i].description);
    }
}

// ค้นหา code ตาม ID
Code* code_find(int id) {
    for (int i = 0; i < code_count; i++) {
        if (codes[i].id == id) return &codes[i];
    }
    return NULL;
}

