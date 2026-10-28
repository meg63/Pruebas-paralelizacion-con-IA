
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <omp.h>

// ---------------------------------------------------------
// ESTRUCTURA DE RESULTADOS
// ---------------------------------------------------------
typedef struct {
    const char *cadena;
    int vocales;
    int consonantes;
} Conteo;

#define V(k) (1 << ((*k | 32) - 'a') & 0x104111)

// ---------------------------------------------------------
// FUNCIÓN AUXILIAR PARA GENERAR CADENAS ALEATORIAS
// ---------------------------------------------------------
const char** generar_cadenas_prueba(int n) {
    const char **a = (const char **)malloc(n * sizeof(char*));
    if (!a) return NULL;

    for (int i = 0; i < n; i++) {
        int len = 5 + (rand() % 10);
        char *str = (char *)malloc((len + 1) * sizeof(char));

        if (!str) {
            for (int k = 0; k < i; k++) {
                free((void*)a[k]);
            }
            free(a);
            return NULL;
        }

        for (int j = 0; j < len; j++) {
            int mayuscula = rand() % 2;
            str[j] = mayuscula
                ? 'A' + (rand() % 26)
                : 'a' + (rand() % 26);
        }

        str[len] = '\0';
        a[i] = str;
    }

    return a;
}

// ---------------------------------------------------------
// FUNCIÓN AUXILIAR PARA LIBERAR MEMORIA
// ---------------------------------------------------------
void liberar_cadenas(const char **a, int n) {
    for (int i = 0; i < n; i++) {
        free((void*)a[i]);
    }
    free(a);
}

// ---------------------------------------------------------
// ALGORITMO PARALELO: SE CONSERVA EL ENFOQUE ORIGINAL
// ---------------------------------------------------------
void procesar_cadenas_paralelo(const char *a[], int n, Conteo out[]) {

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        const char *p = a[i];

        out[i].cadena = a[i];
        out[i].vocales = 0;
        out[i].consonantes = 0;

        for (; *p; p++) {
            isalpha((unsigned char)*p)
                ? V(p)
                    ? out[i].vocales++
                    : out[i].consonantes++
                : 0;
        }
    }
}

// ---------------------------------------------------------
// MAIN: ENTRADAS Y SALIDAS IGUALES AL SECUENCIAL
// ---------------------------------------------------------
int main(void) {
    int sizes[] = {
        1000000,      // Pequeña
        10000000,     // Mediana
        100000000     // Grande
    };

    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Misma semilla que el código secuencial
    srand(42);

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int n = sizes[s_idx];

        printf("\n--- Prueba con arreglo de %d cadenas ---\n", n);

        const char **a = generar_cadenas_prueba(n);
        Conteo *out = (Conteo *)malloc(n * sizeof(Conteo));

        if (!a || !out) {
            printf("Error de memoria.\n");

            if (a) {
                liberar_cadenas(a, n);
            }

            free(out);
            return 1;
        }

        // Medición del tiempo de procesamiento paralelo
        double start = omp_get_wtime();

        procesar_cadenas_paralelo(a, n, out);

        double end = omp_get_wtime();
        double time_spent = end - start;

        // Mostrar resultados con el mismo formato del secuencial
        if (n <= 100) {
            printf("out = [\n");

            for (int i = 0; i < n; i++) {
                printf("  [\"%s\", %d, %d]%s\n",
                       out[i].cadena,
                       out[i].vocales,
                       out[i].consonantes,
                       (i < n - 1) ? "," : "");
            }

            printf("]\n");
        } else {
            printf("Procesamiento completado. Resumen:\n");

            printf("  Primera -> [\"%s\", V:%d, C:%d]\n",
                   out[0].cadena,
                   out[0].vocales,
                   out[0].consonantes);

            printf("  Ultima  -> [\"%s\", V:%d, C:%d]\n",
                   out[n - 1].cadena,
                   out[n - 1].vocales,
                   out[n - 1].consonantes);
        }

        printf("Tiempo paralelo: %f segundos\n", time_spent);

        // Liberar memoria
        liberar_cadenas(a, n);
        free(out);
    }

    return 0;
}
