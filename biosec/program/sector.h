#ifndef SECTOR_H
#define SECTOR_H

#define MAX_SECTORS 50

typedef struct {
    int id;
    char name[50];
} Sector;

void sector_init();
void sector_create();
void sector_list();
Sector* sector_find(int id);

#endif

