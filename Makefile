
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = passvault
SOURCES = src/main.c src/password.c src/file.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean
