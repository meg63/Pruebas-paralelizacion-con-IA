#include <stdio.h>

void obtener_maximos_locales(int a[], int n) {
    printf("out = [ ");
    int primero = 1;

    for (int i = 0; i < n; i++) {
        int es_izq_valido = (i == 0) || (a[i] > a[i - 1]);
        int es_der_valido = (i == n - 1) || (a[i] > a[i + 1]);

        if (es_izq_valido && es_der_valido) {
            if (!primero) {
                printf(", ");
            }
            printf("[%d, %d]", i, a[i]);
            primero = 0;
        }
    }
    printf(" ]\n");
}

int main() {
    int a[] = {1, 3, 2, 5, 4, 8, 6, 7, 2};
    int n = sizeof(a) / sizeof(a[0]);

    obtener_maximos_locales(a, n);

    return 0;
}