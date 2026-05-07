#include <stdio.h>

void AfficherTab(int Tab[], int NbElt)
{
    for(int i = 0; i < NbElt; i++)
    {
        printf("%d ", Tab[i]);
    }
    printf("\n");
}

int main()
{
    int T[5] = {1, 2, 3, 4, 5};

    AfficherTab(T, 5);

    return 0;
}