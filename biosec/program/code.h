#ifndef CODE_H
#define CODE_H

#define MAX_CODES 100

typedef struct {
    int id;
    char code[20];     // รหัส เช่น "ACC001", "FUND01"
    char description[50];
} Code;

// ฟังก์ชัน
void code_init();
void code_create();
void code_list();
Code* code_find(int id);

#endif

