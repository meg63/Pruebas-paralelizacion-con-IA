#include <stdio.h>
#include <stdlib.h>
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

    for (int i = 0; i < n; i++) {
        if (inicio[i])
            o += sprintf(o, "%c%ld", s[i], (long)i);
    }

    *o = '\0';

    free(inicio);
    free(longitud);
}

char* generar_cadena_prueba(int longitud) {
    char *cadena = (char*)malloc((longitud + 1) * sizeof(char));
    if (!cadena) return NULL;
    
    int i = 0;
    while (i < longitud) {
        char c = 'a' + (rand() % 26); 
        int repeticiones = 1 + (rand() % 20); 
        
        for (int j = 0; j < repeticiones && i < longitud; j++) cadena[i++] = c;
    }
    cadena[longitud] = '\0';
    return cadena;
}

int main() {
    int sizes[] = { 100, 10000000, 200000000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    srand(42); // Semilla fija

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int len = sizes[s_idx];
        printf("\n--- Prueba con cadena de %d caracteres ---\n", len);

        char *t = generar_cadena_prueba(len);
        char *o = (char*)malloc((len * 3 + 1) * sizeof(char));

        if (!t || !o) return 1;

        double start = omp_get_wtime();
        
        _(t, o);
        
        double end = omp_get_wtime();

        if (len <= 100) {
            printf("Entrada: %s\nSalida:  %s\n", t, o);
        } else {
            printf("Salida truncada: %.30s ...\n", o);
        }
        printf("Tiempo Paralelo (Indices): %f segundos\n", end - start);

        free(t);
        free(o);
    }
    return 0;
}