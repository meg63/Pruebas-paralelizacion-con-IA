#include <stdio.h>
#include <omp.h>

int main() {
    int a[] = {1,3,2,5,4,8,6,7,2};
    int n = sizeof(a) / sizeof(a[0]);

    /* marca[i] indica si a[i] cumple la condición */
    int marca[n];

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int mayor_izq = (i == 0)     || (a[i] > a[i-1]);
        int mayor_der = (i == n - 1) || (a[i] > a[i+1]);

        marca[i] = mayor_izq && mayor_der;
    }

    /* La salida se mantiene secuencial para conservar el orden */
    printf("out = [ ");

    int k = 0;
    for (int i = 0; i < n; i++) {
        if (marca[i]) {
            printf(", [%d, %d]" + !k++, i, a[i]);
        }
    }

    puts(" ]");

    return 0;
}
