#include <iostream>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int current = 0;  // producer运行加1，consumer运行减1
int buf[10];
int in = 0, out = 0;
int items = 0, spaces = 10;
bool flag;  // 标记线程结束运行
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;  // 缓冲区不满
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;  // 缓冲区不空

void *producer( void *arg ) {
    while( flag ) {
        pthread_mutex_lock( &mutex );  // 为保证条件变量不会因为多线程混乱，所以先加锁
        while( !spaces ) {  // 避免“惊群”效应，避免因其他线程实现得到事件而导致该线程“假醒”
            pthread_cond_wait( &notfull, &mutex );
        }
        buf[in] = current++;
        in = ( in + 1 ) % 10;
        items++;
        spaces--;

        printf( "producer %zu , current = %d\n", pthread_self(), current );
        for( int i = 0; i < 10; i++ ) {
            printf( "%-4d", buf[i] );
        }
        printf( "\n\n" );

        pthread_cond_signal( &notempty );
        pthread_mutex_unlock( &mutex );
    }
    pthread_exit( NULL );
}

void *consumer( void *arg ) {
    while( flag ) {
        pthread_mutex_lock( &mutex );
        while( !items ) {
            pthread_cond_wait( &notempty, &mutex );
        }
        buf[out] = -1;
        out = ( out + 1 ) % 10;
        current--;
        items--;
        spaces++;

        printf( "consumer %zu , current = %d\n", pthread_self(), current );
        for( int i = 0; i < 10; i++ ) {
            printf( "%-4d", buf[i] );
        }
        printf( "\n\n" );

        pthread_cond_signal( &notfull );
        pthread_mutex_unlock( &mutex );
    }
    pthread_exit( NULL );
}

int main() {
    memset( buf, -1, sizeof(buf) );
    flag = true;
    pthread_t pro[10], con[10];
    int i = 0;

    for( int i = 0; i < 10; i++ ) {
        pthread_create( &pro[i], NULL, producer, NULL );
        pthread_create( &con[i], NULL, consumer, NULL );
    }

    sleep(1);  // 让线程运行一秒
    flag = false;

    for( int i = 0; i < 10; i++ ) {
        pthread_join( pro[i], NULL );
        pthread_join( con[i], NULL );
    }

    return 0;
} 

