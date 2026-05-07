#include <stdio.h>

int main() {

    int a, b, c, d, e;
    int max, min;

    printf("Saisissez 5 nombres entiers : ");
    scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);

    max = min = a;

    if (b > max) max = b;
    if (c > max) max = c;
    if (d > max) max = d;
    if (e > max) max = e;

    if (b < min) min = b;
    if (c < min) min = c;
    if (d < min) min = d;
    if (e < min) min = e;

    printf("Le nombre le plus grand est %d et le plus petit est %d.\n", max, min);

    return 0;
}
