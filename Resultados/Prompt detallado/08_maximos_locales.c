#include <stdio.h>
#include <omp.h>

int main() {
    int a[] = {1,3,2,5,4,8,6,7,2};
    int n = sizeof(a) / sizeof(a[0]);

    int resultado[n];
    int cantidad = 0;

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int izquierda = (i == 0) || (a[i] > a[i - 1]);
        int derecha   = (i == n - 1) || (a[i] > a[i + 1]);

        resultado[i] = izquierda && derecha;
    }

    printf("out = [ ");

    for (int i = 0; i < n; i++) {
        if (resultado[i]) {
            if (cantidad++)
                printf(", ");

            printf("[%d, %d]", i, a[i]);
        }
    }

    puts(" ]");

    return 0;
}
