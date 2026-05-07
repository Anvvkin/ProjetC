#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "ti.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
        return EXIT_FAILURE;

    tImage Im = ImLire(argv[1]);
    if (Im == NULL)
        return EXIT_FAILURE;

    /* Rotation */
    tImage ImRot = Rotation(Im);
    if (ImRot == NULL)
    {
        ImLiberer(&Im);
        return EXIT_FAILURE;
    }
    ImEcrire(ImRot, "rotation.pgm");
    ImLiberer(&ImRot);

    /* Inversion */
    unsigned char TableInv[256];
    RemplirTableInversion(TableInv);
    tImage ImInv = AppliquerTable(TableInv, Im);
    if (ImInv == NULL)
    {
        ImLiberer(&Im);
        return EXIT_FAILURE;
    }
    ImEcrire(ImInv, "inversion.pgm");
    ImLiberer(&ImInv);

    /* Correction gamma */
    double Gamma = atof(argv[2]);
    unsigned char TableGamma[256];
    RemplirTableGamma(Gamma, TableGamma);
    tImage ImGamma = AppliquerTable(TableGamma, Im);
    if (ImGamma == NULL)
    {
        ImLiberer(&Im);
        return EXIT_FAILURE;
    }
    ImEcrire(ImGamma, "gamma.pgm");
    ImLiberer(&ImGamma);

    ImLiberer(&Im);
    return EXIT_SUCCESS;
}
