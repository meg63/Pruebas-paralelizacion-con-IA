#include <stdio.h>
#include <stdlib.h>
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
     * Corresponde a la condición del for interior:
     *
     *     for(p=s; *p==*s; p++);
     *
     * Cada posición puede analizarse independientemente.
     */
    #pragma omp parallel for
    for (size_t i = 0; i < n; i++) {
        if (i == 0 || s[i] != s[i - 1])
            start[i] = 1;
        else
            start[i] = 0;
    }

    /*
     * Asignación de identificadores de segmento.
     * Esta operación mantiene el orden necesario para construir
     * la salida.
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
     * Cada elemento de la salida tiene tamaño variable.
     * Primero calculamos cuánto espacio necesita cada segmento.
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
     * Los hilos escriben en regiones distintas de o,
     * por lo que no existe una condición de carrera.
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

int main()
{
    char t[] = "aaabbbccd";
    char o[99];

    _(t, o);

    printf("Entrada: %s\nSalida:  %s\n", t, o);

    return 0;
}
