#include <stdio.h>

int main() {

int n, i;
float valeurs[100];
float somme = 0, moyenne;

    printf("Entrez le nombre de valeurs : \n");
        scanf("%d", &n);

    if (n > 100 || n <=0){
        printf ("Nombre supérieur à 100. \n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Entrez la valeur %d : ", i + 1);
            scanf("%f", &valeurs[i]);
                somme += valeurs[i];
    }


    moyenne = somme / n;
        printf("La moyenne est : %.2f\n", moyenne);

    printf("Les valeurs superieures a la moyenne sont :\n");

        for (i = 0; i < n; i++) {
            if (valeurs[i] > moyenne) {
                printf("%.2f\n", valeurs[i]);
            }
        }
    return 0;
}