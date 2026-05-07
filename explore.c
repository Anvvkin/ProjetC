#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "explore.h"

/* directions : Nord, Est, Sud, Ouest */
static const int DX[4]   = {  0,  1,  0, -1 };
static const int DY[4]   = { -1,  0,  1,  0 };
static const char DIR[4] = { 'N', 'E', 'S', 'W' };

/* callback pour chercher un objet "tresor" dans l'inventaire */
static int chercher_tresor(const char *nom, int qte, void *ctx)
{
    (void)qte;
    if (strcmp(nom, "tresor") == 0) {
        *((int *)ctx) = 1;
        return 0; /* on arrete la visite */
    }
    return 1;
}

/* renvoie 1 si la salle contient un objet "tresor" */
static int salle_a_tresor(tSalle s)
{
    int found = 0;
    InventaireVisiter(SalleObjets(s), chercher_tresor, &found);
    return found;
}

/* exploration DFS recursive
 * renvoie 1 si le tresor est trouve, 0 sinon */
static int explorer_rec(tDonjon d, tJoueur j,
                        int x, int y,
                        unsigned char *visite, FILE *f)
{
    int w = DonjonW(d);
    int h = DonjonH(d);
    int i, nx, ny;
    tSalle s;

    /* on marque la salle comme visitee */
    visite[y * w + x] = 1;
    SalleVisiter(DonjonSalle(d, x, y));
    JoueurMajPos(j, x, y);

    /* on verifie si on a trouve le tresor */
    if (salle_a_tresor(DonjonSalle(d, x, y)))
        return 1;

    /* on essaie les 4 directions */
    for (i = 0; i < 4; i++) {
        nx = x + DX[i];
        ny = y + DY[i];

        if (nx < 0 || nx >= w || ny < 0 || ny >= h)
            continue;

        s = DonjonSalle(d, nx, ny);
        if (s == NULL || !SalleExiste(s))
            continue;
        if (visite[ny * w + nx])
            continue;

        fprintf(f, "%c\n", DIR[i]);

        if (explorer_rec(d, j, nx, ny, visite, f))
            return 1;

        /* retour arriere */
        fprintf(f, "%c (BACK)\n", DIR[(i + 2) % 4]);
        JoueurMajPos(j, x, y);
    }

    return 0;
}


int ExploreTrouverTresor(tDonjon d, tJoueur j, const char *fichierTrace)
{
    FILE *f;
    unsigned char *visite;
    int w, h;
    int x, y;
    int res;

    if (d == NULL || j == NULL || fichierTrace == NULL)
        return 0;

    JoueurPosition(j, &x, &y);

    f = fopen(fichierTrace, "w");
    if (f == NULL)
        return 0;

    w = DonjonW(d);
    h = DonjonH(d);

    visite = calloc((size_t)w * (size_t)h, 1);
    if (visite == NULL) {
        fclose(f);
        return 0;
    }

    fprintf(f, "START %d %d\n", x, y);
    res = explorer_rec(d, j, x, y, visite, f);

    JoueurPosition(j, &x, &y);
    fprintf(f, "END %d %d\n", x, y);
    fprintf(f, "FOUND %d\n", res);

    free(visite);
    fclose(f);
    return res;
}
