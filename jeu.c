#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "save.h"
#include "explore.h"

/* affiche l'aide */
static void cmd_help(struct sJeu *g)
{
    UI_DefinirMessage(g->ui,
        "Commandes: n/e/s/w | take <obj> | drop <obj> | fight | save <f> | load <f> | cheat <f> | help | quit");
}

/* deplace le joueur dans la direction (dx, dy) */
static void cmd_deplacer(struct sJeu *g, int dx, int dy)
{
    int x, y;
    tSalle s;

    JoueurPosition(g->j, &x, &y);
    s = DonjonSalle(g->d, x + dx, y + dy);

    if (s == NULL || !SalleExiste(s)) {
        UI_DefinirMessage(g->ui, "Impossible d'aller dans cette direction.");
        return;
    }

    JoueurMajPos(g->j, x + dx, y + dy);
    SalleVisiter(s);
}

/* ramasse un objet dans la salle */
static void cmd_prendre(struct sJeu *g, const char *nom)
{
    int x, y;
    tSalle s;

    JoueurPosition(g->j, &x, &y);
    s = DonjonSalle(g->d, x, y);

    if (InventaireQuantite(SalleObjets(s), nom) == 0) {
        UI_DefinirMessage(g->ui, "Il n'y a pas de '%s' ici.", nom);
        return;
    }

    if (!InventaireAjouter(JoueurInventaire(g->j), nom, 1)) {
        UI_DefinirMessage(g->ui, "Impossible de prendre '%s'.", nom);
        return;
    }

    InventaireRetirer(SalleObjets(s), nom, 1);
    UI_DefinirMessage(g->ui, "Vous avez ramasse '%s'.", nom);
}

/* pose un objet dans la salle */
static void cmd_poser(struct sJeu *g, const char *nom)
{
    int x, y;
    tSalle s;

    if (InventaireQuantite(JoueurInventaire(g->j), nom) == 0) {
        UI_DefinirMessage(g->ui, "Vous n'avez pas de '%s'.", nom);
        return;
    }

    JoueurPosition(g->j, &x, &y);
    s = DonjonSalle(g->d, x, y);

    if (!InventaireAjouter(SalleObjets(s), nom, 1)) {
        UI_DefinirMessage(g->ui, "Impossible de deposer '%s'.", nom);
        return;
    }

    InventaireRetirer(JoueurInventaire(g->j), nom, 1);
    UI_DefinirMessage(g->ui, "Vous avez depose '%s'.", nom);
}

/* combat contre l'ennemi de la salle */
static void cmd_combat(struct sJeu *g)
{
    int x, y;
    tEnnemi e;
    int degatsJoueur, degatsEnnemi;

    JoueurPosition(g->j, &x, &y);
    e = SalleEnnemi(DonjonSalle(g->d, x, y));

    if (e == NULL || !EnnemiEstVivant(e)) {
        UI_DefinirMessage(g->ui, "Il n'y a pas d'ennemi vivant ici.");
        return;
    }

    /* le joueur attaque d'abord */
    degatsJoueur = EnnemiSubirAttaque(e, g->j);

    if (!EnnemiEstVivant(e)) {
        UI_DefinirMessage(g->ui, "Vous infligez %d degats. %s est mort !", degatsJoueur, EnnemiNom(e));
        return;
    }

    /* l'ennemi riposte */
    degatsEnnemi = EnnemiAttaquerJoueur(e, g->j);

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

/* interprete et execute la commande tapee par le joueur */
static void executer_commande(struct sJeu *g, char *cmd)
{
    char mot[64];
    char arg[256];

    if (sscanf(cmd, "%63s", mot) != 1)
        return;

    if (strcmp(mot, "quit") == 0 || strcmp(mot, "q") == 0) {
        g->fini = 1;

    } else if (strcmp(mot, "help") == 0 || strcmp(mot, "h") == 0) {
        cmd_help(g);

    } else if (strcmp(mot, "n") == 0) {
        cmd_deplacer(g, 0, -1);
    } else if (strcmp(mot, "s") == 0) {
        cmd_deplacer(g, 0, 1);
    } else if (strcmp(mot, "e") == 0) {
        cmd_deplacer(g, 1, 0);
    } else if (strcmp(mot, "w") == 0) {
        cmd_deplacer(g, -1, 0);

    } else if (strcmp(mot, "take") == 0) {
        if (sscanf(cmd, "%*s %63s", arg) == 1)
            cmd_prendre(g, arg);
        else
            UI_DefinirMessage(g->ui, "Usage: take <objet>");

    } else if (strcmp(mot, "drop") == 0) {
        if (sscanf(cmd, "%*s %63s", arg) == 1)
            cmd_poser(g, arg);
        else
            UI_DefinirMessage(g->ui, "Usage: drop <objet>");

    } else if (strcmp(mot, "fight") == 0 || strcmp(mot, "f") == 0) {
        cmd_combat(g);

    } else if (strcmp(mot, "save") == 0) {
        if (sscanf(cmd, "%*s %255s", arg) == 1) {
            if (SaveEcrire(arg, g->d, g->j))
                UI_DefinirMessage(g->ui, "Partie sauvegardee dans '%s'.", arg);
            else
                UI_DefinirMessage(g->ui, "Erreur lors de la sauvegarde.");
        } else {
            UI_DefinirMessage(g->ui, "Usage: save <fichier>");
        }

    } else if (strcmp(mot, "load") == 0) {
        if (sscanf(cmd, "%*s %255s", arg) == 1) {
            if (SaveLire(arg, g->d, g->j))
                UI_DefinirMessage(g->ui, "Partie chargee depuis '%s'.", arg);
            else
                UI_DefinirMessage(g->ui, "Erreur lors du chargement.");
        } else {
            UI_DefinirMessage(g->ui, "Usage: load <fichier>");
        }

    } else if (strcmp(mot, "cheat") == 0) {
        if (sscanf(cmd, "%*s %255s", arg) == 1) {
            if (ExploreTrouverTresor(g->d, g->j, arg))
                UI_DefinirMessage(g->ui, "Tresor trouve ! Trace ecrite dans '%s'.", arg);
            else
                UI_DefinirMessage(g->ui, "Tresor introuvable. Trace ecrite dans '%s'.", arg);
        } else {
            UI_DefinirMessage(g->ui, "Usage: cheat <fichier_trace>");
        }

    } else {
        UI_DefinirMessage(g->ui, "Commande inconnue : '%s'. Tapez help.", mot);
    }
}


struct sJeu *JeuCreer(const char *fichierDonjon)
{
    struct sJeu *g;
    int dx, dy;

    g = malloc(sizeof(struct sJeu));
    if (g == NULL)
        return NULL;

    g->d    = NULL;
    g->j    = NULL;
    g->ui   = NULL;
    g->fini = 0;

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

    UI_DefinirMessage(g->ui, "Bienvenue dans le donjon ! Tapez help pour voir les commandes.");
    return g;
}


void JeuLiberer(struct sJeu *g)
{
    if (g == NULL)
        return;
    UI_Liberer(&g->ui);
    JoueurLiberer(&g->j);
    DonjonLiberer(&g->d);
    free(g);
}


void JeuJouer(struct sJeu *g)
{
    char *cmd;

    if (g == NULL)
        return;

    while (!g->fini) {
        UI_Afficher(g->ui, g->d, g->j);
        cmd = UI_LireLigne(g->ui);
        if (cmd == NULL)
            break;
        executer_commande(g, cmd);
        free(cmd);
    }
}
