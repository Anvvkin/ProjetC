#include <stdio.h>
#include <stdlib.h>
#include "donjon.h"
#include "joueur.h"
#include "ui.h"

int main(int argc, char *argv[])
{
    tDonjon d;
    tJoueur j;
    tUI ui;
    int dx, dy;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier_donjon>\n", argv[0]);
        return 1;
    }

    if (!DonjonCharger(argv[1], &d, &dx, &dy)) {
        fprintf(stderr, "Erreur : impossible de charger '%s'.\n", argv[1]);
        return 1;
    }

    j = JoueurCreer(dx, dy);
    if (j == NULL) {
        fprintf(stderr, "Erreur : impossible de creer le joueur.\n");
        DonjonLiberer(&d);
        return 1;
    }

    ui = UI_Creer();
    if (ui == NULL) {
        fprintf(stderr, "Erreur : impossible de creer l'interface.\n");
        JoueurLiberer(&j);
        DonjonLiberer(&d);
        return 1;
    }

    printf("Donjon charge : %dx%d, depart en (%d,%d)\n",
           DonjonW(d), DonjonH(d), dx, dy);

    UI_Afficher(ui, d, j);

    UI_Liberer(&ui);
    JoueurLiberer(&j);
    DonjonLiberer(&d);

    return 0;
}
