CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

all: tp8ex1 tp8ex2

tp8ex1: tp8ex1.o matrice.o
	$(CC) $(CFLAGS) -o tp8ex1 tp8ex1.o matrice.o $(LDFLAGS)

tp8ex1.o: tp8ex1.c matrice.h
	$(CC) $(CFLAGS) -c tp8ex1.c

tp8ex2: tp8ex2.o image.o matrice.o
	$(CC) $(CFLAGS) -o tp8ex2 tp8ex2.o image.o matrice.o $(LDFLAGS)

tp8ex2.o: tp8ex2.c image.h matrice.h
	$(CC) $(CFLAGS) -c tp8ex2.c

image.o: image.c image.h matrice.h
	$(CC) $(CFLAGS) -c image.c

matrice.o: matrice.c matrice.h
	$(CC) $(CFLAGS) -c matrice.c

clean:
	rm -f *.o tp8ex1 tp8ex2
