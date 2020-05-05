#include <stdio.h>
#include <stdlib.h>  
#include <pthread.h>  
  
  
#define N_WRITER 3 //写者数目  
#define N_READER 5 //读者数目  
#define W_SLEEP  1 //控制写频率  
#define R_SLEEP  1 //控制读频率  
  
  
pthread_t wid[N_WRITER],rid[N_READER];  
const int MAX_RAND = 1000;//产生的最大随机数  
pthread_mutex_t writeLock = PTHREAD_MUTEX_INITIALIZER;//同一时间只能一个人写文件,互斥  
pthread_mutex_t accessReaderCnt = PTHREAD_MUTEX_INITIALIZER;//同一时间只能有一个人访问 readerCnt  
int data = 0;
int readerCnt = 0;
void write() 
{  
    int rd = rand();  
    printf("write %d\n",rd);  
    data = rd;  
}  
void read()  
{  
    printf("read %d\n",data);  
}  
void * writer(void * in)  
{  
    while(1)  
    {  
        pthread_mutex_lock(&writeLock);  
        write();  
        pthread_mutex_unlock(&writeLock);  
        sleep(W_SLEEP);  
    }  
    pthread_exit((void *) 0);  
}  
  
void * reader (void * in)  
{  
    while(1)  
    {  
        pthread_mutex_lock(&accessReaderCnt);  
        readerCnt++;  
        if(readerCnt == 1){  
            pthread_mutex_lock(&writeLock);  
        }  
        pthread_mutex_unlock(&accessReaderCnt);  
          
        read();  
          
        pthread_mutex_lock(&accessReaderCnt);  
        readerCnt--;  
        if(readerCnt == 0){  
            pthread_mutex_unlock(&writeLock);  
        }  
        pthread_mutex_unlock(&accessReaderCnt);  
        sleep(R_SLEEP);  
    }  
    pthread_exit((void *) 0);  
}  
  
int main()  
{  
    int i = 0;  
    for(i = 0; i < N_READER; i++)  
    {  
        pthread_create(&wid[i],NULL,reader,NULL);  
    }
    for(i = 0; i < N_WRITER; i++)  
    {  
        pthread_create(&rid[i],NULL,writer,NULL);  
    }  
    while(1){  
        sleep(10);  
    }  
    return 0;  
}