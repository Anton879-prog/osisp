CC = gcc
CFLAGS = -pedantic -W -Wall -Wextra
DEBUG_FLAGS = -g -ggdb -std=c11
RELEASE_FLAGS = -std=c11 -Werror
TARGET = dirwalk
SRC = src/dirwalk.c

# Каталоги для сборки
DEBUG_DIR = build/debug
RELEASE_DIR = build/release

all: debug release

debug: $(SRC)
	@mkdir -p $(DEBUG_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(DEBUG_DIR)/$(TARGET) $(SRC)

release: $(SRC)
	@mkdir -p $(RELEASE_DIR)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $(RELEASE_DIR)/$(TARGET) $(SRC)

clean:
	rm -rf build/
