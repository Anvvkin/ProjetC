#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "donjon.h"
#include "ennemi.h"

struct sDonjon {
    int w, h;
    tSalle *cases;
};


tDonjon DonjonCreer(int w, int h) {
    if (w <= 0 || h <= 0)
        return NULL;
    tDonjon d = (tDonjon) malloc(sizeof(struct sDonjon));
    if (d == NULL)
        return NULL;
    d->w = w;
    d->h = h;
    d->cases = (tSalle *) malloc((size_t)w * h * sizeof(tSalle));
    if (d->cases == NULL) {
        free(d);
        return NULL;
    }
    return d;
}

void DonjonLiberer(tDonjon *pd) {
    if (pd == NULL || *pd == NULL)
        return;
    int n = (*pd)->w * (*pd)->h;
    for (int i = 0; i < n; i++)
        SalleLiberer(&(*pd)->cases[i]);
    free((*pd)->cases);
    free(*pd);
    *pd = NULL;
}

int DonjonW(const tDonjon d) { return d->w; }
int DonjonH(const tDonjon d) { return d->h; }

tSalle DonjonSalle(const tDonjon d, int x, int y) {
    if (d == NULL || x < 0 || x >= d->w || y < 0 || y >= d->h)
        return NULL;
    return d->cases[y * d->w + x];
}

int DonjonCharger(const char *fichier, tDonjon *dj, int *departX, int *departY) {
    if (fichier == NULL || dj == NULL || departX == NULL || departY == NULL)
        return 0;

    FILE *f = fopen(fichier, "r");
    if (f == NULL)
        return 0;

    char ligne[512];
    if (fgets(ligne, sizeof(ligne), f) == NULL) { fclose(f); return 0; }

    int w, h;
    if (sscanf(ligne, "%d %d", &w, &h) != 2) { fclose(f); return 0; }

    tDonjon d = DonjonCreer(w, h);
    if (d == NULL) { fclose(f); return 0; }

    *departX = 0;
    *departY = 0;

    for (int y = 0; y < h; y++) {
        if (fgets(ligne, sizeof(ligne), f) == NULL) {
            DonjonLiberer(&d);
            fclose(f);
            return 0;
        }
        for (int x = 0; x < w; x++) {
            char c = (x < (int)strlen(ligne)) ? ligne[x] : '.';
            tSalle s;
            if (c == '#') {
                s = SalleCreerMur();
            } else {
                s = SalleCreerVide();
                if (s != NULL && c == '@') {
                    *departX = x;
                    *departY = y;
                    SalleVisiter(s);
                }
            }
            if (s == NULL) {
                DonjonLiberer(&d);
                fclose(f);
                return 0;
            }
            d->cases[y * w + x] = s;
        }
    }

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        size_t len = strlen(ligne);
        while (len > 0 && (ligne[len-1] == '\n' || ligne[len-1] == '\r'))
            ligne[--len] = '\0';

        int x, y, offset;
        if (strncmp(ligne, "DESC ", 5) == 0) {
            offset = 0;
            if (sscanf(ligne + 5, "%d %d %n", &x, &y, &offset) >= 2) {
                tSalle s = DonjonSalle(d, x, y);
                if (s != NULL && SalleExiste(s))
                    SalleMajDescription(s, ligne + 5 + offset);
            }
        } else if (strncmp(ligne, "ITEM ", 5) == 0) {
            char nom[NOM_MAX];
            int qte;
            if (sscanf(ligne + 5, "%d %d %31s %d", &x, &y, nom, &qte) == 4) {
                tSalle s = DonjonSalle(d, x, y);
                if (s != NULL && SalleExiste(s))
                    InventaireAjouter(SalleObjets(s), nom, qte);
            }
        } else if (strncmp(ligne, "ENEMY ", 6) == 0) {
            char nom[NOM_MAX];
            int pv, atk, def;
            if (sscanf(ligne + 6, "%d %d %31s %d %d %d", &x, &y, nom, &pv, &atk, &def) == 6) {
                tSalle s = DonjonSalle(d, x, y);
                if (s != NULL && SalleExiste(s)) {
                    tEnnemi e = EnnemiCreer(nom, pv, atk, def);
                    if (e != NULL)
                        SalleAjouterEnnemi(s, e);
                }
            }
        }
    }

    fclose(f);
    *dj = d;
    return 1;
}
