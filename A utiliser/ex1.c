#include <stdio.h>
#include <stdlib.h>

typedef unsigned char **tMatrice;

tMatrice MatAllouer(int NbLig, int NbCol) {
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



int main() {
    int NbLig, NbCol;

    tMatrice mat = MatLire(&NbLig, &NbCol);
    if (mat == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        return 0;
    }

    printf("Matrice originale :\n");
    MatAfficher(mat, NbLig, NbCol);

    tMatrice copie = MatCopier(mat, NbLig, NbCol);
    if (copie == NULL) {
        fprintf(stderr, "Erreur de copie\n");
        MatLiberer(&mat);
        return 0;
    }

    printf("Matrice copiée :\n");
    MatAfficher(copie, NbLig, NbCol);

    MatLiberer(&mat);
    MatLiberer(&copie);

    return 1;
}