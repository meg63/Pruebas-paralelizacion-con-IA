#include <stdio.h>
#include <omp.h>

#define _(x) for (j = 2; j * j <= (x); j++)
#define O(a,b) ((a) % (b) ? 0 : (b) + ((b) * (b) != (a) ? (a) / (b) : 0))

int main() {
    int r[] = {2, 100000};

    printf("out = [ ");

    #pragma omp parallel for
    for (int i = r[0]; i <= r[1]; i++) {
        int s = 1;

        if (i > 1) {
            int j;

            _(i)
                s += O(i, j);

            if (s == i) {
                #pragma omp critical
                {
                    printf("%d ", i);
                }
            }
        }
    }

    puts("]");
    return 0;
}
