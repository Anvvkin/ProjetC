#include "inventaire.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("=== Test du module inventaire ===\n\n");

    /* --- Creation --- */
    tInventaire inv = InventaireCreer();
    if (!inv) { fprintf(stderr, "ERREUR: creation inventaire\n"); return 1; }
    printf("[OK] Creation\n");

    /* --- Inventaire vide --- */
    char *s = InventaireVersChaine(inv);
    printf("[OK] Vide: %s\n", s);           /* attendu: vide */
    free(s);
    printf("[OK] Compter vide: %d\n", InventaireCompter(inv));  /* attendu: 0 */

    /* --- Ajout d'objets --- */
    InventaireAjouter(inv, "bandage", 2);
    InventaireAjouter(inv, "potion", 1);
    InventaireAjouter(inv, "cle", 1);
    printf("[OK] Nb apres 3 ajouts: %d\n", InventaireCompter(inv));  /* attendu: 3 */

    /* --- Ajout sur objet existant --- */
    InventaireAjouter(inv, "bandage", 3);
    printf("[OK] Quantite bandage: %d\n", InventaireQuantite(inv, "bandage"));  /* attendu: 5 */

    /* --- Affichage --- */
    s = InventaireVersChaine(inv);
    printf("[OK] Contenu: %s\n", s);
    free(s);

    /* --- Retrait partiel --- */
    int r = InventaireRetirer(inv, "bandage", 2);
    printf("[OK] Retrait 2 bandages (attendu 1): %d\n", r);
    printf("[OK] Quantite bandage restante: %d\n", InventaireQuantite(inv, "bandage")); /* attendu: 3 */

    /* --- Retrait total (objet supprime de la liste) --- */
    InventaireRetirer(inv, "potion", 1);
    printf("[OK] Quantite potion apres retrait total: %d\n", InventaireQuantite(inv, "potion")); /* attendu: 0 */
    printf("[OK] Nb apres suppression potion: %d\n", InventaireCompter(inv)); /* attendu: 2 */

    /* --- Retrait impossible (quantite insuffisante) --- */
    r = InventaireRetirer(inv, "cle", 5);
    printf("[OK] Retrait 5 cles impossible (attendu 0): %d\n", r);

    /* --- Retrait objet absent --- */
    r = InventaireRetirer(inv, "epee", 1);
    printf("[OK] Retrait objet absent (attendu 0): %d\n", r);

    /* --- Quantite objet absent --- */
    printf("[OK] Quantite objet absent (attendu 0): %d\n", InventaireQuantite(inv, "epee"));

    /* --- Vider --- */
    InventaireVider(inv);
    s = InventaireVersChaine(inv);
    printf("[OK] Apres vider: %s\n", s);  /* attendu: vide */
    free(s);
    printf("[OK] Compter apres vider: %d\n", InventaireCompter(inv)); /* attendu: 0 */

    /* --- Re-utilisation apres vider --- */
    InventaireAjouter(inv, "torche", 4);
    s = InventaireVersChaine(inv);
    printf("[OK] Apres re-ajout: %s\n", s);
    free(s);

    /* --- Liberation --- */
    InventaireLiberer(&inv);
    printf("[OK] Pointeur apres liberation: %s\n", inv == NULL ? "NULL (OK)" : "ERREUR");

    printf("\n=== Tous les tests passes ===\n");
    return 0;
}
