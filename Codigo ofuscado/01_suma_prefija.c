#include <stdio.h>

#define _(x,y) (x?y[x-1]:0)
#define S(i) (i[out]=i[a]+_(i,out))

int main(void) {
    int a[] = {3, 1, 7, 0, 4, 1, 6, 3}, out[8], i = 0;
    while (i < 8 ? printf("%d ", S(i)), ++i : 0);
    putchar(10);
    return 0;
}