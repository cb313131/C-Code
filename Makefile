CC =gcc

CFLAGS =-std=c99 -pedantic -Wall 

HWK = /c/cs223/Hwk2

Pack: Pack.o
	$(CC) $(CFLAGS) -o Pack Pack.o

Subst16: Subst16.o library.o
	$(CC) $(CFLAGS) $^ -o $@

Subst16.o library.o: library.h