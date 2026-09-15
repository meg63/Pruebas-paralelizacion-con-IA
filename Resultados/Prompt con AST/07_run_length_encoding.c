#include <stdio.h>
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
     * Para este AST concreto conocemos que la salida de cada grupo
     * tiene la forma "%c%ld".
     *
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
     * Primero calculamos dónde escribirá cada grupo.
     * Esto elimina la dependencia de:
     *
     *     o += sprintf(...)
     *
     * del AST original.
     */
    char *dest[100];
    char *out = o;

    for (int i = 0; i < grupos; i++) {
        dest[i] = out;

        /*
         * El máximo necesario para "%c%ld" se reserva
         * mediante un pequeño cálculo conservador.
         */
        int n = (int)(ends[i] - starts[i]);
        int written = snprintf(out, 32, "%c%ld",
                               *starts[i], (long)n);

        out += written;
    }

    /*
     * Cada iteración escribe en una zona distinta de o,
     * por lo que estas operaciones sí son independientes.
     */
    #pragma omp parallel for
    for (int i = 0; i < grupos; i++) {
        int n = (int)(ends[i] - starts[i]);

        sprintf(dest[i], "%c%ld",
                *starts[i], (long)n);
    }
}

int main()
{
    char t[] = "aaabbbccd";
    char o[99];

    _(t, o);

    printf("Entrada: %s\nSalida:  %s\n", t, o);

    return 0;
}
