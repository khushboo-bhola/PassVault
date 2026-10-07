#include <stdio.h>
#include <string.h>
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

void searchPassword()
{
    FILE *file;
    Password entry;
    char searchWebsite[100];
    int found = 0;

    printf("\nEnter website to search: ");
    scanf("%99s", searchWebsite);

    file = fopen("data/passwords.dat", "rb");

    if (file == NULL)
    {
        printf("\nNo passwords found.\n");
        return;
    }

    while (fread(&entry, sizeof(Password), 1, file) == 1)
    {
        if (strcmp(entry.website, searchWebsite) == 0)
        {
            printf("\nPassword found:\n");
            printf("ID: %d\n", entry.id);
            printf("Website: %s\n", entry.website);
            printf("Username: %s\n", entry.username);
            printf("Password: %s\n", entry.password);

            found = 1;
        }
    }

    fclose(file);

    if (found == 0)
    {
        printf("\nNo password found for %s.\n", searchWebsite);
    }
}

void deletePassword()
{
    FILE *file;
    FILE *tempFile;

    Password entry;
    int deleteId;
    int found = 0;

    printf("\nEnter ID to delete: ");
    scanf("%d", &deleteId);

    file = fopen("data/passwords.dat", "rb");

    if (file == NULL)
    {
        printf("\nNo passwords found.\n");
        return;
    }

    tempFile = fopen("data/temp.dat", "wb");

    if (tempFile == NULL)
    {
        printf("\nError creating temporary file.\n");
        fclose(file);
        return;
    }

    while (fread(&entry, sizeof(Password), 1, file) == 1)
    {
        if (entry.id == deleteId)
        {
            found = 1;
            continue;
        }

        fwrite(&entry, sizeof(Password), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    remove("data/passwords.dat");
    rename("data/temp.dat", "data/passwords.dat");

    if (found)
    {
        printf("\nPassword deleted successfully.\n");
    }
    else
    {
        printf("\nPassword with ID %d not found.\n", deleteId);
    }
}