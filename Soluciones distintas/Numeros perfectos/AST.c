#include <stdio.h>
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

        // Declaramos las variables requeridas por tu algoritmo
        int i, j, s;
        int count = 0;
        int mayor_encontrado = 0;

        if (r[1] <= 100000) {
            printf("out = [ ");
        }

        double start = omp_get_wtime();

        // ---------------------------------------------------------
        // NÚCLEO DEL ALGORITMO INTACTO
        // ---------------------------------------------------------
        #pragma omp parallel for ordered private(j, s)
        for(i=r[0]; i<=r[1]; i++) {
            s = 1;

            if(i > 1) {
                _(i)
                    s += O(i,j);

                if(s == i) {
                    // El bloque ordered asegura que entran en estricto orden
                    #pragma omp ordered
                    {
                        if (r[1] <= 100000) {
                            // Imprimir directamente solo en la prueba pequeña
                            printf("%d ", i);
                        }
                        
                        // Registro silencioso para las pruebas de estrés
                        count++;
                        mayor_encontrado = i; // Como es 'ordered', el último siempre será el mayor
                    }
                }
            }
        }
        // ---------------------------------------------------------
        // FIN DEL NÚCLEO
        // ---------------------------------------------------------

        double end = omp_get_wtime();
        double time_spent = end - start;

        // --- SALIDA ---
        if (r[1] <= 100000) {
            puts("]");
        } else {
            printf("Numeros perfectos encontrados: %d (Mayor: %d)\n", count, mayor_encontrado);
        }
        
        printf("Tiempo paralelo (OpenMP Ordered): %f segundos\n", time_spent);
    }

    return 0;
}