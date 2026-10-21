#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void _(char *s, char *o)
{
    int n = 0;

    for (char *p = s; *p; ) {
        char c = *p;
        while (*p == c) p++;
        n++;
    }

    int *inicio = (int *)malloc(n * sizeof(int));
    int *longitud = (int *)malloc(n * sizeof(int));

    int i = 0;
    for (int pos = 0; s[pos] != '\0'; ) {
        int inicio_grupo = pos;
        while (s[pos] != '\0' && s[pos] == s[inicio_grupo]) pos++;
        inicio[i] = inicio_grupo;
        longitud[i] = pos - inicio_grupo;
        i++;
    }

    int *offset = (int *)malloc(n * sizeof(int));
    int total = 0;

    for (i = 0; i < n; i++) {
        offset[i] = total;
        int x = longitud[i];
        int digitos = 1;
        while (x >= 10) {
            x /= 10;
            digitos++;
        }
        total += 1 + digitos;
    }

    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        sprintf(o + offset[i], "%c%d", s[inicio[i]], longitud[i]);
    }

    o[total] = '\0';

    free(inicio);
    free(longitud);
    free(offset);
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
        printf("Tiempo Paralelo (Offsets): %f segundos\n", end - start);

        free(t);
        free(o);
    }
    return 0;
}