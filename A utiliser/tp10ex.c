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

        tImage ImRot = Rotation(Im);
        if (ImRot == NULL)
        {
            ImLiberer(&Im);
            return EXIT_FAILURE;
        }
        ImEcrire(ImRot, "rotation.pgm");
        ImEcrireBin(ImRot, "rotation-bin.pgm");
        ImLiberer(&ImRot);

        unsigned char TableInv[256];
        RemplirTableInversion(TableInv);
        tImage ImInv = AppliquerTable(TableInv, Im);
        if (ImInv == NULL)
        {
            ImLiberer(&Im);
            return EXIT_FAILURE;
        }
        ImEcrire(ImInv, "inversion.pgm");
        ImEcrireBin(ImInv, "inversion-bin.pgm");
        ImLiberer(&ImInv);

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
        ImEcrireBin(ImGamma, "gamma-bin.pgm");
        ImLiberer(&ImGamma);

        ImLiberer(&Im);
        return EXIT_SUCCESS;
    }
