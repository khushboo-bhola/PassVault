
#include <stdio.h>
#include <string.h>
#include <openssl/crypto.h>

#include "../include/password.h"
#include "../include/file.h"
#include "../include/utils.h"

void addPassword(void)
{
    if (vaultCount >= MAX_PASSWORDS) {
        printf("Vault is full.\n");
        return;
    }

    Password entry = {0};

    printf("\nEnter website: ");
    if (scanf("%99s", entry.website) != 1) {
        clearInputBuffer();
        printf("Invalid website.\n");
        return;
    }

    printf("Enter username: ");
    if (scanf("%99s", entry.username) != 1) {
        clearInputBuffer();
        printf("Invalid username.\n");
        return;
    }

    printf("Enter password: ");
    if (scanf("%99s", entry.password) != 1) {
        clearInputBuffer();
        OPENSSL_cleanse(&entry, sizeof(entry));
        printf("Invalid password.\n");
        return;
    }

    entry.id = getNextId();

    if (entry.id <= 0) {
        printf("Could not generate an ID.\n");
        OPENSSL_cleanse(&entry, sizeof(entry));
        return;
    }

    if (!savePassword(entry)) {
        printf("Password was not saved.\n");
        OPENSSL_cleanse(&entry, sizeof(entry));
        return;
    }

    vault[vaultCount++] = entry;

    printf("Password saved successfully. ID: %d\n", entry.id);
    OPENSSL_cleanse(&entry, sizeof(entry));
}

void viewPasswords(void)
{
    if (vaultCount == 0) {
        printf("\nNo passwords found.\n");
        return;
    }

    printf("\n==== SAVED PASSWORDS ====\n");

    for (int i = 0; i < vaultCount; i++) {
        printf("\nID: %d\n", vault[i].id);
        printf("Website: %s\n", vault[i].website);
        printf("Username: %s\n", vault[i].username);
        printf("Password: %s\n", vault[i].password);
    }
}

void searchPassword(void)
{
    char website[100];

    printf("\nEnter website to search: ");

    if (scanf("%99s", website) != 1) {
        clearInputBuffer();
        printf("Invalid website.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < vaultCount; i++) {
        if (strcmp(vault[i].website, website) == 0) {
            printf("\nPassword found:\n");
            printf("ID: %d\n", vault[i].id);
            printf("Website: %s\n", vault[i].website);
            printf("Username: %s\n", vault[i].username);
            printf("Password: %s\n", vault[i].password);
            found = 1;
        }
    }

    if (!found)
        printf("No password found for %s.\n", website);
}

void deletePassword(void)
{
    int deleteId;

    printf("\nEnter ID to delete: ");

    if (scanf("%d", &deleteId) != 1) {
        clearInputBuffer();
        printf("Invalid ID.\n");
        return;
    }

    int index = -1;

    for (int i = 0; i < vaultCount; i++) {
        if (vault[i].id == deleteId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Password with ID %d not found.\n", deleteId);
        return;
    }

    Password removed = vault[index];

    for (int i = index; i < vaultCount - 1; i++)
        vault[i] = vault[i + 1];

    vaultCount--;

    const char *masterPassword = getenv("PASSVAULT_MASTER");

    if (masterPassword == NULL ||
        !saveVault(masterPassword)) {
        /* Restore the record if persistent deletion fails. */
        for (int i = vaultCount; i > index; i--)
            vault[i] = vault[i - 1];

        vault[index] = removed;
        vaultCount++;

        OPENSSL_cleanse(&removed, sizeof(removed));
        printf("Deletion failed. Original record restored.\n");
        return;
    }

    OPENSSL_cleanse(&vault[vaultCount], sizeof(Password));
    OPENSSL_cleanse(&removed, sizeof(removed));

    printf("Password deleted successfully.\n");
}
