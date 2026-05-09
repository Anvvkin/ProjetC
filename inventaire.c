#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventaire.h"

struct sItem {
    char nom[NOM_MAX];
    int quantite;
    struct sItem *suiv;
};

struct sInventaire {
    struct sItem *debut;
};


tInventaire InventaireCreer(void) {
    tInventaire inv = (tInventaire) malloc(sizeof(struct sInventaire));
    if (inv == NULL)
        return NULL;
    inv->debut = NULL;
    return inv;
}

void InventaireVider(tInventaire inv) {
    if (inv == NULL)
        return;
    struct sItem *cur = inv->debut;
    while (cur != NULL) {
        struct sItem *tmp = cur->suiv;
        free(cur);
        cur = tmp;
    }
    inv->debut = NULL;
}

void InventaireLiberer(tInventaire *pinv) {
    if (pinv == NULL || *pinv == NULL)
        return;
    InventaireVider(*pinv);
    free(*pinv);
    *pinv = NULL;
}

int InventaireAjouter(tInventaire inv, const char *nom, int qte) {
    if (inv == NULL || nom == NULL)
        return 0;

    struct sItem *cur = inv->debut;
    while (cur != NULL) {
        if (strncmp(cur->nom, nom, NOM_MAX) == 0) {
            cur->quantite += qte;
            return 1;
        }
        cur = cur->suiv;
    }

    struct sItem *nouveau = (struct sItem *) malloc(sizeof(struct sItem));
    if (nouveau == NULL)
        return 0;
    strncpy(nouveau->nom, nom, NOM_MAX - 1);
    nouveau->nom[NOM_MAX - 1] = '\0';
    nouveau->quantite = qte;
    nouveau->suiv = inv->debut;
    inv->debut = nouveau;
    return 1;
}

int InventaireRetirer(tInventaire inv, const char *nom, int qte) {
    if (inv == NULL || nom == NULL)
        return 0;

    struct sItem *prev = NULL;
    struct sItem *cur = inv->debut;
    while (cur != NULL) {
        if (strncmp(cur->nom, nom, NOM_MAX) == 0) {
            if (cur->quantite < qte)
                return 0;
            cur->quantite -= qte;
            if (cur->quantite == 0) {
                if (prev != NULL)
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
    if (inv == NULL || nom == NULL)
        return 0;
    struct sItem *cur = inv->debut;
    while (cur != NULL) {
        if (strncmp(cur->nom, nom, NOM_MAX) == 0)
            return cur->quantite;
        cur = cur->suiv;
    }
    return 0;
}

int InventaireCompter(const tInventaire inv) {
    if (inv == NULL)
        return 0;
    int n = 0;
    struct sItem *cur = inv->debut;
    while (cur != NULL) {
        n++;
        cur = cur->suiv;
    }
    return n;
}

char *InventaireVersChaine(const tInventaire inv) {
    if (inv == NULL || inv->debut == NULL) {
        char *s = (char *) malloc(5);
        if (s == NULL) return NULL;
        strcpy(s, "vide");
        return s;
    }

    int n = InventaireCompter(inv);
    char *res = (char *) malloc((size_t)n * (NOM_MAX + 8) + 1);
    if (res == NULL)
        return NULL;

    res[0] = '\0';
    int premier = 1;
    struct sItem *cur = inv->debut;
    while (cur != NULL) {
        if (!premier) strcat(res, ", ");
        char buf[NOM_MAX + 16];
        snprintf(buf, sizeof(buf), "%s(%d)", cur->nom, cur->quantite);
        strcat(res, buf);
        premier = 0;
        cur = cur->suiv;
    }
    return res;
}

int InventaireVisiter(const tInventaire inv,
                      int (*visiter)(const char *nom, int quantite, void *contexte),
                      void *contexte) {
    if (inv == NULL || visiter == NULL)
        return 1;
    struct sItem *cur = inv->debut;
    while (cur != NULL) {
        if (!visiter(cur->nom, cur->quantite, contexte))
            return 0;
        cur = cur->suiv;
    }
    return 1;
}
