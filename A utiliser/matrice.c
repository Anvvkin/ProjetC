#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"


tMatrice MatAllouer(int NbLig, int NbCol) {
    if (NbLig <= 0 || NbCol <= 0)
        return NULL;

    tMatrice tab = (tMatrice) malloc(NbLig * sizeof(unsigned char *));
    if (tab == NULL)
        return NULL;

    unsigned char *Mat = (unsigned char *) malloc(NbLig * NbCol * sizeof(unsigned char));
    if (Mat == NULL) {
        free(tab);
        return NULL;
    }

    for (int i = 0; i < NbLig; i++)
        tab[i] = Mat + i * NbCol;

    return tab;
}

tMatrice MatLire(int *pNbLig, int *pNbCol) {
    printf("Entrez le nombre de lignes : ");
    scanf("%d", pNbLig);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", pNbCol);

    if (*pNbLig <= 0 || *pNbCol <= 0) {
        printf("Dimensions invalides.\n");
        return NULL;
    }

    tMatrice mat = MatAllouer(*pNbLig, *pNbCol);
    if (mat == NULL)
        return NULL;

    for (int i = 0; i < *pNbLig; i++) {
        for (int j = 0; j < *pNbCol; j++) {
            printf("mat[%d][%d] = ", i, j);
            scanf("%hhu", &mat[i][j]);
        }
    }

    return mat;
}

void MatAfficher(tMatrice Mat, int NbLig, int NbCol) {
    for (int i = 0; i < NbLig; i++) {
        for (int j = 0; j < NbCol; j++) {
            printf("%d ", Mat[i][j]);
        }
        printf("\n");
    }
}

tMatrice MatCopier(tMatrice Mat, int NbLig, int NbCol) {
    tMatrice copie = MatAllouer(NbLig, NbCol);
    if (copie == NULL)
        return NULL;

    for (int i = 0; i < NbLig; i++)
        for (int j = 0; j < NbCol; j++)
            copie[i][j] = Mat[i][j];

    return copie;
}

void MatLiberer(tMatrice *pMat) {
    if (pMat == NULL || *pMat == NULL)
        return;

    free((*pMat)[0]);
    free(*pMat);
    *pMat = NULL;
}
