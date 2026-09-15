#include <stdio.h>
#include <omp.h>

int M[3][3] = {{8,4,1},{6,9,3},{7,5,2}}, i, j, k, f;

int main() {
    for (putchar('['); i < 9; i++) {

        for (j = 0, k = 1; j < 3; j++)
            ;

        k = 1;

        #pragma omp parallel for reduction(&:k)
        for (j = 0; j < 3; j++)
            k &= *(*M + i/3*3 + j) >= *(*M + i)
              && *(*M + j*3 + i%3) <= *(*M + i);

        if (k)
            printf("%s[%d, %d, %d]",
                   f++ ? ", " : "",
                   *(*M + i), i/3, i%3);
    }

    puts(" ]");
    return 0;
}
