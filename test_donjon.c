#include "donjon.h"
#include "joueur.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier_donjon>\n", argv[0]);
        return 1;
    }

    tDonjon d = NULL;
    int dx, dy;

    if (!DonjonCharger(argv[1], &d, &dx, &dy)) {
        fprintf(stderr, "Erreur: impossible de charger '%s'\n", argv[1]);
        return 1;
    }

    tJoueur j = JoueurCreer(dx, dy);
    if (!j) {
        fprintf(stderr, "Erreur: impossible de creer le joueur\n");
        DonjonLiberer(&d);
        return 1;
    }

    tUI ui = UI_Creer();
    if (!ui) {
        fprintf(stderr, "Erreur: impossible de creer l'interface\n");
        JoueurLiberer(&j);
        DonjonLiberer(&d);
        return 1;
    }

    UI_Afficher(ui, d, j);

    UI_Liberer(&ui);
    JoueurLiberer(&j);
    DonjonLiberer(&d);
    return 0;
}
