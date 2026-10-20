#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    // 3 para la prueba original, y las 3 dimensiones de carga propuestas
    int sizes[] = {3,  100,500,1000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\nPrueba con Matriz %d x %d\n", n, n);

        // Asignación de memoria dinámica (arreglo bidimensional contiguo)
        int (*M)[n] = malloc(n * sizeof(*M));
        if (M == NULL) {
            printf("Error asignando memoria para %dx%d\n", n, n);
            return 1;
        }

        // --- ENTRADA ---
        if (n == 3) {
            int original[3][3] = {
                {8, 4, 1},
                {6, 9, 3},
                {7, 5, 2}
            };
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    M[r][c] = original[r][c];
                }
            }
        } else {
            // Inicializar matrices grandes con datos predecibles
            for (int r = 0; r < n; r++) {
                for (int c = 0; c < n; c++) {
                    M[r][c] = (r * n) + c;
                }
            }
        }

        long long total_elements = (long long)n * n;
        int count = 0; // Para contar resultados en matrices grandes sin imprimir

        if (n == 3) printf("[");

        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO
        // ---------------------------------------------------------
        #pragma omp parallel for
        for (long long i = 0; i < total_elements; i++) {

            int j, k = 1;

            // La lógica de división (i / n) y módulo (i % n) se mantiene tal cual
            for (j = 0; j < n; j++)
                k &= M[i / n][j] >= M[i / n][i % n] &&
                     M[j][i % n] <= M[i / n][i % n];

            if (k) {
                #pragma omp critical
                {
                    if (n == 3) {
                        // Imprime solo si es la matriz pequeña de prueba
                        printf("[%d, %d, %d] ",
                               M[i / n][i % n],
                               (int)(i / n),
                               (int)(i % n));
                    }
                    count++;
                }
            }
        }
        // ---------------------------------------------------------
        // FIN DEL NÚCLEO
        // ---------------------------------------------------------

        double end = omp_get_wtime();
        double time_spent = end - start;

        // --- SALIDA ---
        if (n == 3) {
            printf("]\n");
        } else {
            printf("Puntos silla encontrados: %d\n", count);
            printf("Tiempo paralelo (OpenMP): %f segundos\n", time_spent);
        }

        // Liberar recursos
        free(M);
    }

    return 0;
}