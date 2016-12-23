---
layout: article
title:  "用ptrace实现一个小型调试器"
categories: linux
---

> 本文章主要介绍linux下的系统调用ptrace，以及他如何去实现一个小型调试器

{% include toc.html %}

## PTRACE
这个系统调用在Pradeep Padala在linux journal上的两篇博文[《Playing with ptrace》](http://www.linuxjournal.com/article/6100?page=0,0)发表后开始被大家熟知，文章中用各种例子介绍了ptrace如何查看和修改进程的内存映像。后来ptrace还被安全人员应用于注入+API Hook，感兴趣的同学可以看看古河在看雪论坛发表的[文章](http://bbs.pediy.com/showthread.php?t=141355)。


简单来说，ptrace提供了一种使进程得以监视和控制其它进程的方式，它还能够改变其他进程中的寄存器和内核映像，因此其主要用来实现断点调试和系统调用的跟踪。



### 在内核如何实现
在介绍ptrace如何实现之前，先来看看下面5个linux的进程状态：

1. R (TASK_RUNNING)，可执行状态&运行状态(在run_queue队列里的状态)
2. S (TASK_INTERRUPTIBLE)，可中断的睡眠状态, 可处理signal
3. D (TASK_UNINTERRUPTIBLE)，不可中断的睡眠状态,　可处理signal,　有延迟
4. T (TASK_STOPPED or TASK_TRACED)，暂停状态或**跟踪状态**,　不可处理signal,　因为根本没有时间片运行代码
5. Z (TASK_DEAD - EXIT_ZOMBIE)，退出状态，进程成为僵尸进程。不可被kill,　即不响应任务信号,　无法用SIGKILL杀死

其中第4条的TASK_TRACED，就是ptrace所需要的：

![ptrace impl]({{ site.url }}/images/linux/talk-about-ptrace/ptrace-impl.png)

在执行系统调用之前，内核会先检查当前进程是否处于被“跟踪”(traced)的状态。如果是的话，内核暂停当前进程并将控制权交给跟踪进程，使跟踪进程得以察看或者修改被跟踪进程的寄存器。

### 如何使用
* 原型

```c
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
```

* request参数选项

PTRACE_TRACEME：表示本进程将被其父进程跟踪，交付给这个进程的所有信号（除SIGKILL之外），都将使其停止，父进程将通过wait()获知这一情况。

PTRACE_ATTACH：attach到一个指定的进程，使其成为当前进程跟踪的子进程，子进程的行为等同于它进行了一次PTRACE_TRACEME操作。

PTRACE_CONT：继续运行之前停止的子进程。可同时向子进程交付指定的信号。

PTRACE_PEEKTEXT, PTRACE_PEEKDATA：读取被追踪者由addr指向的内存一个字长的内容

PTRACE_PEEKUSER：读取被追踪者停止时保存的寄存器内容

PTRACE_POKETEXT, PTRACE_POKEDATA：拷贝一字长的data到被追踪者addr指向的内存处

PTRACE_POKEUSER：拷贝一字长的data到被追踪者保存寄存器的内存地址处（被追踪者恢复时会有新的寄存器值）

PTRACE_GETREGS，PTRACE_SETREGS：比PEEKUSER和POKEUSER更方便的获取和设置寄存器，保存到结构体中

PTRACE_SYSCALL和PTRACE_SINGLESTEP：追踪下一个系统调用或指令

> 更详细的内容请看[man手册](http://man7.org/linux/man-pages/man2/ptrace.2.html)

知道上面这些ptrace的选项后，下面我们就可以实现一个简单的调试器了。

## 实现一个小型调试器
	
### 设置断点b
将目标地址处的指令的第一个字节设置为断点指令（0xcc）

![set breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/set-breakpoint.png)

### 继续运行c
删除断点并执行ptrace(PTRACE_CONT,...):

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/continue.png)

### 单步指令si

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/singlestep.png)

### 打印变量(p / print)
ptrace(PEEKDATA…)


### 查看信息 (i / info)
ptrace(GETREGS…)

> [实现代码](https://github.com/yyff/ydb)

## 演示
调试下面程序的汇编码：

hello.c:

```c
#include <stdio.h>
int main(){
	printf("hello\n");
	return 0;
}
```
hello.s:

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/assembly-code.png)

调试界面：

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/demo-show.png)


## 扩展
如何让调试器能调试c语言？需要引入一种调试信息的标准格式--DWARF，其存在在unix的可执行文件（ELF）的section中。DWARF is a widely used, standardized debugging data format. DWARF was originally designed along with Executable and Linkable Format (ELF)--[维基百科](https://en.wikipedia.org/wiki/Dwarf)。

### DWARF
elf文件中的调试信息section：

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/dwarf1.png)

符号名和对应的地址：objdump --dwarf=info dwarf_test

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/dwarf2.png)

行号信息：objdump --dwarf=decodedline dwarf_test
![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/dwarf2.png)

## Reference

1. [Playing with ptrace, Part I](http://www.linuxjournal.com/article/6100?page=0,0)
2. [Playing with ptrace, Part I](http://www.linuxjournal.com/article/6210)
3. [How debuggers work: Part 1 – Basics](http://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1)
4. [How debuggers work: Part 2 - Breakpoints](http://eli.thegreenplace.net/2011/01/27/how-debuggers-work-part-2-breakpoints)
5. [How debuggers work: Part 3 - Debugging information](http://eli.thegreenplace.net/2011/02/07/how-debuggers-work-part-3-debugging-information)


