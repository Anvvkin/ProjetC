CC     = gcc
CFLAGS = -Wall -Wextra -DNIV01

# Cible par defaut (sera remplacee par 'donjon' au niveau 1)
all: test_inventaire

# --- Niveau 0 : tests ---

test_inventaire: test_inventaire.o inventaire.o
	$(CC) $(CFLAGS) -o $@ $^

test_donjon: test_donjon.o inventaire.o joueur.o salle.o donjon.o ui.o
	$(CC) $(CFLAGS) -o $@ $^

# --- Niveau 1 : jeu complet (a activer quand jeu.c et main.c existent) ---
# donjon: main.o inventaire.o joueur.o salle.o donjon.o jeu.o ui.o
# 	$(CC) $(CFLAGS) -o $@ $^

# --- Compilation des .c en .o ---
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- Nettoyage ---
clean:
	rm -f *.o test_inventaire test_donjon donjon

.PHONY: all clean
