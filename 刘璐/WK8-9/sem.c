#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 2            // 消费者或者生产者的数目
#define M 10           // 缓冲数目
int in = 0;            // 生产者放置产品的位置
int out = 0;           // 消费者取产品的位置
int buff[M] = {0};     // 缓冲初始化为0， 开始时没有产品
sem_t empty_sem;       // 同步信号量， 当满了时阻止生产者放产品
sem_t full_sem;        // 同步信号量， 当没产品时阻止消费者消费
pthread_mutex_t mutex; // 互斥信号量， 一次只有一个线程访问缓冲
int product_id = 0;    //生产者id
int prochase_id = 0;   //消费者id
/* 打印缓冲情况 */
void print()
{
    int i;
    for (i = 0; i < M; i++)
        printf("%d ", buff[i]);
    printf("\n");
}
/* 生产者方法 */
void *product(void* arg)
{
    int id = ++product_id;

    while (1)
    {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(1);

        sem_wait(&empty_sem);
        pthread_mutex_lock(&mutex);

        in = in % M;
        printf("product%d in %d. like: \t", id, in);

        buff[in] = 1;
        print();
        ++in;

        pthread_mutex_unlock(&mutex);
        sem_post(&full_sem);
    }
}
/* 消费者方法 */
void *prochase(void *arg)
{
    int id = ++prochase_id;
    while (1)
    {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(1);
        //sleep(1);

        sem_wait(&full_sem);
        pthread_mutex_lock(&mutex);

        out = out % M;
        printf("prochase%d in %d. like: \t", id, out);

        buff[out] = 0;
        print();
        ++out;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
    }
}
int main()
{
    pthread_t id1[N];
    pthread_t id2[N];
    int i;
    int ret[N];

    // 初始化同步信号量
    int ini1 = sem_init(&empty_sem, 0, M);
    int ini2 = sem_init(&full_sem, 0, 0);
    printf("%d %d\n",ini1,ini2);
    if (ini1 && ini2 != 0)
    {
        printf("sem init failed \n");
        exit(1);
    }
    //初始化互斥信号量
    int ini3 = pthread_mutex_init(&mutex, NULL);
    if (ini3 != 0)
    {
        printf("mutex init failed \n");
        exit(1);
    }
    // 创建N个生产者线程
    for (i = 0; i < N; i++)
    {
        ret[i] = pthread_create(&id1[i], NULL, product, (void *)(&i));
        if (ret[i] != 0)
        {
            printf("product%d creation failed \n", i);
            exit(1);
        }
    }
    //创建N个消费者线程
    for (i = 0; i < N; i++)
    {
        ret[i] = pthread_create(&id2[i], NULL, prochase, NULL);
        if (ret[i] != 0)
        {
            printf("prochase%d creation failed \n", i);
            exit(1);
        }
    }
    //销毁线程
    for (i = 0; i < N; i++)
    {
        pthread_join(id1[i], NULL);
        pthread_join(id2[i], NULL);
    }
    sem_destroy(id1[1]);
    return 0;
}