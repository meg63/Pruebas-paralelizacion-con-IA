#include <stdio.h>
#include <omp.h>

#define _(n) ((n)&1 ? (n)*3+1 : (n)>>1)

int c(long long n) {
    return n > 1 ? 1 + c(_(n)) : 0;
}

int main() {
    int a[] = {6, 7, 27, 15};
    int o[4];

    #pragma omp parallel for
    for (int i = 0; i < 4; i++) {
        o[i] = c(a[i]);
    }

    printf("out = [ ");
    for (int i = 0; i < 4; i++) {
        printf("%d%s", o[i], i == 3 ? " ]\n" : ", ");
    }

    return 0;
}
