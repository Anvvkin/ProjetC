#ifndef SALLE_H
#define SALLE_H

#include "inventaire.h"

/* Type public (pointeur vers structure privee) */
typedef struct sSalle *tSalle;

/* Cree une salle-mur (existe=0). Renvoie NULL en cas d'echec. */
tSalle SalleCreerMur(void);

/* Cree une salle vide accessible (existe=1, non visitee, description par defaut,
 * inventaire vide). Renvoie NULL en cas d'echec (toutes allocations intermediaires liberees). */
tSalle SalleCreerVide(void);

/* Libere la salle et ses ressources. Positionne *ps a NULL. */
void SalleLiberer(tSalle *ps);

/* Getters */
int SalleExiste(const tSalle s);
int SalleEstVisitee(const tSalle s);
const char *SalleDescription(const tSalle s);
tInventaire SalleObjets(const tSalle s);

/* Met a jour la description (copie dynamique). Renvoie 1 si OK, 0 sinon. */
int SalleMajDescription(tSalle s, const char *desc);

/* Marque la salle comme visitee. */
void SalleVisiter(tSalle s);

#endif
