#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier_donjon>\n", argv[0]);
        return 1;
    }

    struct sJeu *g = JeuCreer(argv[1]);
    if (!g) {
        fprintf(stderr, "Erreur: impossible de creer le jeu depuis '%s'\n", argv[1]);
        return 1;
    }

    JeuJouer(g);
    JeuLiberer(g);
    return 0;
}
