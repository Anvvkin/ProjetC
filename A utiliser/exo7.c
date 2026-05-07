#include <stdio.h>


int main() {

    unsigned char a;

    printf("Saisissez un nombre : ");
    scanf("%hhu", &a);

    printf("Le carré de votre nombre est : %hhu\n", a * a);

    return 0;
}