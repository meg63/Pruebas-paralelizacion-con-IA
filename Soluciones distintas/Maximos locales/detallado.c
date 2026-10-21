#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// ---------------------------------------------------------
// ALGORITMO PARALELO INTACTO (Variante resultado/cantidad)
// ---------------------------------------------------------
void obtener_maximos_locales_paralelo_2(int a[], int n, int out_idx[], int out_val[], int *total_maximos) {
    // Se cambia VLA (int resultado[n]) por malloc para soportar arreglos gigantes
    int *resultado = (int*)malloc(n * sizeof(int));
    if (!resultado) return;

    int cantidad = 0;

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int izquierda = (i == 0) || (a[i] > a[i - 1]);
        int derecha   = (i == n - 1) || (a[i] > a[i + 1]);

        resultado[i] = izquierda && derecha;
    }

    // La recolección reemplaza a los printf, manteniendo la misma estructura
    for (int i = 0; i < n; i++) {
        if (resultado[i]) {
            out_idx[cantidad] = i;
            out_val[cantidad] = a[i];
            cantidad++;
        }
    }
    
    *total_maximos = cantidad;
    free(resultado);
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
        
        obtener_maximos_locales_paralelo_2(a, n, out_idx, out_val, &total_encontrados);
        
        double end = omp_get_wtime();
        double time_spent = end - start;

        // Impresión de resultados
        if (n <= 100) {
            printf("Entrada a[] = [ ");
            for(int i=0; i<n; i++) printf("%d%s", a[i], (i<n-1)?", ":"");
            printf(" ]\n");

            printf("Maximos locales (%d encontrados): [ ", total_encontrados);
            for(int i=0; i<total_encontrados; i++) {
                printf("[%d, %d]%s", out_idx[i], out_val[i], (i<total_encontrados-1)?", ":"");
            }
            printf(" ]\n");
        } else {
            // Para arreglos grandes, solo mostramos el conteo
            printf("Maximos locales encontrados: %d\n", total_encontrados);
        }
        
        printf("Tiempo paralelo (Variante resultado): %f segundos\n", time_spent);

        free(a);
        free(out_idx);
        free(out_val);
    }

    return 0;
}