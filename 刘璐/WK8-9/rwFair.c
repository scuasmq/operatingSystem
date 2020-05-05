#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define WRITER_NUM 3
#define READER_NUM 5
#define W_SLEEP 1
#define R_SLEEP 1

pthread_t wid[WRITER_NUM],rid[READER_NUM];
const int MAX_RAND = 1000;
pthread_mutex_t rw,w,mutexReaderCnt;
int readerCnt = 0;

void mutexInit(){
    pthread_mutex_init(&rw,NULL);
    pthread_mutex_init(&w,NULL);
    pthread_mutex_init(&mutexReaderCnt,NULL);
}
void writeAction(){
    printf("writing\n");
}
void readAction(){
    printf("reading\n");
}

void *writer(void* arg){
    while (1)
    {
        pthread_mutex_lock(&w);
        pthread_mutex_lock(&rw);
        writeAction();
        pthread_mutex_unlock(&rw);
        pthread_mutex_unlock(&w);
        sleep(W_SLEEP);
    }
    pthread_exit("writer exit");
}
void *reader(void* arg){
    while (1)
    {
        pthread_mutex_lock(&w);
        pthread_mutex_lock(&mutexReaderCnt);
        if(readerCnt==0)
            pthread_mutex_lock(&rw);
        readerCnt++;
        pthread_mutex_unlock(&mutexReaderCnt);
        pthread_mutex_unlock(&w);//何时释放相当重要

        readAction();

        pthread_mutex_lock(&mutexReaderCnt);
        readerCnt--;
        if(readerCnt==0)
            pthread_mutex_unlock(&rw);
        pthread_mutex_unlock(&mutexReaderCnt);

        sleep(R_SLEEP);
    }
    pthread_exit("reader exit");
}
int main(){
    mutexInit();
    for(int i=0;i<READER_NUM;i++)   pthread_create(&rid[i],NULL,reader,NULL);
    for(int i=0;i<WRITER_NUM;i++)   pthread_create(&wid[i],NULL,writer,NULL);
    while (1)
    {
        sleep(10);
    }
    
    return 0;
}