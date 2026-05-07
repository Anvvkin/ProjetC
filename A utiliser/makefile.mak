CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra -Werror=vla
LDLIBS=-lm
RM=rm -f
OBJ=tp8ex1.o matrice.o
EXE=tp8ex1

# Pour produire le fichier exécutable
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)

# Pour produire les fichiers objets
tp8ex1.o:tp8ex1.c matrice.h
	$(CC) $(CFLAGS) -c tp8ex1.c

matrice.o:matrice.c matrice.h
	$(CC) $(CFLAGS) -c matrice.c

# Pour effacer les objets et les exécutables
clean:
	$(RM) $(OBJ) $(EXE)