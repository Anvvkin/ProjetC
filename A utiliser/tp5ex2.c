#include <stdio.h>
#include <stdlib.h>

double *CreerTableau(int NbElts) {
    double *tab = (double *)malloc(NbElts * sizeof(double));
    return tab; 
}

void SaisirTableau(double *Tab, int NbElts) {
    for (int i = 0; i < NbElts; i++) {
        printf("Entrez l'élément %d : ", i);
        scanf("%lf", &Tab[i]);
    }
}

void AfficherTableau(double *Tab, int NbElts) {
    printf("Contenu du tableau :\n");
    for (int i = 0; i < NbElts; i++) {
        printf("Tab[%d] = %.2f\n", i, Tab[i]);
    }
}


void LibererTableau(double *Tab) {
    free(Tab);
}

int main(void) {
    int n;
    double *monTableau;

    printf("Combien de valeurs souhaitez-vous saisir ? ");
    scanf("%d", &n);

    if (n <= 0) {
        fprintf(stderr, "Nombre invalide.\n");
        return EXIT_FAILURE;
    }

    monTableau = CreerTableau(n);
    if (monTableau == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return EXIT_FAILURE;
    }

    SaisirTableau(monTableau, n);
    AfficherTableau(monTableau, n);
    LibererTableau(monTableau);

    return EXIT_SUCCESS;
}