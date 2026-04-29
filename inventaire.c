#include "inventaire.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct sItem {
    char nom[NOM_MAX];
    int quantite;
    struct sItem *suiv;
};

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
        struct sItem *tmp = cur->suiv;
        free(cur);
        cur = tmp;
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

    // vérifier si l'objet est déjà dans la liste
    struct sItem *cur = inv->debut;
    while (cur) {
        if (strncmp(cur->nom, nom, NOM_MAX) == 0) {
            cur->quantite += qte;
            return 1;
        }
        cur = cur->suiv;
    }

    // l'objet n'existe pas, on le crée
    struct sItem *nouveau = malloc(sizeof(struct sItem));
    if (!nouveau) return 0;
    strncpy(nouveau->nom, nom, NOM_MAX - 1);
    nouveau->nom[NOM_MAX - 1] = '\0';
    nouveau->quantite = qte;
    nouveau->suiv = inv->debut;
    inv->debut = nouveau;
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
                if (prev)
                    prev->suiv = cur->suiv;
                else
                    inv->debut = cur->suiv;
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
    int n = 0;
    struct sItem *cur = inv->debut;
    while (cur) {
        n++;
        cur = cur->suiv;
    }
    return n;
}

char *InventaireVersChaine(const tInventaire inv) {
    if (!inv || !inv->debut) {
        char *s = malloc(5);
        if (!s) return NULL;
        strcpy(s, "vide");
        return s;
    }

    int n = InventaireCompter(inv);
    size_t taille = (size_t)n * (NOM_MAX + 6) + 1;
    char *res = malloc(taille);
    if (!res) return NULL;

    res[0] = '\0';
    struct sItem *cur = inv->debut;
    int premier = 1;
    while (cur) {
        if (!premier) strcat(res, ", ");
        char buf[NOM_MAX + 8];
        snprintf(buf, sizeof(buf), "%s(%d)", cur->nom, cur->quantite);
        strcat(res, buf);
        premier = 0;
        cur = cur->suiv;
    }
    return res;
}

int InventaireVisiter(const tInventaire inv,
                      int (*visiter)(const char *, int, void *),
                      void *contexte) {
    if (!inv || !visiter) return 1;
    struct sItem *cur = inv->debut;
    while (cur) {
        if (!visiter(cur->nom, cur->quantite, contexte))
            return 0;
        cur = cur->suiv;
    }
    return 1;
}
