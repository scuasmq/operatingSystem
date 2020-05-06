# thread



## 零 - *概览*

<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200501213838562.png" alt="image-20200501213838562" style="zoom:75%;" />

<font color = #e3c6d1>以下内容都是在linux下讨论</font>

## 一 - *pthread_creat()*

### 1、函数简介

- pthread_create是UNIX环境创建线程函数

### 2、头文件

- \#include<pthread.h>

### 3、**函数声明**

```c
int pthread_create(
pthread_t *thread,    					// 第一个参数为指向线程标识符的指针。
pthread_attr_t *attr, 					// 第二个参数用来设置线程属性。常为NULL
void *(*start_routine)(void*),  // 第三个参数是线程运行函数的地址。
void *arg)											// 最后一个参数是运行函数的参数。
```

### 4、返回值

- 若成功则返回0，否则返回出错编号

### 5、注意

- 在编译时注意加上-lpthread参数，以调用静态链接库。因为pthread并非Linux系统的默认库。

## 二 - *pthread_join()*

### 1、函数简介

- pthread_join()函数，以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的。

### 2、头文件

- \#include<pthread.h>

### 3、**函数声明**

```c
extern int pthread_join __P (pthread_t __th							//第一个参数为被等待的线程标识符
                             , void **__thread_return); //第二个参数为一个用户定义的指针，它可以用来存储被等待线程的返回值。
```

### 4、返回值

- 若成功则返回0，否则返回出错编号

### 5、注意

-  这个函数是一个线程阻塞的函数，调用它的函数将一直等待到被等待的线程结束为止，当函数返回时，被等待线程的资源被收回。

## 三 - *pthread_exit()*

### 1、函数简介

- 线程通过调用pthread_exit函数终止执行，就如同进程在结束时调用exit函数一样。这个函数的作用是，终止调用它的线程并返回一个指向某个对象的[指针](https://baike.baidu.com/item/指针/2878304)。

### 2、头文件

- \#include<pthread.h>

### 3、**函数声明**

```c
void pthread_exit(void* retval)
```

### 4、返回值

- 一个指向某个对象的指针，储存在`ptread_join`指定的指针中。

### 5、注意

-  不能用它来返回一个指向局部变量的指针，因为线程调用该函数后，这个局部变量就不存在了

### 6、例程

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
void *thread_function(void *arg);
char message[] = "Hello world";
int main(){
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = pthread_create(&a_thread,NULL,thread_function,(void*)message);
    if(res!=0){ perror("Thread creation failed");
                exit(EXIT_FAILURE);}
    printf("Waiting for thread to finish..\n");
    res = pthread_join(a_thread,&thread_result);
    if(res!=0){ perror("Thread join failed");
                exit(EXIT_FAILURE);}
    printf("Thread joined, it returned %s\n",(char*) thread_result);
    printf("Message is now %s\n",message);
    exit(EXIT_SUCCESS);
}
void *thread_function(void *arg){
    printf("thread_function is running. Arugment was %s \n",(char*)arg);
    sleep(3);
    strcpy(message,"Bye!");
    pthread_exit("Thank you for the CPU time");
}
```

## 



