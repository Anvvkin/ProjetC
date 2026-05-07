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

void MatLiberer(tMatrice *pMat) {
    if (pMat == NULL || *pMat == NULL)
        return;

    free((*pMat)[0]); 
    free(*pMat);      
    *pMat = NULL;    
}

tMatrice Triangle(int n) {
    int i, j;
    tMatrice A = MatAllouer(n, n);

    if (A == NULL) {
        printf("Erreur d'allocation\n");
        return NULL;
    }

    A[0][0] = 1;

    for(i = 1; i < n; i++) {
        A[i][0] = 1;
        A[i][i] = 1;

        for(j = 1; j < i; j++) {
            A[i][j] = A[i-1][j-1] + A[i-1][j];
        }
    }

    return A;
}

void AffTriangle(tMatrice A, int NbLig) {
    int i, j;

    for(i = 0; i < NbLig; i++) {
        for(j = 0; j <= i; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n;
    tMatrice T = NULL;

    printf("Entrez le nombre de lignes : ");
    scanf("%d", &n);

    T = Triangle(n);

    if (T != NULL) {
        printf("\nTriangle de Pascal :\n");
        AffTriangle(T, n);
    }

    MatLiberer(&T);

    return 0;
}