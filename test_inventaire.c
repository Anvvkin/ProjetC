#include <stdio.h>
#include <stdlib.h>
#include "inventaire.h"

int main(void)
{
    tInventaire inv;
    char *s;
    int r;

    inv = InventaireCreer();
    if (inv == NULL) {
        fprintf(stderr, "ERREUR InventaireCreer\n");
        return 1;
    }
    printf("creation ok\n");

    s = InventaireVersChaine(inv);
    printf("vide : %s\n", s);
    free(s);
    printf("compter vide : %d\n", InventaireCompter(inv));

    InventaireAjouter(inv, "epee", 1);
    InventaireAjouter(inv, "potion", 3);
    InventaireAjouter(inv, "cle", 1);
    printf("nb apres 3 ajouts : %d\n", InventaireCompter(inv));

    InventaireAjouter(inv, "potion", 2);
    printf("quantite potion (attendu 5) : %d\n", InventaireQuantite(inv, "potion"));

    s = InventaireVersChaine(inv);
    printf("contenu : %s\n", s);
    free(s);

    r = InventaireRetirer(inv, "potion", 2);
    printf("retrait 2 potions (attendu 1) : %d\n", r);
    printf("potion restante (attendu 3) : %d\n", InventaireQuantite(inv, "potion"));

    InventaireRetirer(inv, "cle", 1);
    printf("quantite cle apres retrait total (attendu 0) : %d\n", InventaireQuantite(inv, "cle"));
    printf("nb apres suppression (attendu 2) : %d\n", InventaireCompter(inv));

    r = InventaireRetirer(inv, "potion", 100);
    printf("retrait impossible (attendu 0) : %d\n", r);

    r = InventaireRetirer(inv, "bouclier", 1);
    printf("retrait absent (attendu 0) : %d\n", r);
    printf("quantite absent (attendu 0) : %d\n", InventaireQuantite(inv, "bouclier"));

    InventaireVider(inv);
    s = InventaireVersChaine(inv);
    printf("apres vider : %s\n", s);
    free(s);
    printf("compter apres vider (attendu 0) : %d\n", InventaireCompter(inv));

    InventaireAjouter(inv, "torche", 4);
    s = InventaireVersChaine(inv);
    printf("apres re-ajout : %s\n", s);
    free(s);

    InventaireLiberer(&inv);
    printf("pointeur apres liberation : %s\n", inv == NULL ? "NULL ok" : "ERREUR");

    return 0;
}
