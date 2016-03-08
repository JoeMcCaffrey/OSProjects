COMP = gcc
CFLAGS = -std=c99

all: FileSystem

fileSystem: FileSystem.c
	$(COMP) $(CFLAGS) -pthread FileSystem.c -o $@

clean:
	rm FileSystem
