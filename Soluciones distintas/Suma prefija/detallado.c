#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define _(x,y) (x ? y[x-1] : 0)
#define S(i)   (i[out] = i[a] + _(i,out))

int main(void) {
    // Definir las 3 cantidades de datos a probar (solo las nuevas pruebas)
    int sizes[] = {100000, 10000000, 100000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\nPrueba con N = %d\n", n);

        // Asignación de memoria dinámica para soportar arreglos grandes
        int *a = (int *)malloc(n * sizeof(int));
        int *out = (int *)malloc(n * sizeof(int));

        if (a == NULL || out == NULL) {
            printf("Error asignando memoria para N = %d\n", n);
            return 1;
        }

        // Inicializar el arreglo con 1s para verificación rápida
        for (int i = 0; i < n; i++) {
            a[i] = 1;
        }

        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO (Uso de tareas OpenMP)
        // ---------------------------------------------------------
        #pragma omp parallel
        {
            #pragma omp single
            {
                for (int i = 0; i < n; ++i) {

                    if (i == 0) {
                        #pragma omp task depend(out: out[i])
                        {
                            int k = i;
                            S(k);
                        }
                    } else {
                        #pragma omp task depend(in: out[i-1]) \
                                         depend(out: out[i])
                        {
                            int k = i;
                            S(k);
                        }
                    }
                }

                #pragma omp taskwait
            }
        }
        // ---------------------------------------------------------
        // FIN DEL NÚCLEO
        // ---------------------------------------------------------

        double end = omp_get_wtime();
        double time_spent = end - start;

        // Imprimir resultados
        printf("Tiempo paralelo (OpenMP Tasks): %f segundos\n", time_spent);
        printf("Verificacion: out[%d] = %d (Esperado: %d)\n", n - 1, out[n - 1], n);

        // Liberar memoria para la siguiente iteración
        free(a);
        free(out);
    }

    return 0;
}