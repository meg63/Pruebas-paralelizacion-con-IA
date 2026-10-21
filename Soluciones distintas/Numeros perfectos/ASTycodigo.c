#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define _(x) for(j=2;j*j<=x;j++)
#define O(a,b) (a%b?0:b+(b*b!=a?a/b:0))

int main()
{
    // Definimos las 3 cantidades (rangos) de datos a probar
    int ranges[3][2] = {
        {2, 1000},      // Pequeña (Prueba original)
        {2, 50000},     // Mediana
        {2, 500000}     // Grande
    };
    int num_ranges = sizeof(ranges) / sizeof(ranges[0]);

    for (int idx = 0; idx < num_ranges; idx++) {
        int *r = ranges[idx];
        printf("\n--- Prueba con rango [%d, %d] ---\n", r[0], r[1]);

        int i, j, s;
        
        // Se usa calloc para inicializar en 0 y soportar arreglos muy grandes sin desbordar el stack
        int *found = (int*)calloc(r[1] + 1, sizeof(int));
        if (found == NULL) {
            printf("Error de memoria.\n");
            return 1;
        }

        if (r[1] <= 100000) {
            printf("out = [ ");
        }

        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO
        // ---------------------------------------------------------
        #pragma omp parallel for private(j, s) shared(r, found)
        for(i = r[0]; i <= r[1]; i++) {
            s = 1;

            if(i > 1) {
                _(i)
                    s += O(i,j);

                if(s == i)
                    found[i] = 1;
            }
        }
        // ---------------------------------------------------------
        // FIN DEL NÚCLEO PARALELO
        // ---------------------------------------------------------

        double end = omp_get_wtime();
        double time_spent = end - start;

        // --- SALIDA (Basada en tu bucle secuencial original) ---
        int count = 0;
        int mayor_encontrado = 0;

        /* Se mantiene la salida en el mismo orden que el programa secuencial. */
        for(i = r[0]; i <= r[1]; i++) {
            if(found[i]) {
                if (r[1] <= 100000) {
                    printf("%d ", i);
                }
                count++;
                mayor_encontrado = i;
            }
        }

        if (r[1] <= 100000) {
            puts("]");
        } else {
            printf("Numeros perfectos encontrados: %d (Mayor: %d)\n", count, mayor_encontrado);
        }
        
        printf("Tiempo paralelo (OpenMP arreglo indexado): %f segundos\n", time_spent);

        free(found);
    }

    return 0;
}