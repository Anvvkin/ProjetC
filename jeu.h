#ifndef JEU_H
#define JEU_H

#include "donjon.h"
#include "joueur.h"
#include "ui.h"

struct sJeu {
    tDonjon d;
    tJoueur j;
    tUI ui;
    int fini;
};

struct sJeu *JeuCreer(const char *fichierDonjon);
void JeuLiberer(struct sJeu *g);
void JeuJouer(struct sJeu *g);

#endif
