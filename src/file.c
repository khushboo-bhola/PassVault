#include <stdio.h>
#include "../include/file.h"

int getNextId()
{
    FILE *file;
    Password entry;
    int maxId = 0;

    file = fopen("data/passwords.dat", "rb");

    if (file == NULL){
        return 1;
    }

    while (fread(&entry, sizeof(Password), 1, file) == 1)
    {
        if (entry.id > maxId)
        {
            maxId = entry.id;
        }
    }

    fclose(file);

    return maxId + 1;
}

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