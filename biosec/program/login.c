#include <stdio.h>
#include <string.h>
#include "database.c"

int main() {
    if(!init_db()) return 1;

    int choice;
    char username[50], password[50];

    printf("1. Register\n2. Login\nChoose: ");
    scanf("%d", &choice);
    getchar();

    printf("Username: "); fgets(username, 50, stdin); username[strcspn(username, "\n")] = 0;
    printf("Password: "); fgets(password, 50, stdin); password[strcspn(password, "\n")] = 0;

    if(choice == 1) {
        if(register_user(username, password)) printf("Register successful!\n");
        else printf("Register failed!\n");
    } else if(choice == 2) {
        if(login_user(username, password)) printf("Login successful!\n");
        else printf("Login failed!\n");
    }

    sqlite3_close(db);
    return 0;
}

