#include <stdio.h>
#include <omp.h>

int main(void) {
    int a[] = {3, 1, 7, 0, 4, 1, 6, 3};
    int out[8];
    int n = 8;

    /* Copia inicial */
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        out[i] = a[i];
    }

    /*
     * Prefix sum paralelo.
     * En cada iteración se duplica la distancia
     * de los elementos que se suman.
     */
    for (int d = 1; d < n; d *= 2) {

        #pragma omp parallel for
        for (int i = d; i < n; i++) {
            out[i] += out[i - d];
        }

        #pragma omp barrier
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", out[i]);
    }

    putchar('\n');

    return 0;
}
