#include <stdio.h>
#include <omp.h>

int main() {
    int a[] = {1, 3, 2, 5, 4, 8, 6, 7, 2};
    int n = sizeof(a) / sizeof(a[0]);

    // Guarda qué posiciones son máximos locales
    int local_max[n];

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int izquierda = (i == 0) || (a[i] > a[i - 1]);
        int derecha   = (i == n - 1) || (a[i] > a[i + 1]);

        local_max[i] = izquierda && derecha;
    }

    // La impresión se mantiene secuencial para conservar el orden
    printf("out = [ ");

    int k = 0;
    for (int i = 0; i < n; i++) {
        if (local_max[i]) {
            printf("%s[%d, %d]", k++ ? ", " : "", i, a[i]);
        }
    }

    puts(" ]");

    return 0;
}
