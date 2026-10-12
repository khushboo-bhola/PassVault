
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude
TARGET = passvault

SOURCES = src/main.c src/password.c src/file.c src/utils.c src/crypto.c src/vault.c

LIBS = -lcrypto

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean
