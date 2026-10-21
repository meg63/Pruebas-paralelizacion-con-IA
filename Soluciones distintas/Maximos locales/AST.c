#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// ---------------------------------------------------------
// ALGORITMO PARALELO INTACTO (Variante OMP CRITICAL)
// ---------------------------------------------------------
void obtener_maximos_locales_critico(int a[], int n, int out_idx[], int out_val[], int *total_maximos) {
    int i, k = 0;

    #pragma omp parallel for
    for(i = 0; i < n; i++) {
        // Lógica intacta con operadores ternarios y AND bit a bit (&)
        int mayor = (i ? a[i]>a[i-1] : 1) &
                    (i+1<n ? a[i]>a[i+1] : 1);

        if(mayor) {
            // Se mantiene la sección crítica para guardar los resultados
            #pragma omp critical
            {
                out_idx[k] = i;
                out_val[k] = a[i];
                k++;
            }
        }
    }
    
    *total_maximos = k;
}
// ---------------------------------------------------------

// Función auxiliar para generar arreglos con valores aleatorios
int* generar_arreglo_prueba(int n) {
    int *arreglo = (int*)malloc(n * sizeof(int));
    if (!arreglo) return NULL;
    
    for (int i = 0; i < n; i++) {
        arreglo[i] = rand() % 100;
    }
    return arreglo;
}

int main() {
    int sizes[] = {
        100,         // Pequeña
        10000000,    // Mediana (~40 MB)
        200000000    // Grande (~800 MB)
    };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Semilla fija para asegurar que todos los algoritmos evalúen exactamente los mismos datos
    srand(42);

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int n = sizes[s_idx];
        printf("\n--- Prueba con arreglo de %d elementos ---\n", n);

        int *a = generar_arreglo_prueba(n);
        
        int max_posibles = (n / 2) + 1;
        int *out_idx = (int*)malloc(max_posibles * sizeof(int));
        int *out_val = (int*)malloc(max_posibles * sizeof(int));
        int total_encontrados = 0;

        if (!a || !out_idx || !out_val) {
            printf("Error de memoria.\n");
            return 1;
        }

        // Medición de tiempo
        double start = omp_get_wtime();
        
        obtener_maximos_locales_critico(a, n, out_idx, out_val, &total_encontrados);
        
        double end = omp_get_wtime();
        double time_spent = end - start;

        // Impresión de resultados
        if (n <= 100) {
            printf("Entrada a[] = [ ");
            for(int i=0; i<n; i++) printf("%d%s", a[i], (i<n-1)?", ":"");
            printf(" ]\n");

            // Nota: En esta versión, los resultados aparecerán en desorden debido a omp critical
            printf("Maximos locales (%d encontrados): [ ", total_encontrados);
            for(int i=0; i<total_encontrados; i++) {
                printf("[%d, %d]%s", out_idx[i], out_val[i], (i<total_encontrados-1)?", ":"");
            }
            printf(" ]\n");
        } else {
            // Para arreglos grandes, solo mostramos el conteo
            printf("Maximos locales encontrados: %d\n", total_encontrados);
        }
        
        printf("Tiempo paralelo (Seccion Critical): %f segundos\n", time_spent);

        free(a);
        free(out_idx);
        free(out_val);
    }

    return 0;
}