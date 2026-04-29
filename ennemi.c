#include "ennemi.h"
#include <stdlib.h>
#include <string.h>

struct sEnnemi {
    char nom[NOM_MAX];
    int pv, pvMax, atk, def;
};

tEnnemi EnnemiCreer(const char *nom, int pvMax, int atk, int def) {
    tEnnemi e = malloc(sizeof(struct sEnnemi));
    if (!e) return NULL;
    strncpy(e->nom, nom ? nom : "", NOM_MAX - 1);
    e->nom[NOM_MAX - 1] = '\0';
    e->pv    = pvMax;
    e->pvMax = pvMax;
    e->atk   = atk;
    e->def   = def;
    return e;
}

void EnnemiLiberer(tEnnemi *pe) {
    if (!pe || !*pe) return;
    free(*pe);
    *pe = NULL;
}

char *EnnemiNom(const tEnnemi e)       { return e ? e->nom   : NULL; }
int   EnnemiPV(const tEnnemi e)        { return e ? e->pv    : 0; }
int   EnnemiPVMax(const tEnnemi e)     { return e ? e->pvMax : 0; }
int   EnnemiAttaque(const tEnnemi e)   { return e ? e->atk   : 0; }
int   EnnemiDefense(const tEnnemi e)   { return e ? e->def   : 0; }
int   EnnemiEstVivant(const tEnnemi e) { return e && e->pv > 0; }

int EnnemiSubirAttaque(tEnnemi e, const tJoueur j) {
    if (!e || !j) return 0;
    int degats = JoueurAttaque(j) - e->def;
    if (degats < 1) degats = 1;
    e->pv -= degats;
    if (e->pv < 0) e->pv = 0;
    return degats;
}

int EnnemiAttaquerJoueur(const tEnnemi e, tJoueur j) {
    if (!e || !j) return 0;
    int degats = e->atk - JoueurDefense(j);
    if (degats < 1) degats = 1;
    int pv = JoueurPV(j) - degats;
    if (pv < 0) pv = 0;
    JoueurMajPV(j, pv);
    return degats;
}

void EnnemiMajPV(tEnnemi e, int pv) {
    if (e) e->pv = pv;
}
