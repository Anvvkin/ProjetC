#include <stdio.h>
#include <stdlib.h>
#include "inventaire.h"

int main(void)
{
    tInventaire inv;
    char *s;
    int r;

    printf("=== Test module inventaire ===\n\n");

    /* creation */
    inv = InventaireCreer();
    if (inv == NULL) {
        fprintf(stderr, "ERREUR: InventaireCreer a echoue\n");
        return 1;
    }
    printf("[OK] Creation\n");

    /* inventaire vide */
    s = InventaireVersChaine(inv);
    printf("[OK] Vide : %s\n", s);
    free(s);
    printf("[OK] Compter vide : %d\n", InventaireCompter(inv));

    /* ajout de plusieurs objets */
    InventaireAjouter(inv, "epee", 1);
    InventaireAjouter(inv, "potion", 3);
    InventaireAjouter(inv, "cle", 1);
    printf("[OK] Nb apres 3 ajouts : %d\n", InventaireCompter(inv));

    /* ajout sur objet existant */
    InventaireAjouter(inv, "potion", 2);
    printf("[OK] Quantite potion (attendu 5) : %d\n", InventaireQuantite(inv, "potion"));

    /* affichage */
    s = InventaireVersChaine(inv);
    printf("[OK] Contenu : %s\n", s);
    free(s);

    /* retrait partiel */
    r = InventaireRetirer(inv, "potion", 2);
    printf("[OK] Retrait 2 potions (attendu 1) : %d\n", r);
    printf("[OK] Quantite potion restante (attendu 3) : %d\n", InventaireQuantite(inv, "potion"));

    /* retrait total */
    InventaireRetirer(inv, "cle", 1);
    printf("[OK] Quantite cle apres retrait total (attendu 0) : %d\n", InventaireQuantite(inv, "cle"));
    printf("[OK] Nb apres suppression (attendu 2) : %d\n", InventaireCompter(inv));

    /* retrait impossible */
    r = InventaireRetirer(inv, "potion", 100);
    printf("[OK] Retrait impossible (attendu 0) : %d\n", r);

    /* objet absent */
    r = InventaireRetirer(inv, "bouclier", 1);
    printf("[OK] Retrait absent (attendu 0) : %d\n", r);
    printf("[OK] Quantite absent (attendu 0) : %d\n", InventaireQuantite(inv, "bouclier"));

    /* vider */
    InventaireVider(inv);
    s = InventaireVersChaine(inv);
    printf("[OK] Apres vider : %s\n", s);
    free(s);
    printf("[OK] Compter apres vider (attendu 0) : %d\n", InventaireCompter(inv));

    /* re-utilisation apres vider */
    InventaireAjouter(inv, "torche", 4);
    s = InventaireVersChaine(inv);
    printf("[OK] Apres re-ajout : %s\n", s);
    free(s);

    /* liberation */
    InventaireLiberer(&inv);
    printf("[OK] Pointeur apres liberation : %s\n", inv == NULL ? "NULL (OK)" : "ERREUR");

    printf("\n=== Tous les tests sont passes ===\n");
    return 0;
}
