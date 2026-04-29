#include "jeu.h"
#include "save.h"
#include "explore.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cmd_help(struct sJeu *g) {
    UI_DefinirMessage(g->ui,
        "Commandes: n/e/s/w | take <obj> | drop <obj> | fight | save <f> | load <f> | cheat <f> | quit | help");
}

static int cmd_move(struct sJeu *g, int dx, int dy) {
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tSalle s = DonjonSalle(g->d, x + dx, y + dy);
    if (!s || !SalleExiste(s)) {
        UI_DefinirMessage(g->ui, "Deplacement impossible.");
        return 0;
    }
    JoueurMajPos(g->j, x + dx, y + dy);
    SalleVisiter(s);
    return 1;
}

static void cmd_take(struct sJeu *g, const char *nom) {
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tSalle s = DonjonSalle(g->d, x, y);
    if (InventaireQuantite(SalleObjets(s), nom) == 0) {
        UI_DefinirMessage(g->ui, "Objet '%s' introuvable dans la salle.", nom);
        return;
    }
    if (!InventaireAjouter(JoueurInventaire(g->j), nom, 1)) {
        UI_DefinirMessage(g->ui, "Erreur lors de la prise de '%s'.", nom);
        return;
    }
    InventaireRetirer(SalleObjets(s), nom, 1);
}

static void cmd_drop(struct sJeu *g, const char *nom) {
    if (InventaireQuantite(JoueurInventaire(g->j), nom) == 0) {
        UI_DefinirMessage(g->ui, "Objet '%s' absent de l'inventaire.", nom);
        return;
    }
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tSalle s = DonjonSalle(g->d, x, y);
    if (!InventaireAjouter(SalleObjets(s), nom, 1)) {
        UI_DefinirMessage(g->ui, "Erreur lors du depot de '%s'.", nom);
        return;
    }
    InventaireRetirer(JoueurInventaire(g->j), nom, 1);
}

static void cmd_fight(struct sJeu *g) {
    int x, y;
    JoueurPosition(g->j, &x, &y);
    tEnnemi e = SalleEnnemi(DonjonSalle(g->d, x, y));
    if (!e || !EnnemiEstVivant(e)) {
        UI_DefinirMessage(g->ui, "Aucun ennemi vivant dans cette salle.");
        return;
    }
    int d1 = EnnemiSubirAttaque(e, g->j);
    if (!EnnemiEstVivant(e)) {
        UI_DefinirMessage(g->ui, "Vous infligez %d degats. %s est mort.", d1, EnnemiNom(e));
        return;
    }
    int d2 = EnnemiAttaquerJoueur(e, g->j);
    if (JoueurPV(g->j) <= 0) {
        UI_DefinirMessage(g->ui,
            "Vous infligez %d degats. L'ennemi riposte (%d). Vous etes mort.", d1, d2);
        g->fini = 1;
    } else {
        UI_DefinirMessage(g->ui,
            "Vous infligez %d degats. L'ennemi riposte (%d).", d1, d2);
    }
}

static void execute_command(struct sJeu *g, char *cmd) {
    char mot[64];
    if (sscanf(cmd, "%63s", mot) != 1) return;

    if (strcmp(mot, "quit") == 0 || strcmp(mot, "q") == 0) {
        g->fini = 1;
    } else if (strcmp(mot, "help") == 0 || strcmp(mot, "h") == 0) {
        cmd_help(g);
    } else if (strcmp(mot, "n") == 0) {
        cmd_move(g, 0, -1);
    } else if (strcmp(mot, "s") == 0) {
        cmd_move(g, 0, 1);
    } else if (strcmp(mot, "e") == 0) {
        cmd_move(g, 1, 0);
    } else if (strcmp(mot, "w") == 0) {
        cmd_move(g, -1, 0);
    } else if (strcmp(mot, "take") == 0) {
        char obj[NOM_MAX];
        if (sscanf(cmd, "%*s %31s", obj) == 1)
            cmd_take(g, obj);
        else
            UI_DefinirMessage(g->ui, "Usage: take <objet>");
    } else if (strcmp(mot, "drop") == 0) {
        char obj[NOM_MAX];
        if (sscanf(cmd, "%*s %31s", obj) == 1)
            cmd_drop(g, obj);
        else
            UI_DefinirMessage(g->ui, "Usage: drop <objet>");
    } else if (strcmp(mot, "fight") == 0 || strcmp(mot, "f") == 0) {
        cmd_fight(g);
    } else if (strcmp(mot, "save") == 0) {
        char fichier[256];
        if (sscanf(cmd, "%*s %255s", fichier) == 1)
            SaveEcrire(fichier, g->d, g->j) ?
                UI_DefinirMessage(g->ui, "Partie sauvegardee : %s", fichier) :
                UI_DefinirMessage(g->ui, "Erreur lors de la sauvegarde.");
        else
            UI_DefinirMessage(g->ui, "Usage: save <fichier>");
    } else if (strcmp(mot, "load") == 0) {
        char fichier[256];
        if (sscanf(cmd, "%*s %255s", fichier) == 1)
            SaveLire(fichier, g->d, g->j) ?
                UI_DefinirMessage(g->ui, "Partie chargee : %s", fichier) :
                UI_DefinirMessage(g->ui, "Erreur lors du chargement.");
        else
            UI_DefinirMessage(g->ui, "Usage: load <fichier>");
    } else if (strcmp(mot, "cheat") == 0) {
        char fichier[256];
        if (sscanf(cmd, "%*s %255s", fichier) == 1)
            ExploreTrouverTresor(g->d, g->j, fichier) ?
                UI_DefinirMessage(g->ui, "Tresor trouve ! Trace: %s", fichier) :
                UI_DefinirMessage(g->ui, "Tresor introuvable. Trace: %s", fichier);
        else
            UI_DefinirMessage(g->ui, "Usage: cheat <tracefile>");
    } else {
        UI_DefinirMessage(g->ui, "Commande inconnue: '%s'. Tapez help.", mot);
    }
}

struct sJeu *JeuCreer(const char *fichierDonjon) {
    struct sJeu *g = malloc(sizeof(struct sJeu));
    if (!g) return NULL;
    g->d = NULL; g->j = NULL; g->ui = NULL; g->fini = 0;

    int dx, dy;
    if (!DonjonCharger(fichierDonjon, &g->d, &dx, &dy)) { free(g); return NULL; }
    g->j = JoueurCreer(dx, dy);
    if (!g->j) { DonjonLiberer(&g->d); free(g); return NULL; }
    g->ui = UI_Creer();
    if (!g->ui) { JoueurLiberer(&g->j); DonjonLiberer(&g->d); free(g); return NULL; }

    UI_DefinirMessage(g->ui, "Bienvenue ! Tapez help pour avoir la liste des commandes");
    return g;
}

void JeuLiberer(struct sJeu *g) {
    if (!g) return;
    UI_Liberer(&g->ui);
    JoueurLiberer(&g->j);
    DonjonLiberer(&g->d);
    free(g);
}

void JeuJouer(struct sJeu *g) {
    if (!g) return;
    while (!g->fini) {
        UI_Afficher(g->ui, g->d, g->j);
        char *cmd = UI_LireLigne(g->ui);
        if (!cmd) break;
        execute_command(g, cmd);
        free(cmd);
    }
}
