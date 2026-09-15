#include <stdio.h>

void c(char*s,int n){while(*s){int b=*s&32|65;if((*s|32)>='a'&&(*s|32)<='z')*s=b+(*s-b+(n%26+26))%26;s++;}}

int main(){
    char t[]="HOLA MUNDO";
    c(t,3);
    printf("out = \"%s\"\n",t);
    return 0;
}