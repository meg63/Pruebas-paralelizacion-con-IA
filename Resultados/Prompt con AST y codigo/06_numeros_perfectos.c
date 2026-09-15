#include <stdio.h>
#include <omp.h>

#define _(x) for(j=2;j*j<=x;j++)
#define O(a,b) (a%b?0:b+(b*b!=a?a/b:0))

int main()
{
    int r[] = {2,100000};
    int i, j, s;
    int found[100001] = {0};

    printf("out = [ ");

    #pragma omp parallel for private(j, s) shared(r, found)
    for(i = r[0]; i <= r[1]; i++) {
        s = 1;

        if(i > 1) {
            _(i)
                s += O(i,j);

            if(s == i)
                found[i] = 1;
        }
    }

    /* Se mantiene la salida en el mismo orden que el programa secuencial. */
    for(i = r[0]; i <= r[1]; i++) {
        if(found[i])
            printf("%d ", i);
    }

    puts("]");

    return 0;
}