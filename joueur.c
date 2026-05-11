#include <stdlib.h>
#include "joueur.h"

struct sJoueur {
    int x, y;
    int pv, pvMax;
    int atk, def;
    tInventaire inv;
};


tJoueur JoueurCreer(int x, int y)
{
    tJoueur j = (tJoueur)malloc(sizeof(struct sJoueur));
    if (j == NULL)
        return NULL;
    j->x = x;
    j->y = y;
    j->pv = 20;
    j->pvMax = 20;
    j->atk = 5;
    j->def = 1;
    j->inv = InventaireCreer();
    if (j->inv == NULL) {
        free(j);
        return NULL;
    }
    return j;
}

void JoueurLiberer(tJoueur *pj)
{
    if (pj == NULL || *pj == NULL)
        return;
    InventaireLiberer(&(*pj)->inv);
    free(*pj);
    *pj = NULL;
}

void JoueurPosition(const tJoueur j, int *px, int *py)
{
    if (j == NULL)
        return;
    if (px != NULL) *px = j->x;
    if (py != NULL) *py = j->y;
}

int JoueurPV(const tJoueur j)      { return j->pv; }
int JoueurPVMax(const tJoueur j)   { return j->pvMax; }
int JoueurAttaque(const tJoueur j) { return j->atk; }
int JoueurDefense(const tJoueur j) { return j->def; }

tInventaire JoueurInventaire(const tJoueur j) { return j->inv; }

void JoueurMajPos(tJoueur j, int x, int y)
{
    if (j == NULL)
        return;
    j->x = x;
    j->y = y;
}

void JoueurMajPV(tJoueur j, int pv)
{
    if (j == NULL)
        return;
    j->pv = pv;
}
