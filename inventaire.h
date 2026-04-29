#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#define NOM_MAX 32

/* Type public representant un inventaire (pointeur vers structure privee) */
typedef struct sInventaire *tInventaire;

/* Cree un inventaire vide. Renvoie NULL en cas d'echec d'allocation. */
tInventaire InventaireCreer(void);

/* Retire et libere tous les objets. L'inventaire reste utilisable (vide). */
void InventaireVider(tInventaire inv);

/* Libere tout l'espace memoire et positionne *pinv a NULL. */
void InventaireLiberer(tInventaire *pinv);

/* Ajoute qte unites de nom. Cree l'objet en tete si absent. Renvoie 1 si OK, 0 sinon. */
int InventaireAjouter(tInventaire inv, const char *nom, int qte);

/* Retire qte unites de nom (impossible de retirer plus que disponible).
 * Supprime l'objet si quantite tombe a 0. Renvoie 1 si OK, 0 sinon. */
int InventaireRetirer(tInventaire inv, const char *nom, int qte);

/* Renvoie la quantite de nom dans l'inventaire, 0 si absent ou parametres invalides. */
int InventaireQuantite(const tInventaire inv, const char *nom);

/* Renvoie le nombre d'objets distincts dans l'inventaire. */
int InventaireCompter(const tInventaire inv);

/* Construit et renvoie une chaine allouee dynamiquement de la forme
 * "nom1(q1), nom2(q2)" ou "vide" si vide. A liberer par l'appelant. */
char *InventaireVersChaine(const tInventaire inv);

/* Parcourt tous les objets et appelle visiter(nom, quantite, contexte) pour chacun.
 * Arrete et renvoie 0 si visiter renvoie 0. Renvoie 1 si parcours complet. */
int InventaireVisiter(
    const tInventaire inv,
    int (*visiter)(const char *nom, int quantite, void *contexte),
    void *contexte);

#endif
