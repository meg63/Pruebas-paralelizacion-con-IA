#include <stdio.h>
#include <string.h>

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

int main() {
    char t[] = "aaabbbccd";
    char out[100];

    run_length_encoding(t, out);

    printf("Entrada: %s\n", t);
    printf("Salida:  %s\n", out);

    return 0;
}