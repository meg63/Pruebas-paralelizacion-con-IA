#include <stdio.h>
#define _(x) for(j=2;j*j<=x;j++)
#define O(a,b) (a%b?0:b+(b*b!=a?a/b:0))
int main(){int r[]={2,100000},i=r[0],j,s;printf("out = [ ");
for(;i<=r[1];i++,s=1){if(i>1){_(i)s+=O(i,j);if(s==i)printf("%d ",i);}}
puts("]");return 0;}