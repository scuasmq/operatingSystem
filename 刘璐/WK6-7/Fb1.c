#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
const int maxn = 1e5+10;
int Read(char *s){
    int x=0,f=1;
    int i=0;
    while (s[i]!='\0')
    {
        if(s[i]=='-') return -1;
        x = (x<<1)+(x<<3)+(s[i]^48);
        i++;
    }
    return x*f;
}
void *Fibonacci(void *arg);
int f[maxn];
int n;
int main(int arg,char *args[]){
    if(arg==1) {
        printf("please input a parameter\n");
        exit(EXIT_FAILURE);
    }
    n = Read(args[1]);
    printf("n is %d\n",n);
    if(!n){
        printf("please input a positive parameter\n");
        exit(EXIT_FAILURE);
    }
    int id = fork();
    if(id==0){
        
    }

    return 0;
}