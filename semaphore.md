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
  int pthread_mutex_init(pthread_mutex_t *restrict mutex,						//第一个参数初始化的互斥量的指针；（&mylock）
                          const pthread_mutexattr_t *restrict attr);	//第二个参数attr：指定了新建互斥锁的属性。如果参数attr为NULL，则使用默认的互斥锁属性，默认属性为快速互斥锁 
  ```

- 返回值

  **函数成功执行后，返回0，互斥锁被初始化为未锁住态**

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

//信号量集

### 2.释放信号量函数

**ReleaseSemaphore**//这是wiindows的吧..刘璐ppt好像有点问题

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



### 0.信号量

包含于头文件`#include <semaphore.h>`

### 1.变量

```c
sem_t sem;
```

### 2.信号量初始化sem_init()

- 功能：该函数初始化由 sem 指向的信号对象，并给它一个初始的整数值 value。

- 函数原型

  ```c
  int sem_init(sem_t *sem,	//第一个参数为信号量指针
               int pshared, //第二个参数shared 控制信号量的类型，值为0代表该信号量用于多线程间的同步，值如果大于0表示可以共享，用于多个相关进程间的同步
               //参数 pshared > 0 时指定了 sem 处于共享内存区域，所以可以在进程间共享该变量
               unsigned int value);//第三个参数为信号量值
  ```

- 返回值：sem_init() 成功时返回 0；错误时，返回 -1，并把 errno 设置为合适的值。

### 3.阻塞信号量函数sem_wait()

- 功能：sem_wait 是一个阻塞的函数，测试所指定信号量的值，它的操作是原子的。若 sem value > 0，则该信号量值减去 1 并立即返回。若sem value = 0，则阻塞直到 sem value > 0，此时立即减去 1，然后返回。

- sem_trywait 函数是非阻塞的函数，它会尝试获取获取 sem value 值，如果 sem value = 0，不是阻塞住，而是直接返回一个错误 EAGAIN。

- 函数原型

  ```c
  int sem_wait(sem_t *sem); 
  
  int sem_trywait(sem_t *sem);
  ```

- 相当于P操作



### 4.释放信号量函数sem_post()

- 功能：把指定的信号量 sem 的值加 1，唤醒正在等待该信号量的任意线程。

- 函数原型

  ```c
  int sem_post(sem_t *sem);
  ```

### 5.获取信号量值函数sem_getvalue()

- 功能：获取信号量 sem 的当前值，把该值保存在 sval，若有 1 个或者多个线程正在调用 sem_wait 阻塞在该信号量上，该函数返回阻塞在该信号量上进程或线程个数。

- 函数原型

  ```c
  int sem_getvalue(sem_t *sem, int *sval);
  ```

### 6.摧毁信号量函数sem_destroy()

- 功能：该函数用于对用完的信号量的清理。

- 函数原型

  ```c
  int sem_destroy(sem_t *sem);
  ```

### 7.坑

macOS上弃用`sem_init()` `sem_getvalue()` `sem_destory()`

