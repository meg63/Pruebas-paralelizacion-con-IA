#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    // Tamaños de prueba: 3 (prueba original), 1000, 5000, 15000
    int sizes[] = {3,  100,500,1000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        long long n = sizes[s];
        long long total_elements = n * n;
        
        printf("\nPrueba con Matriz %lld x %lld\n", n, n);

        // Asignación de memoria dinámica para el arreglo principal y el de validación
        int (*M)[n] = malloc(total_elements * sizeof(int));
        int *valido = malloc(total_elements * sizeof(int));

        if (M == NULL || valido == NULL) {
            printf("Error asignando memoria para %lldx%lld\n", n, n);
            return 1;
        }

        // --- ENTRADA ---
        if (n == 3) {
            int original[3][3] = {
                {8,4,1},
                {6,9,3},
                {7,5,2}
            };
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    M[r][c] = original[r][c];
                }
            }
        } else {
            // Inicializar matrices grandes de forma predecible
            for (int r = 0; r < n; r++) {
                for (int c = 0; c < n; c++) {
                    M[r][c] = (r * n) + c;
                }
            }
        }

        // Variables equivalentes a las globales del programa original
        long long i;
        int f = 0;

        if (n == 3) putchar('[');

        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO
        // ---------------------------------------------------------
        #pragma omp parallel for
        for (i = 0; i < total_elements; i++) {
            int k_local = 1;

            // Se respeta la lógica de acceso mediante aritmética de punteros aplanada
            for (int j_local = 0; j_local < n; j_local++)
                k_local &= *(*M + i/n*n + j_local) >= *(*M + i)
                        && *(*M + j_local*n + i%n) <= *(*M + i);

            valido[i] = k_local;
        }

        /*
         * La salida permanece secuencial para conservar
         * el orden del programa original y evitar carreras
         * sobre f y stdout.
         */
        for (i = 0; i < total_elements; i++) {
            if (valido[i]) {
                if (n == 3) {
                    printf("%s[%d, %d, %d]",
                           f++ ? ", " : "",
                           *(*M + i),
                           (int)(i/n),
                           (int)(i%n));
                } else {
                    f++; // Solo contamos los hallazgos en matrices grandes para no saturar consola
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
            printf("Tiempo paralelo (Arreglo valido): %f segundos\n", time_spent);
        }

        // Liberar recursos de esta iteración
        free(M);
        free(valido);
    }

    return 0;
}