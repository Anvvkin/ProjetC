#ifndef DONJON_H
#define DONJON_H

#include "salle.h"

/* Type public (pointeur vers structure privee) */
typedef struct sDonjon *tDonjon;

/* Cree un donjon w x h. Le tableau de salles est alloue mais non initialise.
 * Renvoie NULL si parametres invalides ou echec d'allocation. */
tDonjon DonjonCreer(int w, int h);

/* Libere toutes les salles, le tableau, et la structure. Positionne *pd a NULL. */
void DonjonLiberer(tDonjon *pd);

/* Getters */
int DonjonH(const tDonjon d);
int DonjonW(const tDonjon d);

/* Renvoie la salle en (x,y) ou NULL si coordonnees invalides.
 * Index dans le tableau = y*w + x. */
tSalle DonjonSalle(const tDonjon d, int x, int y);

/* Charge un donjon depuis un fichier texte.
 * Format : dimensions, grille ('#' mur, '.' salle, '@' depart),
 * puis directives optionnelles DESC/ITEM.
 * Renvoie 1 si succes, 0 sinon. */
int DonjonCharger(const char *fichier, tDonjon *dj, int *departX, int *departY);

#endif
