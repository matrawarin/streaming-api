#ifndef SECTION_H
#define SECTION_H

#define MAX_SECTIONS 50

typedef struct {
    int id;
    char name[50];
} Section;

void section_init();
void section_create();
void section_list();
Section* section_find(int id);

#endif

