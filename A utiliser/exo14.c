#include <stdio.h>  

void MinMax(int Tab[], int NbElt, int *pMin, int *pMax)
{
    *pMin = Tab[0];
    *pMax = Tab[0];

    for(int i = 1; i < NbElt; i++)
    {
        if(Tab[i] < *pMin)
        {
            *pMin = Tab[i];
        }

        if(Tab[i] > *pMax)
        {
            *pMax = Tab[i];
        }
    }
}

int main()
{
    int T[5] = {1, 2, 4, 88, 0};

    int min, max;

    MinMax(T, 5, &min, &max);

    printf("Min = %d, Max = %d\n", min, max);

    return 0;
}   