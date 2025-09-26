#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "users.txt"

typedef struct {
    int id;
    char name[50];
    int age;
} User;

void addUser() {
    FILE *file = fopen(FILENAME, "a");
    if (!file) { printf("Error opening file.\n"); return; }
    User u;
    printf("Enter ID: "); scanf("%d", &u.id);
    printf("Enter Name: "); scanf("%s", u.name);
    printf("Enter Age: "); scanf("%d", &u.age);
    fprintf(file, "%d %s %d\n", u.id, u.name, u.age);
    fclose(file);
    printf("User added successfully.\n");
}

void displayUsers() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) { printf("No users found.\n"); return; }
    User u;
    printf("ID\tName\tAge\n");
    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) == 3)
        printf("%d\t%s\t%d\n", u.id, u.name, u.age);
    fclose(file);
}

void updateUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) { printf("Error opening file.\n"); return; }
    User u; int id, found = 0;
    printf("Enter ID to update: "); scanf("%d", &id);
    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            printf("Enter new Name: "); scanf("%s", u.name);
            printf("Enter new Age: "); scanf("%d", &u.age);
            found = 1;
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }
    fclose(file); fclose(temp);
    remove(FILENAME); rename("temp.txt", FILENAME);
    if (found) printf("User updated successfully.\n"); else printf("User not found.\n");
}

void deleteUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) { printf("Error opening file.\n"); return; }
    User u; int id, found = 0;
    printf("Enter ID to delete: "); scanf("%d", &id);
    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) found = 1;
        else fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }
    fclose(file); fclose(temp);
    remove(FILENAME); rename("temp.txt", FILENAME);
    if (found) printf("User deleted successfully.\n"); else printf("User not found.\n");
}

int main() {
    int choice;
    do {
        printf("\n1. Add User\n2. Display Users\n3. Update User\n4. Delete User\n5. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addUser(); break;
            case 2: displayUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: printf("Exiting.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}
