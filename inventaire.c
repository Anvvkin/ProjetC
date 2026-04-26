#include "inventaire.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Structure privee d'un objet de l'inventaire */
struct sItem {
    char nom[NOM_MAX];
    int quantite;
    struct sItem *suiv;
};

/* Structure privee d'un inventaire (liste chainee) */
struct sInventaire {
    struct sItem *debut;
};

tInventaire InventaireCreer(void) {
    tInventaire inv = malloc(sizeof(struct sInventaire));
    if (!inv) return NULL;
    inv->debut = NULL;
    return inv;
}

void InventaireVider(tInventaire inv) {
    if (!inv) return;
    struct sItem *cur = inv->debut;
    while (cur) {
        struct sItem *next = cur->suiv;
        free(cur);
        cur = next;
    }
    inv->debut = NULL;
}

void InventaireLiberer(tInventaire *pinv) {
    if (!pinv || !*pinv) return;
    InventaireVider(*pinv);
    free(*pinv);
    *pinv = NULL;
}

int InventaireAjouter(tInventaire inv, const char *nom, int qte) {
    if (!inv || !nom) return 0;

    /* Si l'objet existe deja, mettre a jour la quantite */
    struct sItem *cur = inv->debut;
    while (cur) {
        if (strncmp(cur->nom, nom, NOM_MAX) == 0) {
            cur->quantite += qte;
            return 1;
        }
        cur = cur->suiv;
    }

    /* Sinon, creer un nouvel objet en tete de liste */
    struct sItem *item = malloc(sizeof(struct sItem));
    if (!item) return 0;
    strncpy(item->nom, nom, NOM_MAX - 1);
    item->nom[NOM_MAX - 1] = '\0';
    item->quantite = qte;
    item->suiv = inv->debut;
    inv->debut = item;
    return 1;
}

int InventaireRetirer(tInventaire inv, const char *nom, int qte) {
    if (!inv || !nom) return 0;

    struct sItem *prev = NULL;
    struct sItem *cur = inv->debut;
    while (cur) {
        if (strncmp(cur->nom, nom, NOM_MAX) == 0) {
            if (cur->quantite < qte) return 0;
            cur->quantite -= qte;
            if (cur->quantite == 0) {
                if (prev) prev->suiv = cur->suiv;
                else inv->debut = cur->suiv;
                free(cur);
            }
            return 1;
        }
        prev = cur;
        cur = cur->suiv;
    }
    return 0;
}

int InventaireQuantite(const tInventaire inv, const char *nom) {
    if (!inv || !nom) return 0;
    struct sItem *cur = inv->debut;
    while (cur) {
        if (strncmp(cur->nom, nom, NOM_MAX) == 0)
            return cur->quantite;
        cur = cur->suiv;
    }
    return 0;
}

int InventaireCompter(const tInventaire inv) {
    if (!inv) return 0;
    int count = 0;
    struct sItem *cur = inv->debut;
    while (cur) {
        count++;
        cur = cur->suiv;
    }
    return count;
}

char *InventaireVersChaine(const tInventaire inv) {
    if (!inv || !inv->debut) {
        char *s = malloc(5);
        if (!s) return NULL;
        strcpy(s, "vide");
        return s;
    }

    /* Taille max : n * (NOM_MAX-1 + "(999)" + ", ") + '\0'
     * = n * (NOM_MAX + 6) suffit largement */
    int n = InventaireCompter(inv);
    size_t size = (size_t)n * (NOM_MAX + 6) + 1;
    char *result = malloc(size);
    if (!result) return NULL;

    result[0] = '\0';
    struct sItem *cur = inv->debut;
    int first = 1;
    while (cur) {
        if (!first) strcat(result, ", ");
        char buf[NOM_MAX + 8];
        snprintf(buf, sizeof(buf), "%s(%d)", cur->nom, cur->quantite);
        strcat(result, buf);
        first = 0;
        cur = cur->suiv;
    }
    return result;
}
