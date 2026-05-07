#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"

void print_bonjour(void)
{
    printf("Bonjour\n");
}

int main(void)
{
    int NbLig, NbCol;

    tMatrice mat = MatLire(&NbLig, &NbCol);
    if (mat == NULL)
    {
        fprintf(stderr, "Erreur d'allocation\n");
        return EXIT_FAILURE;
    }

    printf("Matrice originale :\n");
    MatAfficher(mat, NbLig, NbCol);

    tMatrice copie = MatCopier(mat, NbLig, NbCol);
    if (copie == NULL)
    {
        fprintf(stderr, "Erreur de copie\n");
        MatLiberer(&mat);
        return EXIT_FAILURE;
    }

    printf("Matrice copiée :\n");
    MatAfficher(copie, NbLig, NbCol);

    MatLiberer(&mat);
    MatLiberer(&copie);

    return EXIT_SUCCESS;
}