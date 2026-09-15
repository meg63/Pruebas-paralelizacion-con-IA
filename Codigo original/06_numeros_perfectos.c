#include <stdio.h>
#include <stdlib.h>

int* obtener_numeros_perfectos(int r[2], int *tamano_out) {
    int min = r[0];
    int max = r[1];
    int capacidad = 10;
    int *out = malloc(capacidad * sizeof(int));
    *tamano_out = 0;

    for (int n = min; n <= max; n++) {
        if (n <= 1) continue;
        
        int suma_divisores = 1; 
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                suma_divisores += i;
                if (i != n / i) {
                    suma_divisores += n / i; 
                }
            }
        }

        if (suma_divisores == n) {
            if (*tamano_out >= capacidad) {
                capacidad *= 2;
                out = realloc(out, capacidad * sizeof(int));
            }
            out[(*tamano_out)++] = n;
        }
    }

    return out;
}


int main() {
    int r[2] = {2, 100000};
    int tamano = 0;

    int *out = obtener_numeros_perfectos(r, &tamano);

    printf("out = [ ");
    for (int i = 0; i < tamano; i++) {
        printf("%d%s", out[i], (i == tamano - 1) ? "" : ", ");
    }
    printf(" ]\n");

    free(out);
    return 0;
}