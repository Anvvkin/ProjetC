#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"

int main(int argc, char *argv[])
{
    struct sJeu *g;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier_donjon>\n", argv[0]);
        return 1;
    }

    g = JeuCreer(argv[1]);
    if (g == NULL) {
        fprintf(stderr, "Erreur : impossible de charger le donjon '%s'.\n", argv[1]);
        return 1;
    }

    JeuJouer(g);
    JeuLiberer(g);

    return 0;
}
