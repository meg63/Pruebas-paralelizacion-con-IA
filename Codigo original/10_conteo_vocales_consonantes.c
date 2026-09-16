#include <stdio.h>
#include <ctype.h>

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

int main(void) {
    const char *a[] = {"Hola", "Mundo", "Paralelismo", "C"};
    int n = sizeof(a) / sizeof(a[0]);
    Conteo out[n];

    procesar_cadenas(a, n, out);

    printf("out = [\n");
    for (int i = 0; i < n; i++) {
        printf("  [\"%s\", %d, %d]%s\n", out[i].cadena, out[i].vocales, out[i].consonantes, (i < n - 1) ? "," : "");
    }
    printf("]\n");

    return 0;
}