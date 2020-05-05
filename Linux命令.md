## apt

Advanced Package Tool，又名apt-get，是一款适用于[Unix](https://baike.baidu.com/item/Unix)和[Linux](https://baike.baidu.com/item/Linux)系统的应用程序管理器。

#### 更新

```
apt-get update
```

下载

```
apt-get install <packagename>
```

  

## rm

#### 删除文件

```bash
rm -r <filename>
```

#### 删除当前目录下的所有文件及目录，命令行为：

```bash
rm -r *
```



## unzip

解压到指定文件夹

```
unzip <filename.zip> -d <dicname>
```



## 作业、进程

终止并退出前台命令的执行，回到SHELL

```
Ctrl+C
```



暂停前台命令的执行，将该进程放入后台，回到SHELL

```
Ctrl+Z
```



查看当前在后台执行的命令，可查看命令进程号码

```
jobs
```



运行命令时，在命令末尾加上&可让命令在后台执行

```
&
```



将命令进程号码为N的命令进程放到前台执行，同%N

```
fg N
%N
```



将命令进程号码为N的命令进程放到后台执行

```
bg N
```



杀死进程

```bash
kill <pid>

#强制杀死进程
kill -KILL <pid>

#杀死指定用户所有进程
kill -9 $(ps -ef | grep <usrname>) //方法一 过滤出hnlinux用户进程 
kill -u <usrname> //方法二
```



## 编译

gcc

```bash
gcc -o <outfile> <filename>
```



[linux](https://www.runoob.com/linux/linux-shell.html)

[命令缩写及全称](https://www.cnblogs.com/summersoft/p/7589189.html)

