#include <stdio.h>
#include <omp.h>

int M[3][3] = {
    {8,4,1},
    {6,9,3},
    {7,5,2}
};

int i, j, k, f;

int main() {
    int valido[9];

    putchar('[');

    #pragma omp parallel for
    for (i = 0; i < 9; i++) {
        int k_local = 1;

        for (int j_local = 0; j_local < 3; j_local++)
            k_local &= *(*M + i/3*3 + j_local) >= *(*M + i)
                    && *(*M + j_local*3 + i%3) <= *(*M + i);

        valido[i] = k_local;
    }

    /*
     * La salida permanece secuencial para conservar
     * el orden del programa original y evitar carreras
     * sobre f y stdout.
     */
    for (i = 0; i < 9; i++) {
        if (valido[i])
            printf("%s[%d, %d, %d]",
                   f++ ? ", " : "",
                   *(*M + i),
                   i/3,
                   i%3);
    }

    puts(" ]");

    return 0;
}
