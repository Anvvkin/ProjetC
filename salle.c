#include "salle.h"
#include <stdlib.h>
#include <string.h>

#define DESC_DEFAUT "Une salle sans particularite."

struct sSalle {
    int existe;
    int visitee;
    char *desc;
    tInventaire objets;
    tEnnemi ennemi;
};

tSalle SalleCreerMur(void) {
    tSalle s = malloc(sizeof(struct sSalle));
    if (!s) return NULL;
    s->existe  = 0;
    s->visitee = 0;
    s->desc    = NULL;
    s->objets  = NULL;
    s->ennemi  = NULL;
    return s;
}

tSalle SalleCreerVide(void) {
    tSalle s = malloc(sizeof(struct sSalle));
    if (!s) return NULL;
    s->existe  = 1;
    s->visitee = 0;
    s->ennemi  = NULL;

    s->desc = malloc(strlen(DESC_DEFAUT) + 1);
    if (!s->desc) {
        free(s);
        return NULL;
    }
    strcpy(s->desc, DESC_DEFAUT);

    s->objets = InventaireCreer();
    if (!s->objets) {
        free(s->desc);
        free(s);
        return NULL;
    }
    return s;
}

void SalleLiberer(tSalle *ps) {
    if (!ps || !*ps) return;
    if ((*ps)->existe) {
        free((*ps)->desc);
        InventaireLiberer(&(*ps)->objets);
        EnnemiLiberer(&(*ps)->ennemi);
    }
    free(*ps);
    *ps = NULL;
}

int SalleExiste(const tSalle s)     { return s ? s->existe  : 0; }
int SalleEstVisitee(const tSalle s) { return s ? s->visitee : 0; }
const char *SalleDescription(const tSalle s) { return s ? s->desc   : NULL; }
tInventaire SalleObjets(const tSalle s)      { return s ? s->objets : NULL; }
tEnnemi SalleEnnemi(tSalle s)                { return s ? s->ennemi : NULL; }

int SalleMajDescription(tSalle s, const char *desc) {
    if (!s || !desc || !s->existe) return 0;
    char *tmp = malloc(strlen(desc) + 1);
    if (!tmp) return 0;
    strcpy(tmp, desc);
    free(s->desc);
    s->desc = tmp;
    return 1;
}

void SalleVisiter(tSalle s) {
    if (s) s->visitee = 1;
}

void SalleAjouterEnnemi(tSalle s, const tEnnemi e) {
    if (s) s->ennemi = e;
}
