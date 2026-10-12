#ifndef CRYPTO_H
#define CRYPTO_H

int encryptData(
    const unsigned char *plaintext,
    int plaintextLen,
    const char *masterPassword,
    unsigned char **encrypted,
    int *encryptedLen
);

int decryptData(
    const unsigned char *encrypted,
    int encryptedLen,
    const char *masterPassword,
    unsigned char **plaintext,
    int *plaintextLen
);

#endif