#include <stdio.h>
#include <omp.h>

void _(char *s, char *o)
{
    int n = 0;

    while (s[n] != '\0')
        n++;

    int *inicio = malloc(n * sizeof(int));
    int *longitud = malloc(n * sizeof(int));

    if (!inicio || !longitud)
        return;

    /*
     * Cada posición se analiza de forma independiente.
     */
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (i == 0 || s[i] != s[i - 1]) {
            inicio[i] = 1;

            int j = i + 1;
            while (j < n && s[j] == s[i])
                j++;

            longitud[i] = j - i;
        } else {
            inicio[i] = 0;
            longitud[i] = 0;
        }
    }

    /*
     * La construcción de la salida se mantiene secuencial
     * para conservar exactamente el orden del programa original.
     */
    for (int i = 0; i < n; i++) {
        if (inicio[i])
            o += sprintf(o, "%c%ld", s[i], (long)i);
    }

    *o = '\0';

    free(inicio);
    free(longitud);
}

int main()
{
    char t[] = "aaabbbccd";
    char o[99];

    _(t, o);

    printf("Entrada: %s\nSalida:  %s\n", t, o);

    return 0;
}
