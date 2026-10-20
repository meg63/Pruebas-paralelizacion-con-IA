#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    // Definir las 3 cantidades de datos a probar
    int sizes[] = {100000, 10000000, 100000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\nPrueba con N = %d\n", n);

        // Asignación de memoria dinámica para soportar arreglos grandes
        int *a = (int *)malloc(n * sizeof(int));
        int *out = (int *)malloc(n * sizeof(int));
        int *tmp = (int *)malloc(n * sizeof(int));

        if (a == NULL || out == NULL || tmp == NULL) {
            printf("Error asignando memoria para N = %d\n", n);
            return 1;
        }

        // Inicializar el arreglo con 1s para verificación rápida
        for (int i = 0; i < n; i++) {
            a[i] = 1;
        }

        // Usamos omp_get_wtime() en lugar de clock() porque es más preciso en entornos multihilo
        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO
        // ---------------------------------------------------------
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            out[i] = a[i];
        }

        for (int d = 1; d < n; d *= 2) {

            #pragma omp parallel for
            for (int i = 0; i < n; i++) {
                if (i >= d)
                    tmp[i] = out[i] + out[i - d];
                else
                    tmp[i] = out[i];
            }

            #pragma omp parallel for
            for (int i = 0; i < n; i++) {
                out[i] = tmp[i];
            }
        }
        // ---------------------------------------------------------
        // FIN DEL NÚCLEO
        // ---------------------------------------------------------

        double end = omp_get_wtime();
        double time_spent = end - start;

        // Imprimir resultados
        printf("Tiempo paralelo (OpenMP): %f segundos\n", time_spent);
        printf("Verificacion: out[%d] = %d (Esperado: %d)\n", n - 1, out[n - 1], n);

        // Liberar memoria para la siguiente iteración
        free(a);
        free(out);
        free(tmp);
    }

    return 0;
}