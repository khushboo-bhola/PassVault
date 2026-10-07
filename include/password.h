#ifndef PASSWORD_H
#define PASSWORD_H

typedef struct
{
    int id;
    char website[100];
    char username[100];
    char password[100];
} Password;

void addPassword();

#endif