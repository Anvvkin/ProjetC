#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "ennemi.h"

static const char MAGIC[4] = {'D', 'N', 'G', '1'};
static const int VERSION = 1;

static int ecrire_objet(const char *nom, int qte, void *ctx)
{
    FILE *f = (FILE *)ctx;
    char buf[NOM_MAX];

    memset(buf, 0, NOM_MAX);
    strncpy(buf, nom, NOM_MAX - 1);
    if (fwrite(buf, NOM_MAX, 1, f) != 1) return 0;
    if (fwrite(&qte, sizeof(int), 1, f) != 1) return 0;
    return 1;
}

static int ecrire_inv(tInventaire inv, FILE *f)
{
    int n = InventaireCompter(inv);
    if (fwrite(&n, sizeof(int), 1, f) != 1) return 0;
    if (n > 0 && !InventaireVisiter(inv, ecrire_objet, f)) return 0;
    return 1;
}

static int lire_inv(tInventaire inv, FILE *f)
{
    int n, i, qte;
    char nom[NOM_MAX];

    if (fread(&n, sizeof(int), 1, f) != 1) return 0;
    for (i = 0; i < n; i++) {
        if (fread(nom, NOM_MAX, 1, f) != 1) return 0;
        if (fread(&qte, sizeof(int), 1, f) != 1) return 0;
        InventaireAjouter(inv, nom, qte);
    }
    return 1;
}


int SaveEcrire(const char *fichier, tDonjon d, tJoueur j)
{
    if (fichier == NULL || d == NULL || j == NULL)
        return 0;

    FILE *f = fopen(fichier, "wb");
    if (f == NULL)
        return 0;

    fwrite(MAGIC,    4,           1, f);
    fwrite(&VERSION, sizeof(int), 1, f);

    int w = DonjonW(d);
    int h = DonjonH(d);
    fwrite(&w, sizeof(int), 1, f);
    fwrite(&h, sizeof(int), 1, f);

    int jx, jy;
    JoueurPosition(j, &jx, &jy);
    int jpv = JoueurPV(j);
    fwrite(&jx,  sizeof(int), 1, f);
    fwrite(&jy,  sizeof(int), 1, f);
    fwrite(&jpv, sizeof(int), 1, f);

    ecrire_inv(JoueurInventaire(j), f);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            tSalle s = DonjonSalle(d, x, y);
            unsigned char existe  = (unsigned char)SalleExiste(s);
            unsigned char visitee = (unsigned char)SalleEstVisitee(s);

            fwrite(&existe, 1, 1, f);
            if (!existe)
                continue;

            fwrite(&visitee, 1, 1, f);
            ecrire_inv(SalleObjets(s), f);

            tEnnemi e = SalleEnnemi(s);
            unsigned char has_e = (e != NULL) ? 1 : 0;
            fwrite(&has_e, 1, 1, f);

            if (has_e) {
                char nom[NOM_MAX];
                memset(nom, 0, NOM_MAX);
                strncpy(nom, EnnemiNom(e), NOM_MAX - 1);
                int pv = EnnemiPV(e);
                int pvMax = EnnemiPVMax(e);
                int atk = EnnemiAttaque(e);
                int def = EnnemiDefense(e);

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
    int w, h, x, y;
    int jx, jy, jpv;
    tSalle s;
    tEnnemi e;
    unsigned char existe, visitee, has_e;
    char nom[NOM_MAX];
    int pv, pvMax, atk, def;

    if (fichier == NULL || d == NULL || j == NULL)
        return 0;

    FILE *f = fopen(fichier, "rb");
    if (f == NULL)
        return 0;

    char magic[4];
    int version;
    fread(magic,    4,           1, f);
    fread(&version, sizeof(int), 1, f);
    fread(&w,       sizeof(int), 1, f);
    fread(&h,       sizeof(int), 1, f);

    if (memcmp(magic, MAGIC, 4) != 0 || version != VERSION) {
        fclose(f);
        return 0;
    }
    if (w != DonjonW(d) || h != DonjonH(d)) {
        fclose(f);
        return 0;
    }

    fread(&jx,  sizeof(int), 1, f);
    fread(&jy,  sizeof(int), 1, f);
    fread(&jpv, sizeof(int), 1, f);
    JoueurMajPos(j, jx, jy);
    JoueurMajPV(j, jpv);

    InventaireVider(JoueurInventaire(j));
    lire_inv(JoueurInventaire(j), f);

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            fread(&existe, 1, 1, f);
            s = DonjonSalle(d, x, y);
            if ((int)existe != SalleExiste(s)) {
                fclose(f);
                return 0;
            }
            if (!existe)
                continue;

            fread(&visitee, 1, 1, f);
            if (visitee)
                SalleVisiter(s);

            InventaireVider(SalleObjets(s));
            lire_inv(SalleObjets(s), f);

            fread(&has_e, 1, 1, f);
            if (has_e) {
                fread(nom,    NOM_MAX,     1, f);
                fread(&pv,    sizeof(int), 1, f);
                fread(&pvMax, sizeof(int), 1, f);
                fread(&atk,   sizeof(int), 1, f);
                fread(&def,   sizeof(int), 1, f);

                e = SalleEnnemi(s);
                if (e != NULL) {
                    EnnemiMajPV(e, pv);
                } else {
                    e = EnnemiCreer(nom, pvMax, atk, def);
                    if (e != NULL) {
                        EnnemiMajPV(e, pv);
                        SalleAjouterEnnemi(s, e);
                    }
                }
            } else {
                e = SalleEnnemi(s);
                if (e != NULL)
                    EnnemiMajPV(e, 0);
            }
        }
    }

    fclose(f);
    return 1;
}
