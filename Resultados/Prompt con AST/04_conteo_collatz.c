#include <stdio.h>

#define _(n) n&1?n*3+1:n>>1

int c(long long n) {
    return n > 1 ? 1 + c(_(n)) : 0;
}

int main() {
    int a[] = {6, 7, 27, 15}, o[4], i = 0;

    printf("out = [ ");

    #pragma omp parallel for
    for (i = 0; i < 4; i++)
        o[i] = c(a[i]);

    for (i = 0; i < 4; i++)
        printf("%d%s", o[i], i == 3 ? " ]\n" : ", ");
}
