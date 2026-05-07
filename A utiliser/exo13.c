#include <stdio.h>

int Max(int Tab[], int NbElt)
{
    int max = Tab[0];

    for(int i = 1; i < NbElt; i++)
    {
        if(Tab[i] > max)
        {
            max = Tab[i];
        }
    }

    return max;
}


int main(){

    int T[5] = {1,2,4,88,0};
    int resultat = Max(T, 5);

    printf("Max = %d\n", resultat);
    return 0;

}