
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/crypto.h>

#include "../include/file.h"
#include "../include/crypto.h"

#define DATA_FILE "data/passwords.dat"
#define TEMP_FILE "data/passwords.tmp"
#define MAX_RECORD_SIZE (16 * 1024 * 1024)

static const char *getMasterPassword(void)
{
    const char *p = getenv("PASSVAULT_MASTER");
    return (p != NULL && p[0] != '\0') ? p : NULL;
}

static int writeAll(int fd, const void *buffer, size_t length)
{
    const unsigned char *p = buffer;
    size_t total = 0;

    while (total < length) {
        ssize_t n = write(fd, p + total, length - total);

        if (n < 0 && errno == EINTR)
            continue;

        if (n <= 0)
            return 0;

        total += (size_t)n;
    }

    return 1;
}

static int readAll(int fd, void *buffer, size_t length)
{
    unsigned char *p = buffer;
    size_t total = 0;

    while (total < length) {
        ssize_t n = read(fd, p + total, length - total);

        if (n < 0 && errno == EINTR)
            continue;

        if (n < 0)
            return -1;

        if (n == 0)
            return total == 0 ? 0 : -1;

        total += (size_t)n;
    }

    return 1;
}

static int writeRecord(int fd, const Password *entry,
                       const char *masterPassword)
{
    unsigned char *encrypted = NULL;
    int encryptedLen = 0;
    int ok = 0;

    if (!encryptData((const unsigned char *)entry,
                     (int)sizeof(*entry), masterPassword,
                     &encrypted, &encryptedLen))
        return 0;

    if (encryptedLen <= 0 ||
        (uint32_t)encryptedLen != (uint32_t)encryptedLen ||
        encryptedLen > MAX_RECORD_SIZE)
        goto cleanup;

    /* Length prefix uses native byte order for this local file. */
    uint32_t recordLen = (uint32_t)encryptedLen;

    if (writeAll(fd, &recordLen, sizeof(recordLen)) &&
        writeAll(fd, encrypted, (size_t)encryptedLen))
        ok = 1;

cleanup:
    OPENSSL_cleanse(encrypted, (size_t)(encryptedLen > 0 ?
                                      encryptedLen : 0));
    free(encrypted);
    return ok;
}

int getNextId(void)
{
    int maxId = 0;

    for (int i = 0; i < vaultCount; i++) {
        if (vault[i].id > maxId)
            maxId = vault[i].id;
    }

    return maxId == INT32_MAX ? -1 : maxId + 1;
}

int savePassword(Password entry)
{
    const char *masterPassword = getMasterPassword();

    if (masterPassword == NULL) {
        fprintf(stderr, "PASSVAULT_MASTER is not configured.\n");
        return 0;
    }

    int fd = open(DATA_FILE, O_WRONLY | O_CREAT | O_APPEND, 0600);
    if (fd < 0) {
        perror("Cannot open password file");
        return 0;
    }

    int ok = writeRecord(fd, &entry, masterPassword);

    if (close(fd) != 0)
        ok = 0;

    if (!ok)
        fprintf(stderr, "Could not save encrypted record.\n");

    return ok;
}

int initializeVault(const char *masterPassword)
{
    if (masterPassword == NULL || masterPassword[0] == '\0')
        return 0;

    if (mkdir("data", 0700) != 0 && errno != EEXIST) {
        perror("Cannot create data directory");
        return 0;
    }

    return loadVault(masterPassword);
}

int loadVault(const char *masterPassword)
{
    int fd = open(DATA_FILE, O_RDONLY);

    if (fd < 0) {
        if (errno == ENOENT) {
            vaultCount = 0;
            return 1;
        }

        perror("Cannot open password file");
        return 0;
    }

    Password loaded[MAX_PASSWORDS];
    int count = 0;
    int ok = 1;

    while (1) {
        uint32_t recordLen;
        int rr = readAll(fd, &recordLen, sizeof(recordLen));

        if (rr == 0)
            break;

        if (rr < 0 || recordLen < 44 ||
            recordLen > MAX_RECORD_SIZE) {
            ok = 0;
            break;
        }

        unsigned char *encrypted = malloc(recordLen);
        if (encrypted == NULL) {
            ok = 0;
            break;
        }

        rr = readAll(fd, encrypted, recordLen);

        unsigned char *plain = NULL;
        int plainLen = 0;

        if (rr != 1 ||
            !decryptData(encrypted, (int)recordLen,
                         masterPassword, &plain, &plainLen) ||
            plainLen != (int)sizeof(Password) ||
            count >= MAX_PASSWORDS) {
            ok = 0;
        } else {
            Password entry;
            memcpy(&entry, plain, sizeof(entry));

            if (memchr(entry.website, '\0', sizeof(entry.website)) == NULL ||
                memchr(entry.username, '\0', sizeof(entry.username)) == NULL ||
                memchr(entry.password, '\0', sizeof(entry.password)) == NULL ||
                entry.id <= 0) {
                ok = 0;
            } else {
                loaded[count++] = entry;
            }
        }

        if (plain != NULL) {
            OPENSSL_cleanse(plain, (size_t)(plainLen > 0 ? plainLen : 0));
            free(plain);
        }

        OPENSSL_cleanse(encrypted, recordLen);
        free(encrypted);

        if (!ok)
            break;
    }

    close(fd);

    if (!ok) {
        OPENSSL_cleanse(loaded, sizeof(loaded));
        fprintf(stderr,
                "Vault could not be loaded. Wrong password, corrupt file, "
                "or unsupported file format.\n");
        return 0;
    }

    memcpy(vault, loaded, (size_t)count * sizeof(Password));
    vaultCount = count;
    OPENSSL_cleanse(loaded, sizeof(loaded));
    return 1;
}

int saveVault(const char *masterPassword)
{
    if (masterPassword == NULL || masterPassword[0] == '\0')
        return 0;

    int fd = open(TEMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0600);

    if (fd < 0) {
        perror("Cannot open temporary vault file");
        return 0;
    }

    int ok = 1;

    for (int i = 0; i < vaultCount; i++) {
        if (!writeRecord(fd, &vault[i], masterPassword)) {
            ok = 0;
            break;
        }
    }

    if (ok && fsync(fd) != 0)
        ok = 0;

    if (close(fd) != 0)
        ok = 0;

    if (!ok) {
        unlink(TEMP_FILE);
        fprintf(stderr, "Could not rewrite vault.\n");
        return 0;
    }

    if (rename(TEMP_FILE, DATA_FILE) != 0) {
        perror("Could not replace vault file");
        unlink(TEMP_FILE);
        return 0;
    }

    return 1;
}
