CFLAGS = -Wall -pedantic -std=c99 -g -pthread
CC = gcc

all: dine holes

dine: part1.o
	$(CC) $(CFLAGS) -o part1 part1.o

holes: list.o part2.o
	$(CC) $(CFLAGS) -o part2 part2.o list.o

part1.out: part1.c 
	$(CC) $(CFLAGS) -c part1.c

part2.o: part2.c part2.h

list.o: list.c list.h

clean:
	rm -f *.o dine holes
