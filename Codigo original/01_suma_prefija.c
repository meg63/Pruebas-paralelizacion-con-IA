#include <stdio.h>

void suma_prefija(const int a[], int out[], int n) {
    if (n <= 0) return;
    out[0] = a[0];
    for (int i = 1; i < n; i++) {
        out[i] = out[i - 1] + a[i];
    }
}

int main(void) {
    int a[] = {3, 1, 7, 0, 4, 1, 6, 3};
    int n = sizeof(a) / sizeof(a[0]);
    int out[8];

    suma_prefija(a, out, n);

    for (int i = 0; i < n; i++) {
        printf("%d ", out[i]);
    }
    printf("\n");
    return 0;
}