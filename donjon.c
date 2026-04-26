#include "donjon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Structure privee donjon */
struct sDonjon {
    int w, h;
    tSalle *cases; /* tableau 1D de taille w*h, index = y*w+x */
};

tDonjon DonjonCreer(int w, int h) {
    if (w <= 0 || h <= 0) return NULL;
    tDonjon d = malloc(sizeof(struct sDonjon));
    if (!d) return NULL;
    d->w = w;
    d->h = h;
    d->cases = malloc((size_t)w * h * sizeof(tSalle));
    if (!d->cases) {
        free(d);
        return NULL;
    }
    return d;
}

void DonjonLiberer(tDonjon *pd) {
    if (!pd || !*pd) return;
    int n = (*pd)->w * (*pd)->h;
    for (int i = 0; i < n; i++)
        SalleLiberer(&(*pd)->cases[i]);
    free((*pd)->cases);
    free(*pd);
    *pd = NULL;
}

int DonjonH(const tDonjon d) { return d ? d->h : 0; }
int DonjonW(const tDonjon d) { return d ? d->w : 0; }

tSalle DonjonSalle(const tDonjon d, int x, int y) {
    if (!d || x < 0 || x >= d->w || y < 0 || y >= d->h) return NULL;
    return d->cases[y * d->w + x];
}

int DonjonCharger(const char *fichier, tDonjon *dj, int *departX, int *departY) {
    if (!fichier || !dj || !departX || !departY) return 0;

    FILE *f = fopen(fichier, "r");
    if (!f) return 0;

    char line[512];

    /* Lecture des dimensions */
    if (!fgets(line, sizeof(line), f)) { fclose(f); return 0; }
    int w, h;
    if (sscanf(line, "%d %d", &w, &h) != 2) { fclose(f); return 0; }

    tDonjon d = DonjonCreer(w, h);
    if (!d) { fclose(f); return 0; }

    *departX = 0;
    *departY = 0;

    /* Lecture de la grille */
    for (int y = 0; y < h; y++) {
        if (!fgets(line, sizeof(line), f)) {
            DonjonLiberer(&d);
            fclose(f);
            return 0;
        }
        for (int x = 0; x < w; x++) {
            char c = (x < (int)strlen(line)) ? line[x] : '.';
            tSalle s;
            if (c == '#') {
                s = SalleCreerMur();
            } else {
                s = SalleCreerVide();
                if (s && c == '@') {
                    *departX = x;
                    *departY = y;
                    SalleVisiter(s);
                }
            }
            if (!s) {
                DonjonLiberer(&d);
                fclose(f);
                return 0;
            }
            d->cases[y * w + x] = s;
        }
    }

    /* Lecture des directives optionnelles */
    while (fgets(line, sizeof(line), f)) {
        /* Supprimer le retour chariot/saut de ligne */
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        int x, y, n_lu;
        char nom[NOM_MAX];
        int qte;

        if (strncmp(line, "DESC ", 5) == 0) {
            if (sscanf(line + 5, "%d %d %n", &x, &y, &n_lu) >= 2) {
                char *desc = line + 5 + n_lu;
                tSalle s = DonjonSalle(d, x, y);
                if (s && SalleExiste(s))
                    SalleMajDescription(s, desc);
            }
        } else if (strncmp(line, "ITEM ", 5) == 0) {
            if (sscanf(line + 5, "%d %d %31s %d", &x, &y, nom, &qte) == 4) {
                tSalle s = DonjonSalle(d, x, y);
                if (s && SalleExiste(s))
                    InventaireAjouter(SalleObjets(s), nom, qte);
            }
        }
        /* Directives inconnues : ignorees silencieusement */
    }

    fclose(f);
    *dj = d;
    return 1;
}
