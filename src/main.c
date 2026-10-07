#include <stdio.h>
#include "../include/password.h"

int main()
{
    int choice;

    printf("=================================\n");
    printf("          PASSVAULT\n");
    printf("=================================\n");

    printf("\n1. Add Password\n");
    printf("2. View Passwords\n");
    printf("3. Search Password\n");
    printf("4. Delete Password\n");
    printf("5. Exit\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
       case 1:
            addPassword();
            break;

        case 2:
            viewPasswords();
            break;

        case 3:
            searchPassword();
            break;

        case 4:
            deletePassword();
            break;

        case 5:
            printf("\nThank you for using PassVault!\n");
            break;

        default:
            printf("\nInvalid choice.\n");
    }

    return 0;
}