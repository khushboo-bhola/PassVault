
#include <stdio.h>
#include <stdlib.h>

#include "../include/password.h"
#include "../include/file.h"
#include "../include/utils.h"

int main(void)
{
    const char *masterPassword = getenv("PASSVAULT_MASTER");

    if (masterPassword == NULL || masterPassword[0] == '\0') {
        fprintf(stderr,
                "Set PASSVAULT_MASTER before starting PassVault.\n");
        return 1;
    }

    if (!initializeVault(masterPassword)) {
        fprintf(stderr, "Could not initialize or load the vault.\n");
        return 1;
    }

    int choice;

    while (1) {
        printf("\n===== PASSVAULT =====\n");
        printf("1. Add Password\n");
        printf("2. View Passwords\n");
        printf("3. Search Password\n");
        printf("4. Delete Password\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice) {
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
                printf("Thank you for using PassVault!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}
