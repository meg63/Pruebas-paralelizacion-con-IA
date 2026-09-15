#include <stdio.h>

int main(){
    int a[]={1,3,2,5,4,8,6,7,2},n=sizeof(a)/4,i=-1,k=0;
    for(printf("out = [ ");++i<n;)
        (i?a[i]>a[i-1]:1)&(i+1<n?a[i]>a[i+1]:1)&&printf(", [%d, %d]"+!k++,i,a[i]);
    puts(" ]");
}