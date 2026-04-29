#ifndef SALLE_H
#define SALLE_H

#include "inventaire.h"
#include "ennemi.h"

typedef struct sSalle *tSalle;

tSalle SalleCreerMur(void);
tSalle SalleCreerVide(void);
void SalleLiberer(tSalle *ps);

int SalleExiste(const tSalle s);
int SalleEstVisitee(const tSalle s);
const char *SalleDescription(const tSalle s);
tInventaire SalleObjets(const tSalle s);
tEnnemi SalleEnnemi(tSalle s);

int  SalleMajDescription(tSalle s, const char *desc);
void SalleVisiter(tSalle s);
void SalleAjouterEnnemi(tSalle s, const tEnnemi e);

#endif
