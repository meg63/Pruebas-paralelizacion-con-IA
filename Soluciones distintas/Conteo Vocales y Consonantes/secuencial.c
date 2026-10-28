#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <omp.h>

// ---------------------------------------------------------
// ALGORITMO INTACTO
// ---------------------------------------------------------
typedef struct {
    const char *cadena;
    int vocales;
    int consonantes;
} Conteo;

int es_vocal(char c) {
    c = tolower((unsigned char)c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

void procesar_cadenas(const char *a[], int n, Conteo out[]) {
    for (int i = 0; i < n; i++) {
        out[i].cadena = a[i];
        out[i].vocales = 0;
        out[i].consonantes = 0;

        for (int j = 0; a[i][j] != '\0'; j++) {
            char c = a[i][j];
            if (isalpha((unsigned char)c)) {
                if (es_vocal(c)) {
                    out[i].vocales++;
                } else {
                    out[i].consonantes++;
                }
            }
        }
    }
}
// ---------------------------------------------------------

// Función auxiliar para generar un arreglo de cadenas aleatorias
const char** generar_cadenas_prueba(int n) {
    const char **a = (const char **)malloc(n * sizeof(char*));
    if (!a) return NULL;
    
    for (int i = 0; i < n; i++) {
        int len = 5 + (rand() % 10); // Cadenas de entre 5 y 14 caracteres
        char *str = (char *)malloc((len + 1) * sizeof(char));
        
        for (int j = 0; j < len; j++) {
            int mayuscula = rand() % 2;
            str[j] = mayuscula ? 'A' + (rand() % 26) : 'a' + (rand() % 26);
        }
        str[len] = '\0';
        a[i] = str;
    }
    return a;
}

// Función auxiliar para liberar la memoria de las cadenas
void liberar_cadenas(const char **a, int n) {
    for (int i = 0; i < n; i++) {
        free((void*)a[i]);
    }
    free(a);
}

int main(void) {
    int sizes[] = {
        1000000,     // Pequeña
        10000000,     // Mediana 
        100000000,    //Grande
    };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Semilla fija para que todas las pruebas futuras procesen exactamente las mismas palabras
    srand(42);

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int n = sizes[s_idx];
        printf("\n--- Prueba con arreglo de %d cadenas ---\n", n);

        const char **a = generar_cadenas_prueba(n);
        // Usar malloc para evitar Stack Overflow con arreglos grandes
        Conteo *out = (Conteo *)malloc(n * sizeof(Conteo));

        if (!a || !out) {
            printf("Error de memoria.\n");
            return 1;
        }

        // Medición de tiempo de procesamiento
        double start = omp_get_wtime();
        
        procesar_cadenas(a, n, out);
        
        double end = omp_get_wtime();
        double time_spent = end - start;

        // Mostrar resultados
        if (n <= 100) {
            printf("out = [\n");
            for (int i = 0; i < n; i++) {
                printf("  [\"%s\", %d, %d]%s\n", 
                       out[i].cadena, out[i].vocales, out[i].consonantes, 
                       (i < n - 1) ? "," : "");
            }
            printf("]\n");
        } else {
            // Para datos grandes mostramos un resumen (la primera y última cadena procesada)
            printf("Procesamiento completado. Resumen:\n");
            printf("  Primera -> [\"%s\", V:%d, C:%d]\n", out[0].cadena, out[0].vocales, out[0].consonantes);
            printf("  Ultima  -> [\"%s\", V:%d, C:%d]\n", out[n-1].cadena, out[n-1].vocales, out[n-1].consonantes);
        }
        
        printf("Tiempo secuencial: %f segundos\n", time_spent);

        // Liberar memoria
        liberar_cadenas(a, n);
        free(out);
    }

    return 0;
}