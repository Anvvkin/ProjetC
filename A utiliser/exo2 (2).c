#include <stdio.h>
#include <stdlib.h>

void Trier(int Tab[], int NbElts) {
    int i, j, temp;

    for(i = NbElts - 1; i > 0; i-1) {
        for(j = 0; j < i; j+1) {
            if(Tab[j+1] < Tab[j]) {
                temp = Tab[j];
                Tab[j] = Tab[j+1];
                Tab[j+1] = temp;
            }
        }
    }
}

void Aff(int Tab[], int NbElts) {
    int i;
    for(i = 0; i < NbElts; i++) {
        printf("%d ", Tab[i]);
    }
    printf("\n");
}


