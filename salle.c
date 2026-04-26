#include "salle.h"
#include <stdlib.h>
#include <string.h>

#define DESC_DEFAUT "Une salle sans particularite."

/* Structure privee d'une salle */
struct sSalle {
    int existe;
    int visitee;
    char *desc;
    tInventaire objets;
};

tSalle SalleCreerMur(void) {
    tSalle s = malloc(sizeof(struct sSalle));
    if (!s) return NULL;
    s->existe  = 0;
    s->visitee = 0;
    s->desc    = NULL;
    s->objets  = NULL;
    return s;
}

tSalle SalleCreerVide(void) {
    tSalle s = malloc(sizeof(struct sSalle));
    if (!s) return NULL;

    s->existe  = 1;
    s->visitee = 0;

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
    }
    free(*ps);
    *ps = NULL;
}

int SalleExiste(const tSalle s)      { return s ? s->existe  : 0; }
int SalleEstVisitee(const tSalle s)  { return s ? s->visitee : 0; }
const char *SalleDescription(const tSalle s) { return s ? s->desc : NULL; }
tInventaire SalleObjets(const tSalle s)      { return s ? s->objets : NULL; }

int SalleMajDescription(tSalle s, const char *desc) {
    if (!s || !desc || !s->existe) return 0;
    char *nouvelle = malloc(strlen(desc) + 1);
    if (!nouvelle) return 0;
    strcpy(nouvelle, desc);
    free(s->desc);
    s->desc = nouvelle;
    return 1;
}

void SalleVisiter(tSalle s) {
    if (s) s->visitee = 1;
}
