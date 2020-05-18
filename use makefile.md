一个Makefile编译链接多个.c或.cpp文件，生成多个可执行程序

```makefile
CC = g++
C_FLAGS = -g -Wall
INCLUDE =
LIBS =
#wildcard对cpp文件进行展开
SRCS := $(wildcard *.cpp)
#将SRCS中后缀.cpp替换成空，即可执行程序的名字与cpp名字相同
TARGET  := $(patsubst %.cpp, %, $(SRCS))
OBJDIR = .

.cpp:
   $(CC) $(C_FLAGS) -o $@ $< $(INCLUDE) $(LIBS)
   #可移动到指定的OBJDIR目录
   #mv $@ $(OBJDIR)
all: $(TARGET) clean
clean:
  rm -rf *.o
```



## 函数

```makefile
文件结构
mk/
	a.c
	b.c
	sub/
		aa.c
		bb.c

---------wildcard---------
wildcard把 指定目录下的所有后缀是c的文件全部展开
e.g.
src=$(wildcard *.c ./sub/*.c)
all:
	@echo $(src)
a.c b.c ./sub/aa.c ./sub/bb.c

---------notdir---------
notdir把展开的文件去除掉路径信息
file=$(notdir $(src))
all:
	@echo $(file)
a.c b.c aa.c bb.c


---------patsubst---------
模式字符串替换函数——patsubst。
$(patsubst <pattern>,<replacement>,<text>)
查找<text>中的单词（单词以“空格”、“Tab”或“回车”“换行”分隔）是否符合模式<pattern>，如果匹配的话，则以<replacement>替换。
这里，<pattern>可以包括通配符“%”，表示任意长度的字串。如果<replacement>中也包含“%”，那么，<replacement>中的这个“%”将是<pattern>中的那个“%”所代表的字串。

$(patsubst %.c,%.o, a.c b.c)
把字串“a.c b.c”符合模式[%.c]的单词替换成[%.o]，返回结果是“a.o b.o”

obj=$( patsubst %.c,%.o,$(src) )
all:
	@echo $(boj)
a.o b.o aa.o bb.o
也可以使用
obj=$(file:%.c=%.o)

```

## 变量


```makefile
SRCS := $(wildcard *.cpp),表示获取当前目录下的cpp文件集，放在变量SRCS中

TARGET  := $(patsubst %.cpp, %, $(SRCS)),表示将对应的cpp文件名转为无后缀文件后放在下面的OBJS变量中，可以自己定义可执行文件后缀

Makefile有三个非常有用的变量。分别是$@，$^，$<代表的意义分别是： 
$@--目标文件，$^--所有的依赖文件，$<--第一个依赖文件

```


## 缺省

```makefile
main：main.o mytool1.o mytool2.o 
	gcc -o main main.o mytool1.o mytool2.o

main.o：main.c mytool1.h mytool2.h
	gcc -c main.c

mytool1.o：mytool1.c mytool1.h
	gcc -c mytool1.c

mytool2.o：mytool2.c mytool2.h 
	gcc -c mytool2.c
	
---------------------------------------------------------------
main：main.o mytool1.o mytool2.o 
	gcc -o $@ $^ 

main.o：main.c mytool1.h mytool2.h 
	gcc -c $< 

mytool1.o：mytool1.c mytool1.h 
	gcc -c $< 

mytool2.o：mytool2.c mytool2.h 
	gcc -c $< 
	
---------------------------------------------------------------
缺省规则
.c.o： 
gcc -c $<

这个规则表示所有的 .o文件都是依赖与相应的.c文件的。例如mytool.o依赖于mytool.c
这样Makefile还可以变为

main：main.o mytool1.o mytool2.o 
	gcc -o $@ $^ 
.c.o： 
	gcc -c $< 

```





```makefile
deq: deq.c job.h
	gcc -o deq deq.c

enq: enq.c job.h
	gcc -o enq -c  enq.c

stat: stat.c job.h
	gcc -o stat stat.c

scheduler: scheduler.c job.h
	gcc -o scheduler  scheduler.c

clean:
	rm out deq enq scheduler stat
```



## 模版

```makefile
CC = gcc
CFLAGS = 
#-g -Wall
INCLUDE = 
LIBS = 

SRCS := $(wildcard *.c)
TARGET := $(patsubst %.c,%,$(SRCS))
OBJDIR = .

.:#这里的.我感觉是缺省吧，不是很清楚
	$(CC) $(CFLAGS) -o $@ $< $(INCLUDE) $(LIBS)
	#可移动到指定的OBJDIR目录
	#mv $@ $(OBJDIR)

all: $(TARGET) clean
clean:
	rm -rf *.o
```



## 伪目标

```
.PHONY告诉make该目标是“假的”（磁盘上其实没有clean），这时make为生成这个目标就会将其规则执行一次。.PHONY修饰的目标就是只有规则没有依赖。

all:由于makefile只能有一个目标，所以可以构造一个没有规则的终极目标all，并以这两个可执行文件作为依赖。
```

