#include <stdio.h>
#include "../include/password.h"

void addPassword()
{
    Password entry;

    printf("\nEnter website: ");
    scanf("%99s", entry.website);

    printf("Enter username: ");
    scanf("%99s", entry.username);

    printf("Enter password: ");
    scanf("%99s", entry.password);

    entry.id = 1;

    printf("\nPassword details:\n");
    printf("ID: %d\n", entry.id);
    printf("Website: %s\n", entry.website);
    printf("Username: %s\n", entry.username);
    printf("Password: %s\n", entry.password);
}