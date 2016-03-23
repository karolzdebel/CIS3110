CFLAGS = -Wall -pedantic -std=c99 -g -pthread
CC = gcc

all: part1 part2

part1: part1.o
	$(CC) $(CFLAGS) -o part1 part1.o

part2: list.o part2.o
	$(CC) $(CFLAGS) -o part2 part2.o list.o

part1.o: part1.c 
	$(CC) $(CFLAGS) -c part1.c

part2.o: part2.c part2.h

list.o: list.c list.h

clean:
	rm -f *.o part1 part2
