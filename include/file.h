#ifndef FILE_H
#define FILE_H

#include "password.h"

int initializeVault(const char *masterPassword);
int saveVault(const char *masterPassword);
int loadVault(const char *masterPassword);

int savePassword(Password entry);
int getNextId(void);

#endif