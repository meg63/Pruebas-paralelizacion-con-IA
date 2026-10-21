#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h> // Usamos omp.h para usar la misma métrica de tiempo

void run_length_encoding(const char *t, char *out) {
    int len = strlen(t);
    int out_index = 0;

    for (int i = 0; i < len; i++) {
        int count = 1;
        while (i + 1 < len && t[i] == t[i + 1]) {
            count++;
            i++;
        }
        out_index += sprintf(out + out_index, "%c%d", t[i], count);
    }
    out[out_index] = '\0';
}

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
        
        run_length_encoding(t, o);
        
        double end = omp_get_wtime();

        if (len <= 100) {
            printf("Entrada: %s\nSalida:  %s\n", t, o);
        } else {
            printf("Salida truncada: %.30s ...\n", o);
        }
        printf("Tiempo Secuencial: %f segundos\n", end - start);

        free(t);
        free(o);
    }
    return 0;
}