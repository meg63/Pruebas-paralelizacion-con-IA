
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <omp.h>

void _(char *s, char *o)
{
    size_t n = 0;

    while (s[n] != '\0')
        n++;

    if (n == 0) {
        o[0] = '\0';
        return;
    }

    /*
     * start[i] = 1 si en i comienza un nuevo segmento.
     */
    int *start = malloc(n * sizeof(int));

    /*
     * group[i] = identificador del segmento al que pertenece i.
     */
    size_t *group = malloc(n * sizeof(size_t));

    if (start == NULL || group == NULL) {
        free(start);
        free(group);
        o[0] = '\0';
        return;
    }

    /*
     * Cada posición se analiza independientemente en paralelo.
     */
    #pragma omp parallel for
    for (size_t i = 0; i < n; i++) {
        if (i == 0 || s[i] != s[i - 1])
            start[i] = 1;
        else
            start[i] = 0;
    }

    /*
     * Asignación secuencial de identificadores de segmento.
     */
    size_t groups = 0;

    for (size_t i = 0; i < n; i++) {
        if (start[i])
            groups++;

        group[i] = groups - 1;
    }

    /*
     * Posición inicial y longitud de cada segmento.
     */
    size_t *begin = malloc(groups * sizeof(size_t));
    size_t *length = malloc(groups * sizeof(size_t));
    size_t *offset = malloc(groups * sizeof(size_t));

    if (begin == NULL || length == NULL || offset == NULL) {
        free(start);
        free(group);
        free(begin);
        free(length);
        free(offset);
        o[0] = '\0';
        return;
    }

    for (size_t i = 0; i < n; i++) {
        if (start[i])
            begin[group[i]] = i;
    }

    for (size_t g = 0; g < groups; g++) {
        if (g + 1 < groups)
            length[g] = begin[g + 1] - begin[g];
        else
            length[g] = n - begin[g];
    }

    /*
     * Calcula el tamaño necesario para cada segmento.
     */
    size_t *size = malloc(groups * sizeof(size_t));

    if (size == NULL) {
        free(start);
        free(group);
        free(begin);
        free(length);
        free(offset);
        o[0] = '\0';
        return;
    }

    #pragma omp parallel for
    for (size_t g = 0; g < groups; g++) {
        size[g] = (size_t)snprintf(
            NULL,
            0,
            "%c%zu",
            s[begin[g]],
            length[g]
        );
    }

    /*
     * Prefix sum para determinar dónde escribe cada hilo.
     */
    size_t total = 0;

    for (size_t g = 0; g < groups; g++) {
        offset[g] = total;
        total += size[g];
    }

    /*
     * Los hilos escriben en regiones distintas de la salida.
     */
    #pragma omp parallel for
    for (size_t g = 0; g < groups; g++) {
        sprintf(
            o + offset[g],
            "%c%zu",
            s[begin[g]],
            length[g]
        );
    }

    o[total] = '\0';

    free(start);
    free(group);
    free(begin);
    free(length);
    free(offset);
    free(size);
}

/*
 * MISMA FUNCIÓN GENERADORA DEL CÓDIGO SECUENCIAL.
 */
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
    /*
     * MISMOS TAMAÑOS DE PRUEBA DEL CÓDIGO SECUENCIAL.
     */
    int sizes[] = { 100, 10000000, 200000000, 400000000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    /*
     * MISMA SEMILLA PARA GENERAR LA ENTRADA.
     */
    srand(42);

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int len = sizes[s_idx];

        printf("\n--- Prueba con cadena de %d caracteres ---\n", len);

        /*
         * MISMA GENERACIÓN DE ENTRADA Y RESERVA DE SALIDA.
         */
        char *t = generar_cadena_prueba(len);
        char *o = (char*)malloc((len * 3 + 1) * sizeof(char));

        if (!t || !o) {
            free(t);
            free(o);
            return 1;
        }

        /*
         * MISMA MÉTRICA DE TIEMPO QUE EL SECUENCIAL.
         * Solo se mide la ejecución del algoritmo paralelo.
         */
        double start = omp_get_wtime();

        _(t, o);

        double end = omp_get_wtime();

        /*
         * MISMA PRESENTACIÓN DE RESULTADOS.
         */
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
