// Developed Bank Management System using Random Access File

#include <stdio.h>
#include <stdlib.h>

struct clientData
{
    int acctNum;
    char lastName[15];
    char firstName[15];
    double balance;
};

// Function Prototypes
void createFile();
void displayRecords(FILE *fPtr);
void addRecord(FILE *fPtr);
void updateRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
int menu();

int main()
{
    FILE *fPtr;
    int choice;

    // Create file if not available
    createFile();

    // Open file
    if ((fPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = menu()) != 5)
    {
        switch (choice)
        {
        case 1:
            displayRecords(fPtr);
            break;

        case 2:
            addRecord(fPtr);
            break;

        case 3:
            updateRecord(fPtr);
            break;

        case 4:
            deleteRecord(fPtr);
            break;

        default:
            printf("Invalid Choice\n");
        }
    }

    fclose(fPtr);

    printf("Program Ended Successfully.\n");

    return 0;
}

// Create Empty File with 100 Blank Records
void createFile()
{
    FILE *fPtr;

    struct clientData blankClient = {0, "", "", 0.0};

    fPtr = fopen("credit.dat", "rb");

    // File already exists
    if (fPtr != NULL)
    {
        fclose(fPtr);
        return;
    }

    // Create new file
    fPtr = fopen("credit.dat", "wb");

    for (int i = 0; i < 100; i++)
    {
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    }

    fclose(fPtr);
}

// Display All Records
void displayRecords(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-10s %-15s %-15s %-10s\n",
           "Account", "Last Name", "First Name", "Balance");

    printf("----------------------------------------------------------\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10d %-15s %-15s %-10.2lf\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// Add New Record
void addRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    int account;

    printf("Enter Account Number (1-100): ");
    scanf("%d", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid Account Number\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        client.acctNum = account;

        printf("Enter Last Name: ");
        scanf("%s", client.lastName);

        printf("Enter First Name: ");
        scanf("%s", client.firstName);

        printf("Enter Balance: ");
        scanf("%lf", &client.balance);

        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Record Added Successfully.\n");
    }
}

// Update Existing Record
void updateRecord(FILE *fPtr)
{
    struct clientData client;

    int account;
    double amount;

    printf("Enter Account Number to Update: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        printf("\nCurrent Balance = %.2lf\n", client.balance);

        printf("Enter Amount (+ deposit / - withdraw): ");
        scanf("%lf", &amount);

        client.balance += amount;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Balance Updated Successfully.\n");
    }
}

// Delete Record
void deleteRecord(FILE *fPtr)
{
    struct clientData blankClient = {0, "", "", 0.0};
    struct clientData client;

    int account;

    printf("Enter Account Number to Delete: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

        printf("Record Deleted Successfully.\n");
    }
}

// Menu Function
int menu()
{
    int choice;

    printf("\n===== BANK MANAGEMENT SYSTEM =====\n");

    printf("1. Display All Records\n");
    printf("2. Add New Record\n");
    printf("3. Update Record\n");
    printf("4. Delete Record\n");
    printf("5. Exit\n");

    printf("Enter Your Choice: ");
    scanf("%d", &choice);

    return choice;
}