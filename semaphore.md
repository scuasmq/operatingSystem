# Semaphore

[TOC]

## 零、概览

**信号量（semaphore）机制**  

- 解决并发进程同步的工具
- P操作表示同步进程发出的检测信号量操作，检测是否能够使用临界资源
- V操作表示访问完临界资源的进程通知等待进程已经完成了临界资源的访问，将临界资源释放。

## 一、**经典互斥与同步问题回顾**

### 1 - **生产者-消费者问题**

[producer–consumer problem](producer–consumer problem.md)

### 2 - **读者-写者问题**

[readers–writers problem](readers–writers problem.md)

### 3 - **哲学家进餐问题**

[dining-philosopher problem](dining-philosopher problem.md)

## 二、同步对象

**同步对象是指Windows/Linux中用于实现同步与互斥的实体，**

**包括互斥量(Mutex)、信号量(Semaphore)、临界区(Critical Section)和事件(Events)等。**



**以读者-写者(读者优先)为例：**

**1.创建若干线程分别模拟读者操作和写者操作**

**2.读线程间和写线程间对各自局部共享资源的访问修改采用** **Mutex对象，结合WaitForSingleObject** **保证互斥操作。**

**3.读线程与写线程争用全局临界资源采用临界区(Critical** **Seciton)。**

**4.统管读写线程的线程采用WaitForMultipleObjects** **保证等待所有的线程结束。**



## 三、互斥量

### 1.创建互斥量函数

- 初始化

  ```c
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER
  Eg： pthread_mutex_t mylock;
  ```

- 功能:**函数功能：该函数初始化临界区对象。**

- 函数原型

  ```c
  int pthread_mutext_init(pthread_mutex_t *restrict mutex,						//第一个参数初始化的互斥量的指针；（&mylock）
                          const pthread_mutexattr_t *restrict attr);	//第二个参数attr：指定了新建互斥锁的属性。如果参数attr为NULL，则使用默认的互斥锁属性，默认属性为快速互斥锁 
  ```

- 返回值

  **函数成功执行后，返回0，互斥锁被初始化为锁住态**

### 2.加锁互斥量函数

- 功能:禁止其他的线程访问对象。

- 函数原型

  ```c
  int pthread_mutex_lock(pthread_mutex_t *mutex);
  //函数功能：该函数是等待指定临界区对象的所有权。当调用线程被
  //赋予所有权时，该函数返回。返回值：无。 P
  ```

- 说明:**当调用线程被赋予所有权时，该函数返回。返回值：无。 P**

### 3.释放互斥量函数

**pthread_mutex_unlock** 

- 功能:**释放互斥对象的控制权。**

- 函数原型

  ```c
  int pthread_mutex_unlock (pthread_mutex_t *mutex);//Mutex：信号量
  ```

- 说明:**相当于我们的V操作,返回值：无。V**



### 4.**互斥量的相关函数**





## 四、信号量

[信号量与互斥量的区别](https://blog.csdn.net/qq_34793133/article/details/80087727)

### 1.创建信号量函数

**semget** 

- 功能:**创建一个新的信号量。**

- 函数原型

  ```c
  int semget(key_t key,			//第一个参数key——用于定义信号量的安全特性
             int nsems,			//第二个参数num_sems指定需要的信号量数目，1 
             int semflg);		//第三个参数sem_flags是一组标志
  ```

- 返回值:**semget()函数成功返回一个相应信号标识符（非零），失败返回-1.**



### 2.释放信号量函数

**ReleaseSemaphore**

- 功能:**函数用于对指定的信号量增加指定的值。**

- 函数原型

  ```c
  BOOL ReleaseSemaphore(
         HANDLE     hSemaphore,				//所要释放的信号量句柄。
  　　   LONG        lReleaseCount,		//所要释放信号量的数目。
  　　   LPLONG     lpPreviousCount);	//指向返回信号量上次值的变量的指针，如果不需要信号量上次的值，那么这个参数可以设置为NULL。
  
  ```

- 说明:**相当于我们的V操作**

### 3.等待同步对象函数

**sem_wait**

- 功能:**减小由sem_id指定的信号量的值.**

- 函数原型

  ```c
  int sem_wait(sem_t *sem); 
  ```

- 注:**相当于我们的P操作**





## 五、管程