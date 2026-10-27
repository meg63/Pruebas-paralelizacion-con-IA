#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void _(char *s, char *o)
{
    char *p = s;

    /* Contar grupos y localizar sus comienzos/finales */
    int grupos = 0;
    char *q = s;

    while (*q) {
        char *inicio = q;

        while (*q == *inicio)
            q++;

        grupos++;
    }

    /*
     * starts[g] = posición inicial del grupo
     * ends[g]   = posición final del grupo
     */
    char *starts[100];
    char *ends[100];

    q = s;
    int g = 0;

    while (*q) {
        starts[g] = q;

        char *inicio = q;
        while (*q == *inicio)
            q++;

        ends[g] = q;
        g++;
    }

    /*
     * Calcular dónde escribirá cada grupo.
     */
    char *dest[100];
    char *out = o;

    for (int i = 0; i < grupos; i++) {
        dest[i] = out;

        int n = (int)(ends[i] - starts[i]);
        int written = snprintf(out, 32, "%c%ld",
                               *starts[i], (long)n);

        out += written;
    }

    /*
     * Paralelización original: cada iteración escribe
     * en una zona distinta de la salida.
     */
    #pragma omp parallel for
    for (int i = 0; i < grupos; i++) {
        int n = (int)(ends[i] - starts[i]);

        sprintf(dest[i], "%c%ld",
                *starts[i], (long)n);
    }
}

/* Generación de cadenas igual que en el secuencial */
char* generar_cadena_prueba(int longitud)
{
    char *cadena = (char*)malloc((longitud + 1) * sizeof(char));
    if (!cadena) return NULL;

    int i = 0;

    while (i < longitud) {
        char c = 'a' + (rand() % 26);
        int repeticiones = 1 + (rand() % 20);

        for (int j = 0; j < repeticiones && i < longitud; j++) {
            cadena[i++] = c;
        }
    }

    cadena[longitud] = '\0';
    return cadena;
}

int main()
{
    int sizes[] = { 100, 10000000, 200000000, 400000000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    srand(42);

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int len = sizes[s_idx];

        printf("\n--- Prueba con cadena de %d caracteres ---\n", len);

        char *t = generar_cadena_prueba(len);
        char *o = (char*)malloc((len * 3LL + 1) * sizeof(char));

        if (!t || !o) {
            free(t);
            free(o);
            return 1;
        }

        double start = omp_get_wtime();

        _(t, o);

        double end = omp_get_wtime();

        if (len <= 100) {
            printf("Entrada: %s\nSalida:  %s\n", t, o);
        } else {
            printf("Salida truncada: %.30s ...\n", o);
        }

        printf("Tiempo Paralelo: %f segundos\n", end - start);

        free(t);
        free(o);
    }

    return 0;
}