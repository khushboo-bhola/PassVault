#include <stdio.h>
#include "../include/password.h"

void addPassword()
{
    char website[100];
    char username[100];
    char password[100];

    printf("\nEnter website: ");
    scanf("%99s", website);

    printf("Enter username: ");
    scanf("%99s", username);

    printf("Enter password: ");
    scanf("%99s", password);

    printf("\nPassword details:\n");
    printf("Website: %s\n", website);
    printf("Username: %s\n", username);
    printf("Password: %s\n", password);
}