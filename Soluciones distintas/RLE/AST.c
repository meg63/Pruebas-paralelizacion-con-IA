#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// ---------------------------------------------------------
// ALGORITMO INTACTO (Adaptado con malloc para soportar > 100 grupos)
// ---------------------------------------------------------
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
     * En lugar de arreglos de tamaño 100, asignamos dinámicamente
     * el tamaño exacto necesario ('grupos')
     */
    char **starts = malloc(grupos * sizeof(char *));
    char **ends   = malloc(grupos * sizeof(char *));

    if (!starts || !ends) return;

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
     * Esto elimina la dependencia de: o += sprintf(...)
     */
    char **dest = malloc(grupos * sizeof(char *));
    if (!dest) return;
    
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
    
    // Liberar la memoria de los punteros
    free(starts);
    free(ends);
    free(dest);
}
// ---------------------------------------------------------

// Función auxiliar: Generará SIEMPRE la misma cadena gracias a la semilla fija
char* generar_cadena_prueba(int longitud) {
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
    int sizes[] = {
        100,         // Pequeña
        10000000,    // Mediana (~10 MB)
        200000000    // Grande (~200 MB)
    };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Semilla fija para que las pruebas compitan con datos idénticos
    srand(42);

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int len = sizes[s_idx];
        printf("\n--- Prueba con cadena de %d caracteres ---\n", len);

        char *t = generar_cadena_prueba(len);
        char *o = (char*)malloc((len * 3 + 1) * sizeof(char));

        if (!t || !o) {
            printf("Error de memoria.\n");
            return 1;
        }

        double start = omp_get_wtime();
        
        _(t, o);
        
        double end = omp_get_wtime();
        double time_spent = end - start;

        if (len <= 100) {
            printf("Entrada: %s\n", t);
            printf("Salida:  %s\n", o);
        } else {
            printf("Salida truncada: %.30s ...\n", o);
        }
        
        printf("Tiempo paralelo (Punteros start/end): %f segundos\n", time_spent);

        free(t);
        free(o);
    }

    return 0;
}