#include <stdlib.h>
#include <math.h>
#include "ti.h"
#include "image.h"
#include "matrice.h"

tImage Rotation(tImage Im)
{
    int NbLig = ImNbLig(Im);
    int NbCol = ImNbCol(Im);

    tImage ImRot = ImAllouer(NbCol, NbLig);
    if (ImRot == NULL)
        return NULL;

    tMatrice Mat    = ImNivGris(Im);
    tMatrice MatRot = ImNivGris(ImRot);

    for (int i = 0; i < NbLig; i++)
        for (int j = 0; j < NbCol; j++)
            MatRot[NbCol - 1 - j][i] = Mat[i][j];

    return ImRot;
}

tImage AppliquerTable(unsigned char Table[256], tImage Im)
{
    int NbLig = ImNbLig(Im);
    int NbCol = ImNbCol(Im);

    tImage ImRes = ImAllouer(NbLig, NbCol);
    if (ImRes == NULL)
        return NULL;

    tMatrice Mat    = ImNivGris(Im);
    tMatrice MatRes = ImNivGris(ImRes);

    for (int i = 0; i < NbLig; i++)
        for (int j = 0; j < NbCol; j++)
            MatRes[i][j] = Table[Mat[i][j]];

    return ImRes;
}

void RemplirTableInversion(unsigned char Table[256])
{
    for (int g = 0; g < 256; g++)
        Table[g] = (unsigned char)(255 - g);
}

void RemplirTableGamma(double Gamma, unsigned char Table[256])
{
    for (int g = 0; g < 256; g++)
        Table[g] = (unsigned char)round(255.0 * pow(g / 255.0, Gamma));
}