原因[“if Apple implemented POSIX unnamed semaphores, how many X Serves would you buy?”](http://www.voidcn.com/link?url=http://lists.apple.com/archives/darwin-kernel/2009/Apr/msg00010.html)

OS X不支持创建无名的信号量，只能使用sem_open创建有名的信号量

```c
sem_t* psemaphore = sem_open("/mysem",O_CREAT, S_IRUSR | S_IWUSR, 10);

销毁：
  
 sem_close(psemaphore);
 sem_unlink("/mysem");

```



## 五、条件变量

**什么是条件变量**

- **条件变量是利用线程间共享的全局变量进行同步的一种机制。** 
- 主要包括两个动作：一个线程等待”条件变量的条件成立”而挂起；另一个线程使”条件成立”（给出条件成立信号）。
- 为了防止竞争，条件变量的使用总是和一个互斥锁结合在一起。 
- 条件变量类型为 pthread_cond_t。
- 使用条件变量可以以原子方式阻塞线程，直到某个特定条件为真为止。条件变量始终与互斥锁一起使用，**对条件的测试是在互斥锁（互斥）的保护下进行的**。

### 1.创建和注销

#### 静态方式创建

```c
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
```

#### 动态方式

```c
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t   *cond_attr); 
  //成功返回0，失败返回错误码.
```

尽管POSIX(可移植操作系统接口)标准中为条件变量定义了属性，但在LinuxThreads中没有实现，因此`cond_attr`值通常为NULL，且被忽略。

#### 注销

注销一个条件变量需要调用`pthread_cond_destroy()`函数，只有在没有线程在该条件变量上等待的时候才能注销这个条件变量，否则返回EBUSY。因为Linux实现的条件变量没有分配什么资源，所以注销动作只包括检查是否有等待线程。 

```c
int pthread_cond_destroy(pthread_cond_t  *cond) ; 
//成功返回0，失败返回错误码.
```

### 2.等待

两种等待方式，无条件等待`pthread_cond_wait()`和计时等待`pthread_cond_timedwait()`。

```c
int pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex);  
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,const struct timespec   *abstime);
//成功返回0，失败返回错误码.
//计时等待方式如果在给定时刻前条件没有满足，则返回ETIMEOUT，结束等待，
```

- 无论哪种等待方式，都必须和一个互斥锁配合，以防止多个线程同时请求`pthread_cond_wait()`（或`pthread_cond_timedwait()`）的竞争条件（Race Condition）。

- mutex互斥锁必须是普通锁或者适应锁，且在调用`pthread_cond_wait()`前必须由<font color = #af0000>本线程</font>加锁，而在更新条件等待队列以前，mutex保持锁定状态，并在线程挂起进入等待前解锁。
- 在条件满足从而离开`pthread_cond_wait()`之前，mutex将被重新加锁，以与进入`pthread_cond_wait()`前的加锁动作对应。 

注意：<font color = #af0000>`pthread_cond_wait()`函数的返回并不意味着条件的值一定发生了变化，必须重新检查条件的值。</font>

阻塞在条件变量上的线程被唤醒以后，直到pthread_cond_wait()函数返回之前条件的值都有可能发生变化。所以函数返回以后，在锁定相应的互斥锁之前，必须重新测试条件值。最好的测试方法是循环调用pthread_cond_wait函数，并把满足条件的表达式置为循环的终止条件。如：

```c
pthread_mutex_lock();

while (condition_is_false)

    pthread_cond_wait();

pthread_mutex_unlock();
```

阻塞在同一个条件变量上的不同线程被释放的次序是**不一定**的。

<font color = #af0000>(还没懂)</font>注意：pthread_cond_wait()函数是退出点，如果在调用这个函数时，已有一个挂起的退出请求，且线程允许退出，这个线程将被终止并开始执行善后处理函数，而这时和条件变量相关的互斥锁仍将处在锁定状态。

### 3.唤醒

唤醒条件有两种形式，`pthread_cond_signal()`唤醒一个等待该条件的线程，存在多个等待线程时按入队顺序唤醒其中一个；而`pthread_cond_broadcast()`则唤醒所有等待线程。

```c
int pthread_cond_signal(pthread_cond_t *cptr); 
int pthread_cond_broadcast (pthread_cond_t * cptr);
//成功返回0，失败返回错误码.
```

- 必须在互斥锁的保护下使用相应的条件变量。否则对条件变量的解锁有可能发生在锁定条件变量之前，从而造成死锁。

- 唤醒阻塞在条件变量上的所有线程的顺序由调度策略决定，如果线程的调度策略是SCHED_OTHER类型的，系统将根据线程的优先级唤醒线程。

- 如果没有线程被阻塞在条件变量上，那么调用`pthread_cond_signal()`将没有作用。
- 由于pthread_cond_broadcast函数唤醒所有阻塞在某个条件变量上的线程，这些线程被唤醒后将再次竞争相应的互斥锁，所以必须小心使用`pthread_cond_broadcast`函数。

##### 丢失问题

在线程未获得相应的互斥锁时调用`pthread_cond_signal`或`pthread_cond_broadcast`函数可能会引起唤醒丢失问题。唤醒丢失往往会在下面的情况下发生:

```
一个线程调用pthread_cond_signal或pthread_cond_broadcast函数；
另一个线程正处在测试条件变量和调用pthread_cond_wait函数之间；
没有线程正在处在阻塞等待的状态下。
```

#### 4.实例

条件变量的使用可以分为两部分：

- ##### 等待线程

  使用pthread_cond_wait前要先加锁； 
  pthread_cond_wait内部会解锁，然后等待条件变量被其它线程激活； 
  pthread_cond_wait被激活后会再自动加锁；

- ##### 激活线程：

  加锁（和等待线程用同一个锁）； 
  pthread_cond_signal发送信号； 
  解锁；

[例程](./刘璐/WK8-9/condEg.c)

