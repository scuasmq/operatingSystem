#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
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
    pthread_t a_thread;
    void *thread_result;
    int res = pthread_create(&a_thread,NULL,Fibonacci,(void*)args);
    if(res){
        printf("create pthread failed\n");
        exit(EXIT_FAILURE);
    }
    void* thread_res;
    res = pthread_join(a_thread,&thread_res);
    if(res){
        printf("join pthread failed\n");
        exit(EXIT_FAILURE);
    }
    printf("after thread: %s\n",(char*)thread_res);
    for(int i=0;i<=n;i++){
        printf("%d: %d\n",i,f[i]);
    }
    return 0;
}

void * Fibonacci(void *arg){
    f[0] = f[1] = 1;
    for(int i=2;i<=n;i++) f[i] = f[i-1]+f[i-2];
    scanf("%d",&f[0]);
    pthread_exit("fibonacci get!");
}