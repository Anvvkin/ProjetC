#include "joueur.h"
#include <stdlib.h>

/* Structure privee d'un joueur */
struct sJoueur {
    int x, y;
    int pv, pvMax;
    int atk, def;
    tInventaire inv;
};

tJoueur JoueurCreer(int x, int y) {
    tJoueur j = malloc(sizeof(struct sJoueur));
    if (!j) return NULL;
    j->x = x;
    j->y = y;
    j->pv    = 20;
    j->pvMax = 20;
    j->atk   = 5;
    j->def   = 1;
    j->inv = InventaireCreer();
    if (!j->inv) {
        free(j);
        return NULL;
    }
    return j;
}

void JoueurLiberer(tJoueur *pj) {
    if (!pj || !*pj) return;
    InventaireLiberer(&(*pj)->inv);
    free(*pj);
    *pj = NULL;
}

void JoueurPosition(const tJoueur j, int *px, int *py) {
    if (!j) return;
    if (px) *px = j->x;
    if (py) *py = j->y;
}

int JoueurPV(const tJoueur j)      { return j ? j->pv    : 0; }
int JoueurPVMax(const tJoueur j)   { return j ? j->pvMax : 0; }
int JoueurAttaque(const tJoueur j) { return j ? j->atk   : 0; }
int JoueurDefense(const tJoueur j) { return j ? j->def   : 0; }

tInventaire JoueurInventaire(const tJoueur j) { return j ? j->inv : NULL; }

void JoueurMajPos(tJoueur j, int x, int y) {
    if (!j) return;
    j->x = x;
    j->y = y;
}

void JoueurMajPV(tJoueur j, int pv) {
    if (j) j->pv = pv;
}
