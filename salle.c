#include <stdlib.h>
#include <string.h>
#include "salle.h"

#define DESC_DEFAUT "Une salle sans particularite."

struct sSalle {
    int existe;
    int visitee;
    char *desc;
    tInventaire objets;
    tEnnemi ennemi;
};


tSalle SalleCreerMur(void)
{
    tSalle s = malloc(sizeof(struct sSalle));
    if (s == NULL)
        return NULL;

    s->existe  = 0;
    s->visitee = 0;
    s->desc    = NULL;
    s->objets  = NULL;
    s->ennemi  = NULL;

    return s;
}


tSalle SalleCreerVide(void)
{
    tSalle s = malloc(sizeof(struct sSalle));
    if (s == NULL)
        return NULL;

    s->existe  = 1;
    s->visitee = 0;
    s->ennemi  = NULL;

    /* description par defaut */
    s->desc = malloc(strlen(DESC_DEFAUT) + 1);
    if (s->desc == NULL) {
        free(s);
        return NULL;
    }
    strcpy(s->desc, DESC_DEFAUT);

    /* inventaire vide pour les objets au sol */
    s->objets = InventaireCreer();
    if (s->objets == NULL) {
        free(s->desc);
        free(s);
        return NULL;
    }

    return s;
}


void SalleLiberer(tSalle *ps)
{
    if (ps == NULL || *ps == NULL)
        return;

    if ((*ps)->existe) {
        free((*ps)->desc);
        InventaireLiberer(&(*ps)->objets);
        EnnemiLiberer(&(*ps)->ennemi);
    }

    free(*ps);
    *ps = NULL;
}


int SalleExiste(const tSalle s)
{
    if (s == NULL)
        return 0;
    return s->existe;
}


int SalleEstVisitee(const tSalle s)
{
    if (s == NULL)
        return 0;
    return s->visitee;
}


const char *SalleDescription(const tSalle s)
{
    if (s == NULL)
        return NULL;
    return s->desc;
}


tInventaire SalleObjets(const tSalle s)
{
    if (s == NULL)
        return NULL;
    return s->objets;
}


tEnnemi SalleEnnemi(tSalle s)
{
    if (s == NULL)
        return NULL;
    return s->ennemi;
}


int SalleMajDescription(tSalle s, const char *desc)
{
    char *tmp;

    if (s == NULL || desc == NULL || !s->existe)
        return 0;

    tmp = malloc(strlen(desc) + 1);
    if (tmp == NULL)
        return 0;

    strcpy(tmp, desc);
    free(s->desc);
    s->desc = tmp;

    return 1;
}


void SalleVisiter(tSalle s)
{
    if (s == NULL)
        return;
    s->visitee = 1;
}


void SalleAjouterEnnemi(tSalle s, const tEnnemi e)
{
    if (s == NULL)
        return;
    s->ennemi = e;
}
