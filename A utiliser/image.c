#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "matrice.h"

struct sImage
{
    int NbLig;
    int NbCol;
    tMatrice NivGris;
};

tImage ImAllouer(int NbLig, int NbCol)
{
    tImage Im = (tImage)malloc(sizeof(struct sImage));
    if (Im == NULL)
        return NULL;

    Im->NivGris = MatAllouer(NbLig, NbCol);
    if (Im->NivGris == NULL)
    {
        free(Im);
        return NULL;
    }

    Im->NbLig = NbLig;
    Im->NbCol = NbCol;
    return Im;
}

void ImLiberer(tImage *pIm)
{
    if (pIm == NULL || *pIm == NULL)
        return;

    MatLiberer(&(*pIm)->NivGris);
    free(*pIm);
    *pIm = NULL;
}

int ImNbLig(tImage Im)
{
    return Im->NbLig;
}

int ImNbCol(tImage Im)
{
    return Im->NbCol;
}

tMatrice ImNivGris(tImage Im)
{
    return Im->NivGris;
}

tImage ImLire(char NomFichier[])
{
    FILE *f = fopen(NomFichier, "r");
    if (f == NULL)
        return NULL;

    char sig[10];
    int NbCol, NbLig, MaxGris;

    fscanf(f, "%s", sig);
    if (sig[0] != 'P' || sig[1] != '2')
    {
        fclose(f);
        return NULL;
    }

    fscanf(f, "%d %d %d", &NbCol, &NbLig, &MaxGris);

    tImage Im = ImAllouer(NbLig, NbCol);
    if (Im == NULL)
    {
        fclose(f);
        return NULL;
    }

    tMatrice Mat = ImNivGris(Im);
    for (int i = 0; i < NbLig; i++)
        for (int j = 0; j < NbCol; j++)
        {
            int val;
            fscanf(f, "%d", &val);
            Mat[i][j] = (unsigned char)round(255.0 * val / MaxGris);
        }

    fclose(f);
    return Im;
}

void ImEcrire(tImage Im, char NomFichier[])
{
    FILE *f = fopen(NomFichier, "w");
    if (f == NULL)
        return;

    int NbLig = ImNbLig(Im);
    int NbCol = ImNbCol(Im);
    tMatrice Mat = ImNivGris(Im);

    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", NbCol, NbLig);
    fprintf(f, "255\n");

    for (int i = 0; i < NbLig; i++)
    {
        for (int j = 0; j < NbCol; j++)
        {
            if (j > 0)
                fprintf(f, " ");
            fprintf(f, "%d", (int)Mat[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}
