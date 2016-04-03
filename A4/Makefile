CC = gcc

all: part1 part2

part1: part1.o
	$(CC) -o part1 part1.o

part2: part2.o
	$(CC) -o part2 part2.o

part1.out: part1.c 
	$(CC) -c part1.c cycle.h

part2.o: part2.c cycle.h

clean:
	rm -f *.o part1 part2
