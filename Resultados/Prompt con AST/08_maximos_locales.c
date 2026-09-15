#include <stdio.h>
#include <omp.h>

int main(){
    int a[]={1,3,2,5,4,8,6,7,2}, n=sizeof(a)/4;
    int i, k=0;

    printf("out = [ ");

    #pragma omp parallel for
    for(i=0; i<n; i++){
        int mayor = (i ? a[i]>a[i-1] : 1) &
                    (i+1<n ? a[i]>a[i+1] : 1);

        if(mayor){
            #pragma omp critical
            {
                printf("%s[%d, %d]", k++ ? ", " : "", i, a[i]);
            }
        }
    }

    puts(" ]");
}
