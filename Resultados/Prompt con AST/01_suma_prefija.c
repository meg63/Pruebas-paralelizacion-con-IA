#include <stdio.h>
#include <omp.h>

#define _(x,y) (x ? y[x-1] : 0)
#define S(i) (i[out] = i[a] + _(i,out))

int main(void) {
    int a[] = {3, 1, 7, 0, 4, 1, 6, 3}, out[8], i = 0;

    #pragma omp parallel
    #pragma omp single
    {
        for (i = 0; i < 8; ++i) {
            if (i == 0) {
                #pragma omp task depend(out: out[i])
                out[i] = a[i];
            } else {
                #pragma omp task depend(in: out[i-1]) depend(out: out[i])
                out[i] = a[i] + out[i-1];
            }
        }
    }

    for (i = 0; i < 8; ++i)
        printf("%d ", out[i]);

    putchar(10);
    return 0;
}
