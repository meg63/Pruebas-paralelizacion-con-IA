#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    // Tamaños de prueba: 3 (original), 1000, 5000, 15000
    int sizes[] = {3,  100,500,1000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\nPrueba con Matriz %d x %d\n", n, n);

        // Asignación de memoria dinámica para el arreglo 2D
        int (*M)[n] = malloc(n * n * sizeof(int));
        if (M == NULL) {
            printf("Error asignando memoria para %dx%d\n", n, n);
            return 1;
        }

        // --- ENTRADA ---
        if (n == 3) {
            int original[3][3] = {{8,4,1},{6,9,3},{7,5,2}};
            for (int r = 0; r < 3; r++)
                for (int c = 0; c < 3; c++)
                    M[r][c] = original[r][c];
        } else {
            for (int r = 0; r < n; r++)
                for (int c = 0; c < n; c++)
                    M[r][c] = (r * n) + c;
        }

        // Variables equivalentes a las globales del código original
        long long total_elements = (long long)n * n;
        long long i; 
        int j, k, f = 0;

        if (n == 3) putchar('[');

        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO
        // ---------------------------------------------------------
        for (i = 0; i < total_elements; i++) {

            // Bucle original conservado tal cual
            for (j = 0, k = 1; j < n; j++)
                ;

            k = 1;

            #pragma omp parallel for reduction(&:k)
            for (j = 0; j < n; j++)
                k &= *(*M + i/n*n + j) >= *(*M + i)
                  && *(*M + j*n + i%n) <= *(*M + i);

            if (k) {
                if (n == 3) {
                    printf("%s[%d, %d, %d]",
                           f++ ? ", " : "",
                           *(*M + i), (int)(i/n), (int)(i%n));
                } else {
                    f++; // Solo contamos para no saturar la salida en las matrices grandes
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
            puts(" ]");
        } else {
            printf("Puntos silla encontrados: %d\n", f);
            printf("Tiempo paralelo (OpenMP Reduction): %f segundos\n", time_spent);
        }

        free(M);
    }

    return 0;
}