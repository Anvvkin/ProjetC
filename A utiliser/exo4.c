#include <stdio.h>
#include <math.h>

int main(){

    printf("Saisissez un nombre : ");
    double a;
    scanf("%lf", &a);

    printf("La racine carré de votre nombre est : %lf\n", sqrt(a));

    return 0;


}