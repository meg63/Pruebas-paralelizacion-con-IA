#include <stdio.h>

int contar_pasos_collatz(long long n) {
    int pasos = 0;
    while (n > 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        pasos++;
    }
    return pasos;
}

void calcular_conteo_collatz(const int *a, int *out, int tamano) {
    for (int i = 0; i < tamano; i++) {
        out[i] = contar_pasos_collatz(a[i]);
    }
}

int main(void) {
    int a[] = {6, 7, 27, 15};
    int tamano = sizeof(a) / sizeof(a[0]);
    int out[4];

    calcular_conteo_collatz(a, out, tamano);

    printf("out = [ ");
    for (int i = 0; i < tamano; i++) {
        printf("%d%s", out[i], (i == tamano - 1) ? "" : ", ");
    }
    printf(" ]\n");

    return 0;
}