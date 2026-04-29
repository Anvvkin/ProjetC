#include "explore.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int check_tresor(const char *nom, int qte, void *ctx) {
    (void) qte;
    if (strcmp(nom, "tresor") == 0) {
        *((int *) ctx) = 1;
        return 0;
    }
    return 1;
}

static int salle_a_tresor(tSalle s) {
    int found = 0;
    InventaireVisiter(SalleObjets(s), check_tresor, &found);
    return found;
}

// directions : N, E, S, W
static const int DX[4] = { 0,  1,  0, -1};
static const int DY[4] = {-1,  0,  1,  0};
static const char DIR[4] = {'N', 'E', 'S', 'W'};

static int explorerRec(tDonjon d, tJoueur j,
                       int x, int y,
                       unsigned char *visite, FILE *f) {
    int w = DonjonW(d);
    int h = DonjonH(d);

    visite[y * w + x] = 1;
    SalleVisiter(DonjonSalle(d, x, y));
    JoueurMajPos(j, x, y);

    if (salle_a_tresor(DonjonSalle(d, x, y)))
        return 1;

    for (int i = 0; i < 4; i++) {
        int nx = x + DX[i];
        int ny = y + DY[i];
        if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
        tSalle s = DonjonSalle(d, nx, ny);
        if (!s || !SalleExiste(s) || visite[ny * w + nx]) continue;

        fprintf(f, "%c\n", DIR[i]);
        if (explorerRec(d, j, nx, ny, visite, f))
            return 1;

        // retour arrière
        fprintf(f, "%c (BACK)\n", DIR[(i + 2) % 4]);
        JoueurMajPos(j, x, y);
    }

    return 0;
}

int ExploreTrouverTresor(tDonjon d, tJoueur j, const char *fichierTrace) {
    if (!d || !j || !fichierTrace) return 0;

    int x, y;
    JoueurPosition(j, &x, &y);

    FILE *f = fopen(fichierTrace, "w");
    if (!f) return 0;

    int w = DonjonW(d);
    int h = DonjonH(d);
    unsigned char *visite = calloc((size_t)w * h, 1);
    if (!visite) { fclose(f); return 0; }

    fprintf(f, "START %d %d\n", x, y);
    int res = explorerRec(d, j, x, y, visite, f);
    JoueurPosition(j, &x, &y);
    fprintf(f, "END %d %d\n", x, y);
    fprintf(f, "FOUND %d\n", res);

    free(visite);
    fclose(f);
    return res;
}
