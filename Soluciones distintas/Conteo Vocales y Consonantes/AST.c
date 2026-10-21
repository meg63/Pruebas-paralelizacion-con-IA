#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <omp.h>

// ---------------------------------------------------------
// ALGORITMO PARALELO INTACTO (Variante puntero q)
// ---------------------------------------------------------
typedef struct{const char*s;int v,c;}R;
#define V(k) (1<<((*k|32)-'a')&0x104111)

void procesar_cadenas_paralelo_q(const char *a[], int n, R r[]) {
    #pragma omp parallel for
    for(int i=0;i<n;i++) {
        r[i].s = a[i];
        r[i].v = r[i].c = 0;

        for(const char *q=a[i]; *q; q++)
            isalpha(*q) ? V(q) ? r[i].v++ : r[i].c++ : 0;
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
        100,         // Pequeña
        1000000,     // Mediana (~20 MB)
        10000000     // Grande (~200 MB)
    };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Semilla fija para que todas las pruebas futuras procesen exactamente las mismas palabras
    srand(42);

    for (int s_idx = 0; s_idx < num_sizes; s_idx++) {
        int n = sizes[s_idx];
        printf("\n--- Prueba con arreglo de %d cadenas ---\n", n);

        const char **a = generar_cadenas_prueba(n);
        // Usar malloc para evitar Stack Overflow con arreglos grandes
        R *r = (R *)malloc(n * sizeof(R));

        if (!a || !r) {
            printf("Error de memoria.\n");
            return 1;
        }

        // Medición de tiempo de procesamiento
        double start = omp_get_wtime();
        
        procesar_cadenas_paralelo_q(a, n, r);
        
        double end = omp_get_wtime();
        double time_spent = end - start;

        // Mostrar resultados (usando la misma lógica de punteros de tu impresión original)
        if (n <= 100) {
            printf("out = [\n");
            for(R *o=r; o<r+n; o++) {
                printf("  [\"%s\", %d, %d]%s\n",
                       o->s, o->v, o->c, o < r+(n-1) ? "," : "");
            }
            printf("]\n");
        } else {
            // Para datos grandes mostramos un resumen
            printf("Procesamiento completado. Resumen:\n");
            printf("  Primera -> [\"%s\", V:%d, C:%d]\n", r[0].s, r[0].v, r[0].c);
            printf("  Ultima  -> [\"%s\", V:%d, C:%d]\n", r[n-1].s, r[n-1].v, r[n-1].c);
        }
        
        printf("Tiempo paralelo (Variante puntero q): %f segundos\n", time_spent);

        // Liberar memoria
        liberar_cadenas(a, n);
        free(r);
    }

    return 0;
}