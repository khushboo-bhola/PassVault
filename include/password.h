#ifndef PASSWORD_H
#define PASSWORD_H

#define MAX_PASSWORDS 1000

typedef struct {
    int id;
    char website[100];
    char username[100];
    char password[100];
} Password;

extern Password vault[MAX_PASSWORDS];
extern int vaultCount;

void addPassword(void);
void viewPasswords(void);
void searchPassword(void);
void deletePassword(void);

#endif