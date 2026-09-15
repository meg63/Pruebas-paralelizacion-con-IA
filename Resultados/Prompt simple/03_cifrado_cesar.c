#include <stdio.h>
#include <omp.h>

void c(char *s, int n) {
    int len = 0;

    // Obtener la longitud de la cadena
    while (s[len] != '\0')
        len++;

    #pragma omp parallel for
    for (int i = 0; i < len; i++) {
        char ch = s[i];

        if ((ch | 32) >= 'a' && (ch | 32) <= 'z') {
            int b = ch & 32 | 65;
            s[i] = b + (ch - b + (n % 26 + 26)) % 26;
        }
    }
}

int main() {
    char t[] = "HOLA MUNDO";

    c(t, 3);

    printf("out = \"%s\"\n", t);

    return 0;
}
