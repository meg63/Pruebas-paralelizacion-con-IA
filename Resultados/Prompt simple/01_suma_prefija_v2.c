#include <stdio.h>
#include <omp.h>

int main(void) {
    int a[] = {3, 1, 7, 0, 4, 1, 6, 3};
    int out[8];
    int tmp[8];
    int n = 8;

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        out[i] = a[i];
    }

    for (int d = 1; d < n; d *= 2) {

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            if (i >= d)
                tmp[i] = out[i] + out[i - d];
            else
                tmp[i] = out[i];
        }

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            out[i] = tmp[i];
        }
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", out[i]);
    }

    putchar('\n');

    return 0;
}
