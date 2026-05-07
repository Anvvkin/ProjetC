#include <stdio.h>

int main() {

    int n, i, j;
    double matrice[10][10];
    int symetrique = 1;

    printf("Ordre de la matrice : ");
    scanf("%d", &n);

    if (n > 10 || n <= 0) {
        printf("Ordre invalide.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Ligne %d : ", i + 1);
        for (j = 0; j < n; j++) {
            scanf("%lf", &matrice[i][j]);
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (matrice[i][j] != matrice[j][i]) {
                symetrique = 0;
            }
        }
    }

    if (symetrique == 1) {
        printf("Cette matrice est symetrique.\n");
    } else {
        printf("Cette matrice n'est pas symetrique.\n");
    }

    return 0;
}