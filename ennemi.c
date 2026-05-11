#include <stdlib.h>
#include <string.h>
#include "ennemi.h"

struct sEnnemi {
    char nom[NOM_MAX];
    int pv, pvMax;
    int atk, def;
};


tEnnemi EnnemiCreer(const char *nom, int pvMax, int atk, int def)
{
    tEnnemi e = (tEnnemi)malloc(sizeof(struct sEnnemi));
    if (e == NULL)
        return NULL;
    if (nom != NULL)
        strncpy(e->nom, nom, NOM_MAX - 1);
    else
        e->nom[0] = '\0';
    e->nom[NOM_MAX - 1] = '\0';
    e->pv = pvMax;
    e->pvMax = pvMax;
    e->atk = atk;
    e->def = def;
    return e;
}

void EnnemiLiberer(tEnnemi *pe)
{
    if (pe == NULL || *pe == NULL)
        return;
    free(*pe);
    *pe = NULL;
}

char *EnnemiNom(const tEnnemi e)       { return e->nom; }
int   EnnemiPV(const tEnnemi e)        { return e->pv; }
int   EnnemiPVMax(const tEnnemi e)     { return e->pvMax; }
int   EnnemiAttaque(const tEnnemi e)   { return e->atk; }
int   EnnemiDefense(const tEnnemi e)   { return e->def; }
int   EnnemiEstVivant(const tEnnemi e) { return e->pv > 0; }

int EnnemiSubirAttaque(tEnnemi e, const tJoueur j)
{
    int degats;

    if (e == NULL || j == NULL)
        return 0;
    degats = JoueurAttaque(j) - e->def;
    if (degats < 1) degats = 1;
    e->pv -= degats;
    if (e->pv < 0) e->pv = 0;
    return degats;
}

int EnnemiAttaquerJoueur(const tEnnemi e, tJoueur j)
{
    int degats;
    int nouveauxPV;

    if (e == NULL || j == NULL)
        return 0;
    degats = e->atk - JoueurDefense(j);
    if (degats < 1) degats = 1;
    nouveauxPV = JoueurPV(j) - degats;
    if (nouveauxPV < 0) nouveauxPV = 0;
    JoueurMajPV(j, nouveauxPV);
    return degats;
}

void EnnemiMajPV(tEnnemi e, int pv)
{
    if (e == NULL)
        return;
    e->pv = pv;
}
