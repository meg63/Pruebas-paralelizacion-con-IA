#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Pasamos las dimensiones como parámetros para usar arreglos de longitud variable (VLA) soportados desde C99
void buscarPuntosSilla(int filas, int columnas, int m[filas][columnas], int imprimir_salida) {
    if (imprimir_salida) printf("out = [ ");
    int primero = 1;
    int contador = 0; // Para contar hallazgos en pruebas grandes sin imprimir

    for (int i = 0; i < filas; i++) {
        
        int min_val = m[i][0];
        for (int j = 1; j < columnas; j++) {
            if (m[i][j] < min_val) {
                min_val = m[i][j];
            }
        }

        for (int j = 0; j < columnas; j++) {
            if (m[i][j] == min_val) {
                int es_max_col = 1;
                for (int k = 0; k < filas; k++) {
                    if (m[k][j] > min_val) {
                        es_max_col = 0;
                        break;
                    }
                }
                if (es_max_col) {
                    if (imprimir_salida) {
                        if (!primero) printf(", ");
                        printf("[%d, %d, %d]", min_val, i, j);
                        primero = 0;
                    }
                    contador++;
                }
            }
        }
    }
    
    if (imprimir_salida) {
        printf(" ]\n");
    } else {
        printf("Puntos silla encontrados: %d\n", contador);
    }
}

int main() {
    // Definimos las cantidades (NxN). El primer elemento (3) es para ejecutar la prueba original.
    int sizes[] = {3,  100,500,1000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\nPrueba con Matriz %d x %d\n", n, n);

        // Asignación de memoria dinámica contigua en el Heap usando punteros a arreglos
        int (*m)[n] = malloc(n * sizeof(*m));

        if (m == NULL) {
            printf("Error asignando memoria para %dx%d\n", n, n);
            return 1;
        }

        // --- ENTRADA ---
        if (n == 3) {
            // Prueba original
            int datos_originales[3][3] = {
                {8, 4, 1},
                {6, 9, 3},
                {7, 5, 2}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    m[i][j] = datos_originales[i][j];
                }
            }
        } else {
            // Rellenar matrices grandes (usamos valores basados en el índice para crear un comportamiento predecible)
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // Configuración básica: un punto silla garantizado o pocos por probabilidad
                    m[i][j] = (i * n) + j; 
                }
            }
        }

        // --- MEDICIÓN DE TIEMPO ---
        clock_t start = clock();
        
        // Ejecución del algoritmo intacto
        buscarPuntosSilla(n, n, m, n == 3);
        
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

        // --- SALIDA ---
        if (n > 3) {
            printf("Tiempo de ejecucion: %f segundos\n", time_spent);
        }

        // Liberar memoria para la siguiente iteración
        free(m);
    }

    return 0;
}