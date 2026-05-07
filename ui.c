#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ui.h"

struct sUI {
    char message[256];
};


tUI UI_Creer(void)
{
    tUI ui = malloc(sizeof(struct sUI));
    if (ui == NULL)
        return NULL;
    ui->message[0] = '\0';
    return ui;
}


void UI_Liberer(tUI *pui)
{
    if (pui == NULL || *pui == NULL)
        return;
    free(*pui);
    *pui = NULL;
}


void UI_DefinirMessage(tUI ui, const char *fmt, ...)
{
    va_list ap;

    if (ui == NULL || fmt == NULL)
        return;

    va_start(ap, fmt);
    vsnprintf(ui->message, sizeof(ui->message), fmt, ap);
    va_end(ap);
}


/* affiche la mini-carte du donjon */
static void afficher_carte(tDonjon d, tJoueur j)
{
    int w, h;
    int jx, jy;
    int x, y;
    tSalle s;
    char c;

    w = DonjonW(d);
    h = DonjonH(d);
    JoueurPosition(j, &jx, &jy);

    /* bord haut */
    printf("+");
    for (x = 0; x < w; x++)
        printf("-");
    printf("+\n");

    for (y = 0; y < h; y++) {
        printf("|");
        for (x = 0; x < w; x++) {
            s = DonjonSalle(d, x, y);

            if (jx == x && jy == y) {
                c = '@';
            } else if (s == NULL || !SalleExiste(s)) {
                c = '#';
            } else if (!SalleEstVisitee(s)) {
                c = '?';
#ifndef NIV01
            } else if (SalleEnnemi(s) != NULL && EnnemiEstVivant(SalleEnnemi(s))) {
                c = 'E';
#endif
            } else {
                c = '.';
            }
            printf("%c", c);
        }
        printf("|\n");
    }

    /* bord bas */
    printf("+");
    for (x = 0; x < w; x++)
        printf("-");
    printf("+\n");
}


/* affiche les infos de la salle courante */
static void afficher_salle(tDonjon d, tJoueur j)
{
    int jx, jy;
    tSalle s;
    char *txt;
    int k;
    const int dx[4] = { 0,  1,  0, -1 };
    const int dy[4] = { -1,  0,  1,  0 };
    const char *labels[4] = { "N", "E", "S", "W" };
    int sorties;

    JoueurPosition(j, &jx, &jy);
    s = DonjonSalle(d, jx, jy);

    if (s == NULL) {
        fprintf(stderr, "ERREUR: salle introuvable.\n");
        return;
    }

    printf("Salle (%d,%d) : %s\n", jx, jy, SalleDescription(s));

    /* sorties disponibles */
    printf("Sorties :");
    sorties = 0;
    for (k = 0; k < 4; k++) {
        tSalle voisin = DonjonSalle(d, jx + dx[k], jy + dy[k]);
        if (voisin != NULL && SalleExiste(voisin)) {
            printf(" %s", labels[k]);
            sorties = 1;
        }
    }
    if (!sorties)
        printf(" aucune");
    printf("\n");

    /* objets au sol */
    txt = InventaireVersChaine(SalleObjets(s));
    printf("Objets au sol : %s\n", txt);
    free(txt);

#ifndef NIV01
    if (SalleEnnemi(s) == NULL) {
        printf("Ennemi : aucun\n");
    } else {
        tEnnemi e = SalleEnnemi(s);
        printf("Ennemi : %s  PV=%d/%d  ATK=%d  DEF=%d\n",
               EnnemiNom(e), EnnemiPV(e), EnnemiPVMax(e),
               EnnemiAttaque(e), EnnemiDefense(e));
    }
#endif
}


/* affiche l'etat du joueur */
static void afficher_joueur(tUI ui, tJoueur j)
{
    char *txt;

    printf("Joueur : PV=%d/%d  ATK=%d  DEF=%d\n",
           JoueurPV(j), JoueurPVMax(j),
           JoueurAttaque(j), JoueurDefense(j));

    txt = InventaireVersChaine(JoueurInventaire(j));
    printf("Inventaire : %s\n", txt);
    free(txt);

    if (ui->message[0] != '\0')
        printf(">> %s\n", ui->message);
}


void UI_Afficher(tUI ui, tDonjon d, tJoueur j)
{
    printf("\n========== DONJON ==========\n");
    afficher_carte(d, j);
    printf("\n");
    afficher_salle(d, j);
    printf("\n");
    afficher_joueur(ui, j);
    printf("\n> ");
    fflush(stdout);
}


char *UI_LireLigne(tUI ui)
{
    char buf[256];
    char *s;
    size_t len;

    (void)ui;

    if (fgets(buf, sizeof(buf), stdin) == NULL)
        return NULL;

    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
        buf[len - 1] = '\0';

    s = malloc(strlen(buf) + 1);
    if (s == NULL)
        return NULL;

    strcpy(s, buf);
    return s;
}
