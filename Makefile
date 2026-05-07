CC     = gcc
CFLAGS = -Wall -Wextra -Werror

all: donjon

donjon: main.o inventaire.o joueur.o salle.o donjon.o ennemi.o jeu.o save.o explore.o ui.o
	$(CC) $(CFLAGS) -o $@ $^

test_inventaire: test_inventaire.o inventaire.o
	$(CC) $(CFLAGS) -o $@ $^

test_donjon: test_donjon.o inventaire.o joueur.o salle.o donjon.o ennemi.o ui.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o donjon test_inventaire test_donjon

.PHONY: all clean
