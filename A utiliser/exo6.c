#include <stdio.h>

int main() {
    printf("Saisissez une température en Farenheit: ");
    int a, b;
    scanf("%d", &a);

    printf("La température en Celsius est : %d\n", b = ((5*(a-32))/9));

    return 0;
}