#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "save.h"
#include "explore.h"

static void cmd_combat(struct sJeu *g)
{
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tEnnemi e = SalleEnnemi(DonjonSalle(g->d, x, y));

    if (e == NULL || !EnnemiEstVivant(e)) {
        UI_DefinirMessage(g->ui, "Il n'y a pas d'ennemi vivant ici.");
        return;
    }

    int degatsJoueur = EnnemiSubirAttaque(e, g->j);
    if (!EnnemiEstVivant(e)) {
        UI_DefinirMessage(g->ui, "Vous infligez %d degats. %s est mort !",
            degatsJoueur, EnnemiNom(e));
        return;
    }

    int degatsEnnemi = EnnemiAttaquerJoueur(e, g->j);
    if (JoueurPV(g->j) <= 0) {
        UI_DefinirMessage(g->ui,
            "Vous infligez %d degats. L'ennemi vous inflige %d degats. Vous etes mort...",
            degatsJoueur, degatsEnnemi);
        g->fini = 1;
    } else {
        UI_DefinirMessage(g->ui,
            "Vous infligez %d degats. L'ennemi vous inflige %d degats.",
            degatsJoueur, degatsEnnemi);
    }
}

static void executer_commande(struct sJeu *g, char *cmd)
{
    char mot[64];
    char arg[256];

    if (sscanf(cmd, "%63s", mot) != 1)
        return;

    if (strcmp(mot, "quit") == 0 || strcmp(mot, "q") == 0) {
        g->fini = 1;

    } else if (strcmp(mot, "help") == 0) {
        UI_DefinirMessage(g->ui,
            "Commandes: n/e/s/w | take <obj> | drop <obj> | fight | save <f> | load <f> | cheat <f> | quit");

    } else if (strcmp(mot, "n") == 0) {
        int x, y;
        JoueurPosition(g->j, &x, &y);
        tSalle s = DonjonSalle(g->d, x, y - 1);
        if (s == NULL || !SalleExiste(s))
            UI_DefinirMessage(g->ui, "Impossible d'aller dans cette direction.");
        else {
            JoueurMajPos(g->j, x, y - 1);
            SalleVisiter(s);
        }

    } else if (strcmp(mot, "s") == 0) {
        int x, y;
        JoueurPosition(g->j, &x, &y);
        tSalle s = DonjonSalle(g->d, x, y + 1);
        if (s == NULL || !SalleExiste(s))
            UI_DefinirMessage(g->ui, "Impossible d'aller dans cette direction.");
        else {
            JoueurMajPos(g->j, x, y + 1);
            SalleVisiter(s);
        }

    } else if (strcmp(mot, "e") == 0) {
        int x, y;
        JoueurPosition(g->j, &x, &y);
        tSalle s = DonjonSalle(g->d, x + 1, y);
        if (s == NULL || !SalleExiste(s))
            UI_DefinirMessage(g->ui, "Impossible d'aller dans cette direction.");
        else {
            JoueurMajPos(g->j, x + 1, y);
            SalleVisiter(s);
        }

    } else if (strcmp(mot, "w") == 0) {
        int x, y;
        JoueurPosition(g->j, &x, &y);
        tSalle s = DonjonSalle(g->d, x - 1, y);
        if (s == NULL || !SalleExiste(s))
            UI_DefinirMessage(g->ui, "Impossible d'aller dans cette direction.");
        else {
            JoueurMajPos(g->j, x - 1, y);
            SalleVisiter(s);
        }

    } else if (strcmp(mot, "take") == 0) {
        if (sscanf(cmd, "%*s %63s", arg) != 1) {
            UI_DefinirMessage(g->ui, "Usage: take <objet>");
            return;
        }
        int x, y;
        JoueurPosition(g->j, &x, &y);
        tSalle s = DonjonSalle(g->d, x, y);
        if (InventaireQuantite(SalleObjets(s), arg) == 0) {
            UI_DefinirMessage(g->ui, "Il n'y a pas de '%s' ici.", arg);
            return;
        }
        InventaireAjouter(JoueurInventaire(g->j), arg, 1);
        InventaireRetirer(SalleObjets(s), arg, 1);
        UI_DefinirMessage(g->ui, "Vous avez ramasse '%s'.", arg);

    } else if (strcmp(mot, "drop") == 0) {
        if (sscanf(cmd, "%*s %63s", arg) != 1) {
            UI_DefinirMessage(g->ui, "Usage: drop <objet>");
            return;
        }
        if (InventaireQuantite(JoueurInventaire(g->j), arg) == 0) {
            UI_DefinirMessage(g->ui, "Vous n'avez pas de '%s'.", arg);
            return;
        }
        int x, y;
        JoueurPosition(g->j, &x, &y);
        tSalle s = DonjonSalle(g->d, x, y);
        InventaireAjouter(SalleObjets(s), arg, 1);
        InventaireRetirer(JoueurInventaire(g->j), arg, 1);
        UI_DefinirMessage(g->ui, "Vous avez depose '%s'.", arg);

    } else if (strcmp(mot, "fight") == 0 || strcmp(mot, "f") == 0) {
        cmd_combat(g);

    } else if (strcmp(mot, "save") == 0) {
        if (sscanf(cmd, "%*s %255s", arg) != 1) {
            UI_DefinirMessage(g->ui, "Usage: save <fichier>");
        } else if (SaveEcrire(arg, g->d, g->j)) {
            UI_DefinirMessage(g->ui, "Partie sauvegardee dans '%s'.", arg);
        } else {
            UI_DefinirMessage(g->ui, "Erreur sauvegarde.");
        }

    } else if (strcmp(mot, "load") == 0) {
        if (sscanf(cmd, "%*s %255s", arg) != 1) {
            UI_DefinirMessage(g->ui, "Usage: load <fichier>");
        } else if (SaveLire(arg, g->d, g->j)) {
            UI_DefinirMessage(g->ui, "Partie chargee depuis '%s'.", arg);
        } else {
            UI_DefinirMessage(g->ui, "Erreur chargement.");
        }

    } else if (strcmp(mot, "cheat") == 0) {
        if (sscanf(cmd, "%*s %255s", arg) != 1) {
            UI_DefinirMessage(g->ui, "Usage: cheat <fichier>");
        } else if (ExploreTrouverTresor(g->d, g->j, arg)) {
            UI_DefinirMessage(g->ui, "Tresor trouve ! Trace dans '%s'.", arg);
        } else {
            UI_DefinirMessage(g->ui, "Pas de tresor. Trace dans '%s'.", arg);
        }

    } else {
        UI_DefinirMessage(g->ui, "Commande inconnue. Tapez help.");
    }
}


