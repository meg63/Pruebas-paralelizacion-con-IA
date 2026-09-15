#include <stdio.h>
#define _ 4
#define O 999999
#define W(x) ((x)<O)
#define P(x) (x>=O?printf("INF\t"):printf("%d\t",x))

void F(int*m,int*o){
    for(int i=0;i<_*_;i++)*(o+i)=*(m+i);
    for(int k=0;k<_;k++)
        for(int i=0;i<_*_;i++){
            int a=*(o+(i/_)*_+k),b=*(o+k*_+i%_);
            *(o+i)=W(a)&&W(b)&&a+b<*(o+i)?a+b:*(o+i);
        }
}

int main(){
    int m[_][_]={{0,5,O,10},{O,0,3,O},{O,O,0,1},{O,O,O,0}},o[_][_];
    F((int*)m,(int*)o);
    for(int i=0;i<_*_;P(o[i/_][i%_]),i++%_==_-1?puts(""):0);
    return 0;
}