CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all: pcode

pcode: pcode.o ht.o
	$(CC) $(CFLAGS) pcode.o ht.o -o pcode

pcode.o: pcode.c ht.h
	$(CC) $(CFLAGS) -c pcode.c

ht.o: ht.c ht.h ht_impl.h
	$(CC) $(CFLAGS) -c ht.c

clean:
	rm -f *.o pcode