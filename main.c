#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier_donjon>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct sJeu *g = JeuCreer(argv[1]);
    if (g == NULL) {
        fprintf(stderr, "Erreur chargement donjon '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    JeuJouer(g);
    JeuLiberer(g);
    return EXIT_SUCCESS;
}
