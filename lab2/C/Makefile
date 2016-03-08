#COMP = gcc
COMP = gcc

all: produceConsume

produceConsume: produceConsume.c
	$(COMP) -pthread produceConsume.c -o $@

clean:
	rm produceConsume
