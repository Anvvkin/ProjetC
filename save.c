#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "ennemi.h"

/* en-tete du fichier binaire pour verifier qu'on charge le bon format */
static const char MAGIC[4] = {'D', 'N', 'G', '1'};
static const int  VERSION   = 1;

/* fonction callback pour ecrire un objet dans le fichier (utilisee avec InventaireVisiter) */
static int ecrire_objet(const char *nom, int qte, void *ctx)
{
    FILE *f = (FILE *)ctx;
    char buf[NOM_MAX];

    memset(buf, 0, NOM_MAX);
    strncpy(buf, nom, NOM_MAX - 1);

    if (fwrite(buf, NOM_MAX, 1, f) != 1)
        return 0;
    if (fwrite(&qte, sizeof(int), 1, f) != 1)
        return 0;
    return 1;
}

/* ecrit le contenu d'un inventaire dans le fichier */
static int ecrire_inventaire(tInventaire inv, FILE *f)
{
    int n = InventaireCompter(inv);

    if (fwrite(&n, sizeof(int), 1, f) != 1)
        return 0;
    if (n > 0 && !InventaireVisiter(inv, ecrire_objet, f))
        return 0;
    return 1;
}

/* lit le contenu d'un inventaire depuis le fichier */
static int lire_inventaire(tInventaire inv, FILE *f)
{
    int n, i;
    char nom[NOM_MAX];
    int qte;

    if (fread(&n, sizeof(int), 1, f) != 1)
        return 0;

    for (i = 0; i < n; i++) {
        if (fread(nom, NOM_MAX, 1, f) != 1)
            return 0;
        if (fread(&qte, sizeof(int), 1, f) != 1)
            return 0;
        InventaireAjouter(inv, nom, qte);
    }
    return 1;
}


int SaveEcrire(const char *fichier, tDonjon d, tJoueur j)
{
    FILE *f;
    int w, h;
    int jx, jy, jpv;
    int x, y;

    if (fichier == NULL || d == NULL || j == NULL)
        return 0;

    f = fopen(fichier, "wb");
    if (f == NULL)
        return 0;

    /* en-tete */
    fwrite(MAGIC,    4,           1, f);
    fwrite(&VERSION, sizeof(int), 1, f);

    w = DonjonW(d);
    h = DonjonH(d);
    fwrite(&w, sizeof(int), 1, f);
    fwrite(&h, sizeof(int), 1, f);

    /* position et PV du joueur */
    JoueurPosition(j, &jx, &jy);
    jpv = JoueurPV(j);
    fwrite(&jx,  sizeof(int), 1, f);
    fwrite(&jy,  sizeof(int), 1, f);
    fwrite(&jpv, sizeof(int), 1, f);

    /* inventaire du joueur */
    ecrire_inventaire(JoueurInventaire(j), f);

    /* etat de chaque salle */
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            tSalle s = DonjonSalle(d, x, y);
            unsigned char existe  = (unsigned char)SalleExiste(s);
            unsigned char visitee = (unsigned char)SalleEstVisitee(s);

            fwrite(&existe, 1, 1, f);
            if (!existe)
                continue;

            fwrite(&visitee, 1, 1, f);
            ecrire_inventaire(SalleObjets(s), f);

            /* ennemi eventuel */
            tEnnemi e = SalleEnnemi(s);
            unsigned char has_e = (e != NULL) ? 1 : 0;
            fwrite(&has_e, 1, 1, f);

            if (has_e) {
                char nom[NOM_MAX];
                int pv, pvMax, atk, def;

                memset(nom, 0, NOM_MAX);
                strncpy(nom, EnnemiNom(e), NOM_MAX - 1);
                pv   = EnnemiPV(e);
                pvMax = EnnemiPVMax(e);
                atk  = EnnemiAttaque(e);
                def  = EnnemiDefense(e);

                fwrite(nom,    NOM_MAX,     1, f);
                fwrite(&pv,    sizeof(int), 1, f);
                fwrite(&pvMax, sizeof(int), 1, f);
                fwrite(&atk,   sizeof(int), 1, f);
                fwrite(&def,   sizeof(int), 1, f);
            }
        }
    }

    fclose(f);
    return 1;
}


int SaveLire(const char *fichier, tDonjon d, tJoueur j)
{
    FILE *f;
    char magic[4];
    int version, w, h;
    int jx, jy, jpv;
    int x, y;

    if (fichier == NULL || d == NULL || j == NULL)
        return 0;

    f = fopen(fichier, "rb");
    if (f == NULL)
        return 0;

    /* verification de l'en-tete */
    fread(magic,    4,           1, f);
    fread(&version, sizeof(int), 1, f);
    fread(&w,       sizeof(int), 1, f);
    fread(&h,       sizeof(int), 1, f);

    if (memcmp(magic, MAGIC, 4) != 0) {
        fprintf(stderr, "Erreur : fichier de sauvegarde invalide.\n");
        fclose(f);
        return 0;
    }

    if (w != DonjonW(d) || h != DonjonH(d)) {
        fprintf(stderr, "Erreur : dimensions incompatibles avec le donjon actuel.\n");
        fclose(f);
        return 0;
    }

    /* restauration du joueur */
    fread(&jx,  sizeof(int), 1, f);
    fread(&jy,  sizeof(int), 1, f);
    fread(&jpv, sizeof(int), 1, f);
    JoueurMajPos(j, jx, jy);
    JoueurMajPV(j, jpv);

    InventaireVider(JoueurInventaire(j));
    lire_inventaire(JoueurInventaire(j), f);

    /* restauration des salles */
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            unsigned char existe;
            fread(&existe, 1, 1, f);

            tSalle s = DonjonSalle(d, x, y);
            if ((int)existe != SalleExiste(s)) {
                fprintf(stderr, "Erreur : incoherence salle/mur en (%d,%d).\n", x, y);
                fclose(f);
                return 0;
            }
            if (!existe)
                continue;

            unsigned char visitee;
            fread(&visitee, 1, 1, f);
            if (visitee)
                SalleVisiter(s);

            InventaireVider(SalleObjets(s));
            lire_inventaire(SalleObjets(s), f);

            unsigned char has_e;
            fread(&has_e, 1, 1, f);

            if (has_e) {
                char nom[NOM_MAX];
                int pv, pvMax, atk, def;

                fread(nom,    NOM_MAX,     1, f);
                fread(&pv,    sizeof(int), 1, f);
                fread(&pvMax, sizeof(int), 1, f);
                fread(&atk,   sizeof(int), 1, f);
                fread(&def,   sizeof(int), 1, f);

                tEnnemi e = SalleEnnemi(s);
                if (e != NULL) {
                    EnnemiMajPV(e, pv);
                } else {
                    tEnnemi ne = EnnemiCreer(nom, pvMax, atk, def);
                    if (ne != NULL) {
                        EnnemiMajPV(ne, pv);
                        SalleAjouterEnnemi(s, ne);
                    }
                }
            } else {
                /* ennemi mort, on met ses PV a 0 */
                tEnnemi e = SalleEnnemi(s);
                if (e != NULL)
                    EnnemiMajPV(e, 0);
            }
        }
    }

    fclose(f);
    return 1;
}
