#include <stdio.h>
#include <omp.h>

#define _(x) for(j=2;j*j<=x;j++)
#define O(a,b) (a%b?0:b+(b*b!=a?a/b:0))

int main()
{
    int r[]={2,100000}, i, j, s;

    printf("out = [ ");

    #pragma omp parallel for ordered private(j, s)
    for(i=r[0]; i<=r[1]; i++) {
        s = 1;

        if(i > 1) {
            _(i)
                s += O(i,j);

            if(s == i) {
                #pragma omp ordered
                printf("%d ", i);
            }
        }
    }

    puts("]");
    return 0;
}