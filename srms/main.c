#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[50];
char currentRole[20];

/* FUNCTION DECLARATIONS */
int loginSystem();
void adminMenu();
void staffMenu();
void userMenu();

void addStudent();
void viewStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

/* ======================= MAIN ======================= */

int main() {
    if (loginSystem()) {

        if (strcmp(currentRole, "admin") == 0) adminMenu();
        else if (strcmp(currentRole, "staff") == 0) staffMenu();
        else if (strcmp(currentRole, "user") == 0) userMenu();

    } else {
        printf("\nAccess Denied! Exiting...\n");
    }
    return 0;
}

/* ======================= LOGIN SYSTEM ======================= */

int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];

    printf("===== LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");

    if (!fp) {
        printf("ERROR: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) != EOF) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {

            strcpy(currentUser, fileUser);
            strcpy(currentRole, fileRole);

            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* ======================= ADMIN MENU ======================= */

void adminMenu() {
    int choice;

    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid option!\n");
        }

    } while (1);
}


void staffMenu() {
    int choice;

    do {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid option!\n");
        }

    } while (1);
}


void userMenu() {
    int choice;

    do {
        printf("\n===== USER MENU =====\n");
        printf("1. View Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid option!\n");
        }

    } while (1);
}


void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll No: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student added successfully!\n");
}

void viewStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No student records found.\n");
        return;
    }

    printf("\n===== STUDENT LIST =====\n");

    while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) != EOF) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n",
               s.roll, s.name, s.marks);
    }

    fclose(fp);
}

void searchStudent() {
    int roll, found = 0;
    struct Student s;

    printf("\nEnter Roll No to Search: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("File not found!\n");
        return;
    }

    while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) != EOF) {
        if (s.roll == roll) {
            printf("Student Found!\n");
            printf("Roll: %d | Name: %s | Marks: %.2f\n",
                   s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    if (!found) printf("Student Not Found!\n");

    fclose(fp);
}

void updateStudent() {
    int roll, found = 0;
    struct Student s;

    printf("\nEnter Roll No to Update: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) != EOF) {

        if (s.roll == roll) {
            found = 1;
            printf("Enter New Name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
        }

        fprintf(temp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Student Updated Successfully!\n");
    else printf("Student Not Found!\n");
}


void deleteStudent() {
    int roll, found = 0;
    struct Student s;

    printf("\nEnter Roll No to Delete: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) != EOF) {

        if (s.roll == roll) {
            found = 1;
            continue;  // skip writing (delete)
        }

        fprintf(temp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Student Deleted Successfully!\n");
    else printf("Student Not Found!\n");
}
