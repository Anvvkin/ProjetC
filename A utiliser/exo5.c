#include <stdio.h>

int main() {

    int a, b, c;

    printf("Saisissez un premier entier : ");
    scanf("%d", &a);

    printf("Saisissez un deuxième entier : ");
    scanf("%d", &b);

    c = a;
    a = b;
    b = c;
    
    printf("a = %d\n", a);
    printf("b = %d\n", b);

    return 0;
}