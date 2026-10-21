#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// EL ALGORITMO SE MANTIENE INTACTO
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
    // Definimos las 3 cantidades (rangos) de datos a probar
    int ranges[3][2] = {
        {2, 1000},      // Pequeña (Prueba original)
        {2, 50000},     // Mediana
        {2, 500000}     // Grande
    };
    int num_ranges = sizeof(ranges) / sizeof(ranges[0]);

    for (int s = 0; s < num_ranges; s++) {
        int tamano = 0;
        printf("\n--- Prueba con rango [%d, %d] ---\n", ranges[s][0], ranges[s][1]);

        // Medir el tiempo de ejecución
        clock_t start = clock();
        
        int *out = obtener_numeros_perfectos(ranges[s], &tamano);
        
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

        // Imprimir el arreglo completo solo para la prueba pequeña
        if (ranges[s][1] <= 100000) {
            printf("out = [ ");
            for (int i = 0; i < tamano; i++) {
                printf("%d%s", out[i], (i == tamano - 1) ? "" : ", ");
            }
            printf(" ]\n");
        } else {
            // Para rangos grandes, solo mostramos la cantidad encontrada y el último valor
            printf("Numeros perfectos encontrados: %d (Ultimo: %d)\n", tamano, tamano > 0 ? out[tamano-1] : 0);
        }
        
        printf("Tiempo de ejecucion: %f segundos\n", time_spent);

        free(out);
    }

    return 0;
}