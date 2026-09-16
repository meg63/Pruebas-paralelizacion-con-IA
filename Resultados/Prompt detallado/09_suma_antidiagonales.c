#include <stdio.h>
#include <omp.h>

int main() {
    int M[] = {1,2,3,4,5,6,7,8,9};
    int O[5] = {0,0,0,0,0};

    #pragma omp parallel for
    for (int i = 0; i < 9; i++) {
        int j = i / 3 + i % 3;

        #pragma omp atomic
        O[j] += M[i];
    }

    printf("out = [ ");
    for (int i = 0; i < 5; i++)
        printf(i < 4 ? "%d, " : "%d ]\n", O[i]);

    return 0;
}
