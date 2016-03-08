COMP = gcc
CFLAGS = -std=c99

all: MemManage

produceConsume: MemManage.c
	$(COMP) $(CFLAGS) -pthread MemManage.c -o $@

clean:
	rm MemManage
