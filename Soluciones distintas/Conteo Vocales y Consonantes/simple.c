
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <omp.h>

typedef struct {
    const char *s;
    int v, c;
} R;

#define V(k) (1 << ((*k | 32) - 'a') & 0x104111)

// Generar cadenas aleatorias, igual que en el secuencial
const char** generar_cadenas_prueba(int n) {
    const char **a = (const char **)malloc(n * sizeof(char*));
    if (!a) return NULL;

    for (int i = 0; i < n; i++) {
        int len = 5 + (rand() % 10);
        char *str = (char *)malloc((len + 1) * sizeof(char));

        if (!str) {
            for (int j = 0; j < i; j++)
                free((void*)a[j]);
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

// Liberar las cadenas generadas
void liberar_cadenas(const char **a, int n) {
    for (int i = 0; i < n; i++)
        free((void*)a[i]);

    free(a);
}

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
        R *r = (R *)malloc(n * sizeof(R));

        if (!a || !r) {
            printf("Error de memoria.\n");
            if (a)
                liberar_cadenas(a, n);
            free(r);
            return 1;
        }

        // Medición del tiempo de procesamiento paralelo
        double start = omp_get_wtime();

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            r[i].s = a[i];
            r[i].v = r[i].c = 0;

            for (const char *p = a[i]; *p; p++)
                isalpha((unsigned char)*p)
                    ? V(p) ? r[i].v++ : r[i].c++
                    : 0;
        }

        double end = omp_get_wtime();
        double time_spent = end - start;

        // Mostrar resultados con el mismo formato del secuencial
        if (n <= 100) {
            printf("out = [\n");

            for (int i = 0; i < n; i++)
                printf("  [\"%s\", %d, %d]%s\n",
                       r[i].s, r[i].v, r[i].c,
                       (i < n - 1) ? "," : "");

            printf("]\n");
        } else {
            printf("Procesamiento completado. Resumen:\n");
            printf("  Primera -> [\"%s\", V:%d, C:%d]\n",
                   r[0].s, r[0].v, r[0].c);
            printf("  Ultima  -> [\"%s\", V:%d, C:%d]\n",
                   r[n - 1].s, r[n - 1].v, r[n - 1].c);
        }

        printf("Tiempo paralelo: %f segundos\n", time_spent);

        // Liberar memoria
        liberar_cadenas(a, n);
        free(r);
    }

    return 0;
}
