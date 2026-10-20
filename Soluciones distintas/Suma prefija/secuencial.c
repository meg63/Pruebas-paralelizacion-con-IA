#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void suma_prefija(const int a[], int out[], int n) {
    if (n <= 0) return;
    out[0] = a[0];
    for (int i = 1; i < n; i++) {
        out[i] = out[i - 1] + a[i];
    }
}

int main(void) {
    // Definir las 3 cantidades de datos a probar
    int sizes[] = {100000, 10000000, 100000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\nPrueba con N = %d\n", n);

        // Asignación de memoria dinámica en el Heap
        int *a = (int *)malloc(n * sizeof(int));
        int *out = (int *)malloc(n * sizeof(int));

        if (a == NULL || out == NULL) {
            printf("Error asignando memoria para N = %d\n", n);
            return 1;
        }

        // Inicializar el arreglo con 1s para verificar fácilmente (resultado esperado = n)
        for (int i = 0; i < n; i++) {
            a[i] = 1; 
        }

        // Medir el tiempo de ejecución secuencial
        clock_t start = clock();
        suma_prefija(a, out, n);
        clock_t end = clock();

        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

        // Imprimir resultados de rendimiento y verificación básica (no imprimir todo el arreglo)
        printf("Tiempo secuencial: %f segundos\n", time_spent);
        printf("Verificacion: out[%d] = %d (Esperado: %d)\n", n - 1, out[n - 1], n);

        // Liberar memoria
        free(a);
        free(out);
    }

    return 0;
}