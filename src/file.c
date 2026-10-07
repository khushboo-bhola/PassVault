#include <stdio.h>
#include "../include/file.h"

void savePassword(Password entry)
{
    FILE *file;

    file = fopen("data/passwords.dat", "ab");

    if (file == NULL)
    {
        printf("Error: Could not open password file.\n");
        return;
    }

    fwrite(&entry, sizeof(Password), 1, file);

    fclose(file);

    printf("Password saved successfully.\n");
}