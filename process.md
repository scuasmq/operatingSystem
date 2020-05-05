# process

[TOC]

## 零 - 概览

<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502001014109.png" alt="image-20200502001014109" style="zoom:50%;" />

## 一 - 进程属性

<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502001111931.png" alt="image-20200502001111931" style="zoom:50%;" />

## 二 - 进程创建

<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502001142208.png" alt="image-20200502001142208" style="zoom:50%;" />

- 系统加电启动后，系统中只有一个进程——<font color = #ff0000>初始化进程</font>>，又称<font color = #ff0000>init进程</font>>，是所有进程的祖先进程，它的进程<font color = #ff0000>pid=1</font>
- 在linux中除了init进程是由系统启动时创建的外，其他所有进程都是由当前进程使用系统调用<font color = #ff0000>fork()</font>>创建的。
- 进程创建后父子进程在系统中<font color = #ff0000>并发</font>执行。

## 三 - 进程结构

- 同时运行grep程序，在不同文件中查找不同的字符串

  <img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502001507724.png" alt="image-20200502001507724" style="zoom:50%;" />

## 四 - fork()

```c
#include <unistd.h>
pid_t fork( );
```

- 此时一个进程“分裂”成两个进程：父进程和子进程。
- 区别：进程ID。利用getpid（）来得到进程号。
- 通过fork创建一个新进程，系统复制当前进程，在进程表中创建一个新的表项
- 新进程几乎与原进程相同，执行代码也相同
- 但有自己的数据空间，环境，文件描述符等

<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502001730156.png" alt="image-20200502001730156" style="zoom:50%;" />

- fork调用的一个奇妙之处就是它仅仅被调用一次，却能够返回两次，它可能有三种不同的返回值：

  - 父进程:返回子进程的PID

  - 子进程:返回0

  - 出现错误:返回-1

    <img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502001805096.png" alt="image-20200502001805096" style="zoom:50%;" />

  <img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502001902767.png" alt="image-20200502001902767" style="zoom:50%;" />

  - 这样的程序会创建七个子进程。<img src="/Users/asmqsaccount/Library/Application Support/typora-user-images/image-20200502002008735.png" alt="image-20200502002008735" style="zoom: 45%;" />

  

## 五 - wait()

当用fork启动一个子进程时，子进程就有了它自己的生命周期并将独立运行。有时候我们希望知道一个进程何时结束。可以通过在父进程中调用wait函数控制

```c
#include<sys/types.h>
#include<sys/wait.h>
pid_t wait(int *stat_loc)
```

- 父进程调用
- wait系统调用将暂停父进程直到它的子进程结束为止
- 返回子进程的PID
- 状态信息允许父进程了解子进程的退出状态，即子进程main函数返回值或子进程中exit函数的退出码
- 若stat_loc不为空指针，状态信息被写入他指定的位置

## 六 - exec函数族

- 父进程创建子进程后,子进程一般要执行不同的程序.为了调用系统程序,我们可以使用系统调用exec 族调用。

- Exec函数可以把当前进程替换为一个新进程

- exec函数包含以下五个函数：

  ```c
  #include<unistd.h>
  int execl(constchar* path, const char* arg, …);
  int execlp(constchar* file, const char* arg, …);
  int execle(constchar* path, const char* arg, char* const envp[]);
  int execv(constchar* path, char* const argv[]);
  int execvp(constchar* file, char* const argv[]);
  ```

- 其中只有execve是真正意义上的系统调用，其它都是在此基础上经过包装的库函数。

  ```c
  #include<unistd.h>
  Int execve(constchar* path, 		//可执行文件路径。
             char* const argv[], 	//要执行的文件名或命令名。
             char* const envp[]);	//环境变量，可省略
  ```

要加另外的可执行程序。

## 七 - 孤儿进程与僵尸进程

### 孤儿进程

- 一个父进程退出，而它的一个或多个子进程还在运行，那么那些子进程将成为孤儿进程。孤儿进程将被init进程(进程号为1)所收养，并由init进程对它们完成状态收集工作。

### 僵尸进程

- 一个进程使用fork创建子进程，如果子进程退出，而父进程并没有调用wait或waitpid获取子进程的状态信息，那么子进程的进程描述符仍然保存在系统中。这种进程称之为僵尸进程。

## 八 - 创建线程与进程区别

- fork():当前进程的拷贝，有自己的变量和PID，时间调度独立，执行几乎完全独立于父进程

- pthread_create():
  - 有自己独立栈（有局部变量）
  - 与创建者共享全局变量，文件描述，信号句柄和当前状态等

