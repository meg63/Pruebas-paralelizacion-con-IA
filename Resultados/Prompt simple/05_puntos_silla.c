#include <stdio.h>
#include <omp.h>

int M[3][3] = {
    {8, 4, 1},
    {6, 9, 3},
    {7, 5, 2}
};

int main() {

    printf("[");

    #pragma omp parallel for
    for (int i = 0; i < 9; i++) {

        int j, k = 1;

        for (j = 0; j < 3; j++)
            k &= M[i / 3][j] >= M[i / 3][i % 3] &&
                 M[j][i % 3] <= M[i / 3][i % 3];

        if (k) {
            #pragma omp critical
            {
                printf("[%d, %d, %d] ",
                       M[i / 3][i % 3],
                       i / 3,
                       i % 3);
            }
        }
    }

    printf("]\n");

    return 0;
}