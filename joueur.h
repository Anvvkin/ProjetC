#ifndef JOUEUR_H
#define JOUEUR_H

#include "inventaire.h"

/* Type public (pointeur vers structure privee) */
typedef struct sJoueur *tJoueur;

/* Cree un joueur en (x,y) avec stats par defaut (pv=20, pvMax=20, atk=5, def=1).
 * Cree aussi un inventaire vide. Renvoie NULL en cas d'echec. */
tJoueur JoueurCreer(int x, int y);

/* Libere le joueur et son inventaire. Positionne *pj a NULL. */
void JoueurLiberer(tJoueur *pj);

/* Getters */
void JoueurPosition(const tJoueur j, int *px, int *py);
int JoueurPV(const tJoueur j);
int JoueurPVMax(const tJoueur j);
int JoueurAttaque(const tJoueur j);
int JoueurDefense(const tJoueur j);
tInventaire JoueurInventaire(const tJoueur j);

/* Setters */
void JoueurMajPos(tJoueur j, int x, int y);
void JoueurMajPV(tJoueur j, int pv);

#endif
