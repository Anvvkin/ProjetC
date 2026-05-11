#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "explore.h"

static int chercher_tresor(const char *nom, int qte, void *ctx)
{
    (void)qte;
    if (strcmp(nom, "tresor") == 0) {
        *((int *)ctx) = 1;
        return 0;
    }
    return 1;
}

static int salle_a_tresor(tSalle s)
{
    int found = 0;
    InventaireVisiter(SalleObjets(s), chercher_tresor, &found);
    return found;
}

static int explorer_rec(tDonjon d, tJoueur j, int x, int y, unsigned char *visite, FILE *f)
{
    int w = DonjonW(d);
    int h = DonjonH(d);
    tSalle s;

    visite[y * w + x] = 1;
    SalleVisiter(DonjonSalle(d, x, y));
    JoueurMajPos(j, x, y);

    if (salle_a_tresor(DonjonSalle(d, x, y)))
        return 1;

    /* nord */
    if (y - 1 >= 0) {
        s = DonjonSalle(d, x, y - 1);
        if (s != NULL && SalleExiste(s) && !visite[(y-1) * w + x]) {
            fprintf(f, "N\n");
            if (explorer_rec(d, j, x, y - 1, visite, f))
                return 1;
            JoueurMajPos(j, x, y);
            fprintf(f, "S (BACK)\n");
        }
    }

    /* est */
    if (x + 1 < w) {
        s = DonjonSalle(d, x + 1, y);
        if (s != NULL && SalleExiste(s) && !visite[y * w + x + 1]) {
            fprintf(f, "E\n");
            if (explorer_rec(d, j, x + 1, y, visite, f))
                return 1;
            JoueurMajPos(j, x, y);
            fprintf(f, "W (BACK)\n");
        }
    }

    /* sud */
    if (y + 1 < h) {
        s = DonjonSalle(d, x, y + 1);
        if (s != NULL && SalleExiste(s) && !visite[(y+1) * w + x]) {
            fprintf(f, "S\n");
            if (explorer_rec(d, j, x, y + 1, visite, f)) return 1;
            JoueurMajPos(j, x, y);
            fprintf(f, "N (BACK)\n");
        }
    }

    /* ouest */
    if (x - 1 >= 0) {
        s = DonjonSalle(d, x - 1, y);
        if (!visite[y * w + x - 1] && s != NULL && SalleExiste(s)) {
            fprintf(f, "W\n");
            if (explorer_rec(d, j, x - 1, y, visite, f))
                return 1;
            JoueurMajPos(j, x, y);
            fprintf(f, "E (BACK)\n");
        }
    }

    return 0;
}


int ExploreTrouverTresor(tDonjon d, tJoueur j, const char *fichierTrace)
{
    int x, y;

    if (d == NULL || j == NULL || fichierTrace == NULL)
        return 0;

    FILE *f = fopen(fichierTrace, "w");
    if (f == NULL)
        return 0;

    JoueurPosition(j, &x, &y);
    int w = DonjonW(d);
    int h = DonjonH(d);

    unsigned char *visite = (unsigned char *)calloc((size_t)w * (size_t)h, 1);
    if (visite == NULL) {
        fclose(f);
        return 0;
    }

    fprintf(f, "START %d %d\n", x, y);
    int res = explorer_rec(d, j, x, y, visite, f);

    JoueurPosition(j, &x, &y);
    fprintf(f, "END %d %d\n", x, y);
    fprintf(f, "FOUND %d\n", res);

    free(visite);
    fclose(f);
    return res;
}
