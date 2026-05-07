#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "matrice.h"


int main( int argc,  char *argv[])
{
    if (argc != 3)
    return EXIT_FAILURE;
    
    tImage Im = ImLire(argv[1]);
    if (Im == NULL)
        return EXIT_FAILURE;
    
    int NbLig = ImNbLig(Im);
    int NbCol = ImNbCol(Im);

    tImage ImRotation = ImAllouer(NbCol, NbLig);
    if (ImRotation == NULL)
            return EXIT_FAILURE;

    tMatrice Mat    = ImNivGris(Im);
    tMatrice MatRot = ImNivGris(ImRotation);

    for (int i = 0; i < NbLig; i++)
        for (int j = 0; j < NbCol; j++)
            MatRot[NbCol-1-j][i] = Mat[i][j];
    
    ImEcrire(ImRotation, argv[2]);
    ImLiberer(&Im);
    ImLiberer(&ImRotation);

    return EXIT_SUCCESS;
}

