#include <stdio.h>
#include <ctype.h>
#include <omp.h>

typedef struct{const char*s;int v,c;}R;
#define V(k) (1<<((*k|32)-'a')&0x104111)

int main(){
    const char*a[]={"Hola","Mundo","Paralelismo","C"};
    R r[4];

    #pragma omp parallel for
    for(int i=0;i<4;i++){
        r[i].s=a[i];
        r[i].v=r[i].c=0;

        for(const char *p=a[i];*p;p++)
            isalpha(*p)?V(p)?r[i].v++:r[i].c++:0;
    }

    for(printf("out = [\n"),R *o=r;o<r+4;o++)
        printf("  [\"%s\", %d, %d]%s\n",
               o->s,o->v,o->c,o<r+3?",":"");

    return puts("]"),0;
}
