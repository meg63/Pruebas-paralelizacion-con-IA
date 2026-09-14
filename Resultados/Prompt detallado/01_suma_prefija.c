#include <stdio.h>
#include <omp.h>

#define _(x,y) (x ? y[x-1] : 0)
#define S(i)   (i[out] = i[a] + _(i,out))

int main(void) {
    int a[] = {3, 1, 7, 0, 4, 1, 6, 3};
    int out[8];

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < 8; ++i) {

                if (i == 0) {
                    #pragma omp task depend(out: out[i])
                    {
                        int k = i;
                        S(k);
                    }
                } else {
                    #pragma omp task depend(in: out[i-1]) \
                                     depend(out: out[i])
                    {
                        int k = i;
                        S(k);
                    }
                }
            }

            #pragma omp taskwait
        }
    }

    for (int i = 0; i < 8; ++i)
        printf("%d ", out[i]);

    putchar('\n');

    return 0;
}
