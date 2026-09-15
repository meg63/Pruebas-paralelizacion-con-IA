#include <stdio.h>
#include <ctype.h>

void cifrado_cesar(char *texto, int n) {
    
    int shift = (n % 26 + 26) % 26; 

    for (int i = 0; texto[i] != '\0'; i++) {
        if (isupper(texto[i])) {
            texto[i] = 'A' + (texto[i] - 'A' + shift) % 26;
        } else if (islower(texto[i])) {
            texto[i] = 'a' + (texto[i] - 'a' + shift) % 26;
        }
    }
}

int main() {
    char t[] = "HOLA MUNDO";
    int n = 3;

    cifrado_cesar(t, n);
    printf("out = \"%s\"\n", t); 

    return 0;
}