CC=gcc

CFLAGS=-std=c99 -Wall -pedantic -g3

Total: Total.o
	$(CC) $(CFLAGS) -o $@ $^

Total.o: Total.c

clean: 
	rm -f *.o "week 1"