#include <stdio.h>
#include <omp.h>

void c(char *s, int n)
{
    int len = 0;

    while (s[len] != '\0')
        len++;

    #pragma omp parallel for
    for (int i = 0; i < len; i++) {
        int b = s[i] & 32 | 65;

        if ((s[i] | 32) >= 'a' && (s[i] | 32) <= 'z')
            s[i] = b + (s[i] - b + (n % 26 + 26)) % 26;
    }
}

int main()
{
    char t[] = "HOLA MUNDO";

    c(t, 3);

    printf("out = \"%s\"\n", t);

    return 0;
}
