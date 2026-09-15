#include <stdio.h>

void _(char*s,char*o){for(char*p=s;*p;o+=sprintf(o,"%c%ld",*s,p-s),s=p)for(p=s;*p==*s;p++);}

int main(){
    char t[]="aaabbbccd",o[99];
    _(t,o);
    printf("Entrada: %s\nSalida:  %s\n",t,o);
    return 0;
}