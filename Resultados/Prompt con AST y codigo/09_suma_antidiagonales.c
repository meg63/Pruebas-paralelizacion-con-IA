#include <stdio.h>
#include <omp.h>

int main() {
    int M[] = {1,2,3,4,5,6,7,8,9};
    int O[5] = {0};
    int i = 0;

    #pragma omp parallel for
    for (i = 0; i < 9; i++) {
        #pragma omp atomic
        O[i/3 + i%3] += M[i];
    }

    printf("out = [ ");

    for (i = 0; i < 5; i++)
        printf(i < 4 ? "%d, " : "%d ]\n", O[i]);

    return 0;
}