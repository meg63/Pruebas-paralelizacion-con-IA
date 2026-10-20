#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define _(x,y) (x ? y[x-1] : 0)
#define S(i)   (i[out] = i[a] + _(i,out))

int main(void) {
    // Definir las 3 cantidades de datos a probar
    int sizes[] = {100000, 10000000, 100000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\nPrueba con N = %d\n", n);

        // Asignación de memoria dinámica para los arreglos
        int *a = (int *)malloc(n * sizeof(int));
        int *out = (int *)malloc(n * sizeof(int));
        int nthreads, *suma;

        if (a == NULL || out == NULL) {
            printf("Error asignando memoria para N = %d\n", n);
            return 1;
        }

        // Inicializar el arreglo de entrada con 1s
        for (int i = 0; i < n; i++) {
            a[i] = 1;
        }

        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO
        // ---------------------------------------------------------
        #pragma omp parallel
        {
            #pragma omp single
            {
                nthreads = omp_get_num_threads();
                suma = calloc(nthreads, sizeof(int));
            }

            int tid = omp_get_thread_num();
            int inicio = tid * n / nthreads;
            int fin    = (tid + 1) * n / nthreads;

            /* Cada hilo calcula su bloque.
               La dependencia out[i-1] se mantiene dentro del bloque. */
            for (int i = inicio; i < fin; ++i) {
                out[i] = a[i] + (i ? out[i - 1] : 0);
            }

            /* Suma final del bloque */
            if (fin > inicio)
                suma[tid] = out[fin - 1];

            #pragma omp barrier

            /*
             * Se obtiene el desplazamiento correspondiente
             * a los bloques anteriores.
             */
            int offset = 0;
            for (int t = 0; t < tid; ++t)
                offset += suma[t];

            /* Se corrige el bloque con el prefijo anterior. */
            if (offset != 0) {
                for (int i = inicio; i < fin; ++i)
                    out[i] += offset;
            }
        }
        // ---------------------------------------------------------
        // FIN DEL NÚCLEO
        // ---------------------------------------------------------

        double end = omp_get_wtime();
        double time_spent = end - start;

        // Imprimir resultados
        printf("Tiempo paralelo (OpenMP Bloques): %f segundos\n", time_spent);
        
        // Verificación básica del último elemento
        // Nota: Debido a la lógica interna "i ? out[i-1] : 0" en los límites de los bloques,
        // el resultado de la verificación dependerá de cómo se resolvieron las condiciones de carrera.
        printf("Verificacion: out[%d] = %d (Esperado: %d)\n", n - 1, out[n - 1], n);

        // Liberar recursos
        free(a);
        free(out);
        free(suma);
    }

    return 0;
}