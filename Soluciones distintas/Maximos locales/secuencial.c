#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// ---------------------------------------------------------
// ALGORITMO ADAPTADO PARA BENCHMARK (Sin printf en el ciclo)
// ---------------------------------------------------------
// Guardamos los resultados en arreglos pre-asignados para evitar el cuello de botella de I/O.
// 'out_idx' guarda la posición y 'out_val' guarda el valor.
void obtener_maximos_locales(int a[], int n, int out_idx[], int out_val[], int *total_maximos) {
    int count = 0;

    for (int i = 0; i < n; i++) {
        // La lógica de evaluación se mantiene idéntica
        int es_izq_valido = (i == 0) || (a[i] > a[i - 1]);
        int es_der_valido = (i == n - 1) || (a[i] > a[i + 1]);

        if (es_izq_valido && es_der_valido) {
            out_idx[count] = i;
            out_val[count] = a[i];
            count++;
        }
    }
    
    *total_maximos = count;
}
// ---------------------------------------------------------

// Función auxiliar para generar arreglos con valores aleatorios
int* generar_arreglo_prueba(int n) {
    int *arreglo = (int*)malloc(n * sizeof(int));
    if (!arreglo) return NULL;
    
    for (int i = 0; i < n; i++) {
        // Generamos números entre 0 y 99 para que sea fácil de leer en pruebas pequeñas
        arreglo[i] = rand() % 100;
    }
    return arreglo;
}

int main() {
    // Definimos las 3 cantidades de datos a probar
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
        
        // En el peor caso (ej. 1,0,1,0,1,0), la mitad de los elementos son máximos locales
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
        
        obtener_maximos_locales(a, n, out_idx, out_val, &total_encontrados);
        
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
            // Para arreglos grandes, solo mostramos el conteo para no saturar la pantalla
            printf("Maximos locales encontrados: %d\n", total_encontrados);
        }
        
        printf("Tiempo secuencial: %f segundos\n", time_spent);

        // Liberar memoria
        free(a);
        free(out_idx);
        free(out_val);
    }

    return 0;
}