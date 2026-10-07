#include <stdio.h>
#include "../include/password.h"
#include "../include/file.h"

void addPassword()
{
    Password entry;

    printf("\nEnter website: ");
    scanf("%99s", entry.website);

    printf("Enter username: ");
    scanf("%99s", entry.username);

    printf("Enter password: ");
    scanf("%99s", entry.password);

    entry.id = getNextId();

    printf("\nPassword details:\n");
    printf("ID: %d\n", entry.id);
    printf("Website: %s\n", entry.website);
    printf("Username: %s\n", entry.username);
    printf("Password: %s\n", entry.password);

    savePassword(entry);
}

void viewPasswords()
{
    FILE *file;
    Password entry;

    file = fopen("data/passwords.dat", "rb");

    if (file == NULL)
    {
        printf("\nNo passwords found.\n");
        return;
    }

    printf("\n========== SAVED PASSWORDS ==========\n");

    while (fread(&entry, sizeof(Password), 1, file) == 1)
    {
        printf("\nID: %d\n", entry.id);
        printf("Website: %s\n", entry.website);
        printf("Username: %s\n", entry.username);
        printf("Password: %s\n", entry.password);
    }

    fclose(file);
}