struct sJeu *JeuCreer(const char *fichierDonjon)
{
    struct sJeu *g = (struct sJeu *)malloc(sizeof(struct sJeu));
    if (g == NULL)
        return NULL;

    g->d    = NULL;
    g->j    = NULL;
    g->ui   = NULL;
    g->fini = 0;

    int dx, dy;
    if (!DonjonCharger(fichierDonjon, &g->d, &dx, &dy)) {
        free(g);
        return NULL;
    }

    g->j = JoueurCreer(dx, dy);
    if (g->j == NULL) {
        DonjonLiberer(&g->d);
        free(g);
        return NULL;
    }

    g->ui = UI_Creer();
    if (g->ui == NULL) {
        JoueurLiberer(&g->j);
        DonjonLiberer(&g->d);
        free(g);
        return NULL;
    }

    UI_DefinirMessage(g->ui, "Bienvenue ! Tapez help pour les commandes.");
    return g;
}

void JeuLiberer(struct sJeu *g)
{
    if (g == NULL) return;
    UI_Liberer(&g->ui);
    JoueurLiberer(&g->j);
    DonjonLiberer(&g->d);
    free(g);
}

void JeuJouer(struct sJeu *g)
{
    if (g == NULL) return;
    while (!g->fini) {
        UI_Afficher(g->ui, g->d, g->j);
        char *cmd = UI_LireLigne(g->ui);
        if (cmd == NULL) break;
        executer_commande(g, cmd);
        free(cmd);
    }
}
