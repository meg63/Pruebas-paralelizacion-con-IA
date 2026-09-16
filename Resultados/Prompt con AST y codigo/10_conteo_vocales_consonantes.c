#include <stdio.h>
#include <ctype.h>
#include <omp.h>

typedef struct {
    const char *s;
    int v, c;
} R;

#define V(k) (1 << ((*k | 32) - 'a') & 0x104111)

int main() {
    const char *a[] = {"Hola", "Mundo", "Paralelismo", "C"};
    R r[4];

    #pragma omp parallel for
    for (int i = 0; i < 4; i++) {
        const char *p = a[i];

        r[i].s = a[i];
        r[i].v = 0;
        r[i].c = 0;

        for (; *p; p++) {
            isalpha(*p) ? V(p) ? r[i].v++ : r[i].c++ : 0;
        }
    }

    printf("out = [\n");

    for (int i = 0; i < 4; i++) {
        printf("  [\"%s\", %d, %d]%s\n",
               r[i].s, r[i].v, r[i].c,
               i < 3 ? "," : "");
    }

    puts("]");

    return 0;
}
