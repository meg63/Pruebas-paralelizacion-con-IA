#include <stdio.h>

#define N 3

void sumar_antidiagonales(int n, int m[n][n], int out[]) {
    
    for (int k = 0; k < 2 * n - 1; k++) {
        out[k] = 0;
    }

   
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            out[i + j] += m[i][j];
        }
    }
}

int main() {
    int m[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    int out[2 * N - 1];

    sumar_antidiagonales(N, m, out);

   
    printf("out = [ ");
    for (int i = 0; i < 2 * N - 1; i++) {
        printf("%d%s", out[i], (i == 2 * N - 2) ? " ]\n" : ", ");
    }

    return 0;
}