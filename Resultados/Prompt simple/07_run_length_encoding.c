#include <stdio.h>
#include <omp.h>

void _(char *s, char *o)
{
    int n = 0;

    // Contar la cantidad de grupos
    for (char *p = s; *p; ) {
        char c = *p;

        while (*p == c)
            p++;

        n++;
    }

    // Arreglos para guardar información de cada grupo
    int inicio[n];
    int longitud[n];

    // Detectar los grupos
    int i = 0;

    for (int pos = 0; s[pos] != '\0'; ) {
        int inicio_grupo = pos;

        while (s[pos] != '\0' &&
               s[pos] == s[inicio_grupo]) {
            pos++;
        }

        inicio[i] = inicio_grupo;
        longitud[i] = pos - inicio_grupo;
        i++;
    }

    // Calcular dónde comienza la salida de cada grupo
    int offset[n];
    int total = 0;

    for (i = 0; i < n; i++) {
        offset[i] = total;

        // Cada grupo genera: carácter + número
        // Como el número puede tener varios dígitos,
        // calculamos su longitud.
        int x = longitud[i];
        int digitos = 1;

        while (x >= 10) {
            x /= 10;
            digitos++;
        }

        total += 1 + digitos;
    }

    // Procesar los grupos en paralelo
    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        sprintf(o + offset[i],
                "%c%d",
                s[inicio[i]],
                longitud[i]);
    }

    o[total] = '\0';
}

int main()
{
    char t[] = "aaabbbccd";
    char o[99];

    _(t, o);

    printf("Entrada: %s\nSalida:  %s\n", t, o);

    return 0;
}
