#include <stdio.h>
#include <omp.h>

#define _(x,y) (x ? y[x-1] : 0)
#define S(i)   (i[out] = i[a] + _(i,out))

int main(void) {
    int a[] = {3, 1, 7, 0, 4, 1, 6, 3};
    int out[8];

    int n = 8;
    int nthreads, *suma;

    #pragma omp parallel
    {
        #pragma omp single
        {
            nthreads = omp_get_num_threads();
            suma = calloc(nthreads, sizeof(int));
        }

        int tid = omp_get_thread_num();
        int inicio = tid * n / nthreads;
        int fin    = (tid + 1) * n / nthreads;

        /* Cada hilo calcula su bloque.
           La dependencia out[i-1] se mantiene dentro del bloque. */
        for (int i = inicio; i < fin; ++i) {
            out[i] = a[i] + (i ? out[i - 1] : 0);
        }

        /* Suma final del bloque */
        if (fin > inicio)
            suma[tid] = out[fin - 1];

        #pragma omp barrier

        /*
         * Se obtiene el desplazamiento correspondiente
         * a los bloques anteriores.
         */
        int offset = 0;
        for (int t = 0; t < tid; ++t)
            offset += suma[t];

        /* Se corrige el bloque con el prefijo anterior. */
        if (offset != 0) {
            for (int i = inicio; i < fin; ++i)
                out[i] += offset;
        }
    }

    for (int i = 0; i < n; ++i)
        printf("%d ", out[i]);

    putchar('\n');

    free(suma);
    return 0;
}
