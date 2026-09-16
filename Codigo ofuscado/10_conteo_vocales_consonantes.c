#include <stdio.h>
#include <ctype.h>

typedef struct{const char*s;int v,c;}R;
#define V(k) (1<<((*k|32)-'a')&0x104111)

int main(){
    const char*a[]={"Hola","Mundo","Paralelismo","C"},**p=a;
    R r[4],*o=r;
    for(;p<a+4;p++,o++)
        for(o->s=*p,o->v=o->c=0;**p;(*p)++)
            isalpha(**p)?V(*p)?o->v++:o->c++:0;
    for(printf("out = [\n"),o=r;o<r+4;o++)
        printf("  [\"%s\", %d, %d]%s\n",o->s,o->v,o->c,o<r+3?",":"");
    return puts("]"),0;
}