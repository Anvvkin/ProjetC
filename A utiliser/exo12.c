#include <stdio.h>

int Min(int Tab[], int NbElt)
{
    int min = Tab[0];

    for(int i = 1; i < NbElt; i++)
    {
        if(Tab[i] < min)
        {
            min = Tab[i];
        }
    }

    return min;
}


int main(){

    int T[5] = {1,2,4,88,0};
    int resultat = Min(T, 5);

    printf("Min = %d\n", resultat);
    return 0;

}