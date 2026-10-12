
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include "../include/crypto.h"

#define SALT_LEN 16
#define NONCE_LEN 12
#define KEY_LEN 32
#define TAG_LEN 16
#define ITERATIONS 600000

int encryptData(
    const unsigned char *plaintext,
    int plaintextLen,
    const char *masterPassword,
    unsigned char **encrypted,
    int *encryptedLen)
{
    unsigned char salt[SALT_LEN];
    unsigned char nonce[NONCE_LEN];
    unsigned char key[KEY_LEN] = {0};
    unsigned char tag[TAG_LEN];

    unsigned char *output = NULL;
    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0, ciphertextLen = 0;
    int totalLen = 0, success = 0;

    if (encrypted == NULL || encryptedLen == NULL)
        return 0;

    *encrypted = NULL;
    *encryptedLen = 0;

    if (plaintextLen < 0 ||
        (plaintextLen > 0 && plaintext == NULL) ||
        masterPassword == NULL ||
        strlen(masterPassword) > INT_MAX ||
        plaintextLen > INT_MAX - SALT_LEN - NONCE_LEN - TAG_LEN)
        goto cleanup;

    totalLen = SALT_LEN + NONCE_LEN + plaintextLen + TAG_LEN;

    if (RAND_bytes(salt, SALT_LEN) != 1 ||
        RAND_bytes(nonce, NONCE_LEN) != 1)
        goto cleanup;

    if (PKCS5_PBKDF2_HMAC(
            masterPassword, (int)strlen(masterPassword),
            salt, SALT_LEN, ITERATIONS, EVP_sha256(),
            KEY_LEN, key) != 1)
        goto cleanup;

    output = malloc((size_t)totalLen);
    if (output == NULL)
        goto cleanup;

    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL)
        goto cleanup;

    memcpy(output, salt, SALT_LEN);
    memcpy(output + SALT_LEN, nonce, NONCE_LEN);

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(),
                           NULL, NULL, NULL) != 1)
        goto cleanup;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN,
                            NONCE_LEN, NULL) != 1)
        goto cleanup;

    if (EVP_EncryptInit_ex(ctx, NULL, NULL, key, nonce) != 1)
        goto cleanup;

    if (plaintextLen > 0) {
        if (EVP_EncryptUpdate(
                ctx, output + SALT_LEN + NONCE_LEN,
                &len, plaintext, plaintextLen) != 1)
            goto cleanup;

        ciphertextLen = len;
    }

    if (EVP_EncryptFinal_ex(
            ctx, output + SALT_LEN + NONCE_LEN + ciphertextLen,
            &len) != 1)
        goto cleanup;

    ciphertextLen += len;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG,
                            TAG_LEN, tag) != 1)
        goto cleanup;

    memcpy(output + SALT_LEN + NONCE_LEN + ciphertextLen,
           tag, TAG_LEN);

    *encrypted = output;
    *encryptedLen = SALT_LEN + NONCE_LEN +
                    ciphertextLen + TAG_LEN;

    output = NULL;
    success = 1;

cleanup:
    if (ctx != NULL)
        EVP_CIPHER_CTX_free(ctx);

    if (output != NULL) {
        OPENSSL_cleanse(output, (size_t)totalLen);
        free(output);
    }

    OPENSSL_cleanse(key, sizeof(key));
    OPENSSL_cleanse(tag, sizeof(tag));
    return success;
}

int decryptData(
    const unsigned char *encrypted,
    int encryptedLen,
    const char *masterPassword,
    unsigned char **plaintext,
    int *plaintextLen)
{
    unsigned char key[KEY_LEN] = {0};
    unsigned char *output = NULL;
    EVP_CIPHER_CTX *ctx = NULL;

    const unsigned char *salt;
    const unsigned char *nonce;
    const unsigned char *ciphertext;
    const unsigned char *tag;

    int ciphertextLen, len = 0, outputLen = 0;
    int success = 0;

    if (plaintext == NULL || plaintextLen == NULL)
        return 0;

    *plaintext = NULL;
    *plaintextLen = 0;

    if (encrypted == NULL || masterPassword == NULL ||
        strlen(masterPassword) > INT_MAX ||
        encryptedLen < SALT_LEN + NONCE_LEN + TAG_LEN)
        goto cleanup;

    salt = encrypted;
    nonce = encrypted + SALT_LEN;
    ciphertext = encrypted + SALT_LEN + NONCE_LEN;

    ciphertextLen = encryptedLen - SALT_LEN -
                    NONCE_LEN - TAG_LEN;

    tag = encrypted + encryptedLen - TAG_LEN;

    if (PKCS5_PBKDF2_HMAC(
            masterPassword, (int)strlen(masterPassword),
            salt, SALT_LEN, ITERATIONS, EVP_sha256(),
            KEY_LEN, key) != 1)
        goto cleanup;

    output = malloc((size_t)ciphertextLen + 1);
    if (output == NULL)
        goto cleanup;

    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL)
        goto cleanup;

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(),
                           NULL, NULL, NULL) != 1)
        goto cleanup;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN,
                            NONCE_LEN, NULL) != 1)
        goto cleanup;

    if (EVP_DecryptInit_ex(ctx, NULL, NULL, key, nonce) != 1)
        goto cleanup;

    if (ciphertextLen > 0) {
        if (EVP_DecryptUpdate(ctx, output, &len,
                              ciphertext, ciphertextLen) != 1)
            goto cleanup;

        outputLen = len;
    }

    if (EVP_CIPHER_CTX_ctrl(
            ctx, EVP_CTRL_GCM_SET_TAG,
            TAG_LEN, (void *)tag) != 1)
        goto cleanup;

    /* Authentication must succeed before plaintext is accepted. */
    if (EVP_DecryptFinal_ex(ctx, output + outputLen, &len) != 1)
        goto cleanup;

    outputLen += len;
    output[outputLen] = '\0';

    *plaintext = output;
    *plaintextLen = outputLen;
    output = NULL;
    success = 1;

cleanup:
    if (ctx != NULL)
        EVP_CIPHER_CTX_free(ctx);

    if (output != NULL) {
        OPENSSL_cleanse(output, (size_t)ciphertextLen + 1);
        free(output);
    }

    OPENSSL_cleanse(key, sizeof(key));
    return success;
}
