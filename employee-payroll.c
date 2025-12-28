#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee {
    int id;
    char name[50];
    float salary;
};

struct Employee e;

/* Function Prototypes */
void addEmployee();
void displayAll();
void searchEmployee();
void updateEmployee();
void deleteEmployee();
void generatePayslip();
float calculateTax(float salary);
int adminLogin();

/* ADMIN LOGIN FUNCTION */
int adminLogin() {
    char username[20], password[20];
    int attempts = 3;

    while (attempts--) {
        printf("\n====== ADMIN LOGIN ======\n");
        printf("Enter Username: ");
        scanf("%s", username);
        printf("Enter Password: ");
        scanf("%s", password);

        if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
            printf("\nLogin Successful! Welcome Admin.\n");
            return 1;
        } else {
            printf("Invalid Username or Password! Attempts left: %d\n", attempts);
        }
    }
    return 0; // Login failed
}

/* CALCULATE TAX FUNCTION */
float calculateTax(float salary) {
    if (salary > 50000)
        return salary * 0.10;
    else if (salary >= 30000)
        return salary * 0.05;
    else
        return 0;
}

/* ADD EMPLOYEE */
void addEmployee() {
    FILE *fp = fopen("employee.dat", "ab");

    printf("\nEnter Employee ID: ");
    scanf("%d", &e.id);
    printf("Enter Employee Name: ");
    scanf("%s", e.name);
    printf("Enter Salary: ");
    scanf("%f", &e.salary);

    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);
    printf("Employee Added Successfully!\n");
}

/* DISPLAY EMPLOYEES */
void displayAll() {
    FILE *fp = fopen("employee.dat", "rb");
    printf("\n====== Employee List ======\n");

    while (fread(&e, sizeof(e), 1, fp)) {
        float tax = calculateTax(e.salary);
        printf("\nID: %d\nName: %s\nSalary: %.2f\nTax: %.2f\n", e.id, e.name, e.salary, tax);
    }
    fclose(fp);
}

/* SEARCH EMPLOYEE */
void searchEmployee() {
    FILE *fp = fopen("employee.dat", "rb");
    int id, found = 0;

    printf("\nEnter Employee ID to Search: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.id == id) {
            printf("\nID: %d\nName: %s\nSalary: %.2f\nTax: %.2f\n", e.id, e.name, e.salary, calculateTax(e.salary));
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("Employee Not Found!\n");
}

/* UPDATE EMPLOYEE */
void updateEmployee() {
    FILE *fp = fopen("employee.dat", "rb+");
    int id, found = 0;

    printf("\nEnter Employee ID to Update: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.id == id) {
            printf("Enter New Name: ");
            scanf("%s", e.name);
            printf("Enter New Salary: ");
            scanf("%f", &e.salary);

            fseek(fp, -sizeof(e), SEEK_CUR);
            fwrite(&e, sizeof(e), 1, fp);
            found = 1;
            printf("Record Updated Successfully!\n");
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("Employee Not Found!\n");
}

/* DELETE EMPLOYEE */
void deleteEmployee() {
    FILE *fp = fopen("employee.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    int id, found = 0;

    printf("\nEnter Employee ID to Delete: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.id != id) {
            fwrite(&e, sizeof(e), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("employee.dat");
    rename("temp.dat", "employee.dat");

    if (found)
        printf("Employee Deleted Successfully!\n");
    else
        printf("Employee Not Found!\n");
}

/* GENERATE PAYSLIP */
void generatePayslip() {
    FILE *fp = fopen("employee.dat", "rb");
    int id, found = 0;
    float tax, netSalary;

    printf("\nEnter Employee ID: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.id == id) {
            found = 1;
            tax = calculateTax(e.salary);
            netSalary = e.salary - tax;

            printf("\n======= PAYSLIP =======\n");
            printf("ID: %d\nName: %s\n", e.id, e.name);
            printf("Gross Salary: %.2f\n", e.salary);
            printf("Tax Deducted: %.2f\n", tax);
            printf("Net Salary: %.2f\n", netSalary);
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("Employee Not Found!\n");
}

/* MAIN FUNCTION */
int main() {
    int choice;

    if (!adminLogin()) {
        printf("\nLogin failed! Exiting...\n");
        exit(0);
    }

    do {
        printf("\n====== EMPLOYEE PAYROLL SYSTEM ======\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Generate Payslip\n");
        printf("0. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: displayAll(); break;
            case 3: searchEmployee(); break;
            case 4: updateEmployee(); break;
            case 5: deleteEmployee(); break;
            case 6: generatePayslip(); break;
            case 0: printf("Exiting System... Goodbye!\n"); break;
            default: printf("Invalid Option! Try Again.\n");
        }
    } while (choice != 0);

    return 0;
}
