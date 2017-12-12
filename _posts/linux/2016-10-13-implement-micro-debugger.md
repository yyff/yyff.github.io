---
layout: article
title:  "实现一个微型调试器"
categories: linux
excerpt: "本文章主要介绍linux下的系统调用ptrace，以及他如何去实现一个小型调试器"
---

> 文章欢迎转载，但转载时请保留本段文字，并置于文章的顶部 作者：c0ks 本文原文地址：http://yyff.github.io/linux/implement-micro-debugger/

{% include toc.html %}

# 实现一个微型调试器

## 0x1 背景
在了解GDB实现的过程中，本人在网上发现（by Google）了一篇写得很好的文章：“How debuggers work”，作者是Eli Bendersky。文章由三部分组成，第一部分介绍了ptrace系统调用（Linux下的调试器实现最核心的元素）并使用它编写了demo：父进程单步跟踪子进程并打印子进程的eip寄存器内容，其中用到了`PTRACE_TRACEME`、`PTRACE_SINGLESTEP `和`PTRACE_PEEKTEXT`这三个ptrace请求类型（ptrace函数的第一个参数，代表ptrace调用的行为）；第二部分介绍了断点的实现：通过将被追踪进程（tracee）的断点处指令替换成`int 3`使其执行到断点处时进入`STOP`状态，并且其会向追踪者（父进程）发送`SIGTRAP`信号。第三部分介绍了存在Linux可执行文件（ELF格式）中的调试信息。因为前两部分介绍的调试只是针对于汇编/机器指令的，想要调试C/C++代码，需要用到存在于可执行文件中的调式信息。这些信息按照一定的格式（DWARF）存储并支持通过函数获取，信息记录了高层语言到汇编/机器语言的映射，比如：c程序代码的行号、函数、语句对应的汇编指令位置，变量所在的内存位置等[1][2][3]。

看完这个系列的文章后，基本就了解了实现GDB所需的技术。但毕竟是“纸上得来”，因此本人也动手实践了一个可调试汇编代码的微型调试器来加深理解，下文将介绍该微型调试器的实现。



## 0x2 相关技术

### 2.1. linux进程状态

linux进程主要有以下5种常见的状态，执行ps或top命令可在S/STATE一列看到这些状态[4]。

进程状态 | 描述
--- | ---
R（运行）| 正在运行或可运行状态（处在运行队列中）
S（可中断睡眠） | 可中断的睡眠状态，接收到信号可提前唤醒
D（不可中断睡眠） | 不可中断的睡眠状态，接收到信号（比如SIGKILL）也不会被唤醒 （p.s.此处“不可中断”指的并不是进程不响应外部硬件的中断，而是指进程不响应异步信号）
Z(僵尸) | 进程结束但未被父进程回收（wait/waitpid）
T（暂停）| 进程停止执行，收到SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU等信号以及被ptrace跟踪时进入此状态[5]

进程的状态状态图如图2.1所示[4]：

![linux-state-transform]({{ site.url }}/images/linux/talk-about-ptrace/linux-state-transform.png)



### 2.2. ptrace系统调用
#### 简介
ptrace是unix或类unix操作系统（比如Linux、OSX等）下的一个系统调用，它使进程得以监视或操作其它进程（比如改变进程的寄存器、内存、进程状态等），通常被调试器（debugger）或其他的代码分析工具所使用。

#### 用法
函数原型：

```c
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
```

通过改变ptrace第一个变量request不同的常量值，其拥有不同的行为，以下是本文实现的的调试器中使用的几种类型（ 参考[6]）：

```
PTRACE_TRACEME：表示本进程将被其父进程跟踪，交付给这个进程的所有信号（除SIGKILL之外），都将使其停止，父进程将通过wait()获知这一情况。

PTRACE_ATTACH：attach到一个指定的进程，使其成为当前进程跟踪的子进程，子进程的行为等同于它进行了一次PTRACETRACEME操作。

PTRACE_CONT：继续运行之前停止的子进程。可同时向子进程交付指定的信号。

PTRACE_PEEKTEXT, PTRACE_PEEKDATA：读取被追踪者由addr指向的内存一个字长的内容

PTRACE_PEEKUSER：读取被追踪者停止时保存的寄存器内容

PTRACE_POKETEXT, PTRACEPOKEDATA：拷贝一字长的data到被追踪者addr指向的内存处

PTRACE_POKEUSER：拷贝一字长的data到被追踪者保存寄存器的内存地址处（被追踪者恢复时会有新的寄存器值）

PTRACE_GETREGS，PTRACE_SETREGS：比PEEKUSER和POKEUSER更方便的获取和设置寄存器，保存到结构体中

PTRACE_SYSCALL和PTRACE_SINGLESTEP：追踪下一个系统调用或指令
```

### 2.3 断点中断（int 0x3 / 0xcc）
在linux内核的中断描述符表（Interrupt descriptor table），有一个专门用于中断程序执行的中断：断点中断，其中断号为3(或0x3, 代表其在表中的索引)。对于中断描述符表中中断的执行方式，主要有硬中断（硬件引起，cpu引脚变化）和软中断（软件执行中断指令int）两种方式，而对调试器（软件）而言，显然需要使用后者使调试进程执行断点指令。

断点中断可通过汇编指令“int 0x3;”执行，其生成的x86机器指令为0xcc。和其它中断指令为双字节指令（int 0x80, int 0x0等汇编指令对应的机器指令）不同[7]，这是一条单字节指令。这个特点使它在覆盖其它指令时至多只能覆盖一条指令。


## 0x3 实现

参考gdb用法，本程序实现了常用的调试命令，包括：开始运行（r/run），单步（si/ni），设置断点（b/breakpoint），继续运行（c/continue），打印（p/print）和查看信息（i/info）。

> 完整程序见：[https://github.com/yyff/ydb](https://github.com/yyff/ydb)

### 3.1. 调试器类接口设计

```c++
/* 
usage：
	Debugger debugger(program_name);
	debugger.run();
*/

class Debugger{
public:
    Debugger(const char* name); // 被调试的二进制程序文件名
    int run(); // 运行调试器，接收命令并处理
private:
	//...
};
```

### 3.2. 启动调试器 Debugger::run()
该方法的功能为：同时启动（fork & exec）被调试程序和调试器程序：使被调试进程处于暂停状态（也就是T状态，见2.1），等待来自调试进程的操作；调试器进程等待接收来自标准输入的命令，解析后执行命令操作被调试进程。该过程的伪代码如下：

```cpp
	
child_pid = fork();
if (child_pid == 0) { 			// user program, tracee.
	ptrace(PTRACE_TRACEME...) 	// let child process go into "T" state
	execl(program_file...)		// execute user program 
}
// debugger, tracer.
wait(...); 				// wait for signal from tracee 
while (1) {
	get_command(); 		// get input from stdin and parse it
	handle_command(); 	// handle command to operate tracee
}

```


### 3.3. 命令实现


#### 设置断点(b / break)

在gdb中，可以在指定行或函数处设置断点，但在它设置之前需要将指定行或函数映射到对应的汇编/机器指令所在地址，这个映射信息是通过一种特殊的调试信息格式`dwarf`记录在可执行程序（linux下是elf文件格式）中的。
但本文的调试器并不大打算涉及这方面的知识，其断点功能只能对指定的（虚拟）地址进行使用。这样虽然简陋，但其却基本描述了调试器断点最核心的实现机制。

在本文的2.3节已经介绍了断点指令，进程执行该指令后将进入`T`状态并向父进程/调试进程（Tracer）发送`SIGTRAP`信号[2]。

由此可见，"给指定地址设置断点"命令可以转化为：让被调试进程tracee在指定地址处执行断点指令0xcc，即讲被调试进程tracee指定地址的指令替换为0xcc。而对tracee进程执行`写`操作，则可以通过ptrace的选项`PTRACE_POKETEXT`实现。

但还有很重要的一点是：替换前需要将断点处被替换内容保存起来，使进程从断点恢复时可以恢复这条被断点覆盖的指令并执行。而对tracee进程执行`读`操作，则可以通过ptrace的选项`PTRACE_PEEKTEXT`实现。


设置断点方法`set_breakpoint()`的伪代码如下：

```cpp
// set breakpoint in addr
data = ptrace(PTRACE_PEEKTEXT, addr...); // get data from addr
ptrace(PTRACE_POKETEXT, addr, 0xcc...);  // cover data in addr with instr '0xcc'
save_data(addr, data);   // save pair "addr-data" 
```

#### 继续运行(c / continue)

使进程从开始处或断点处继续执行，通过ptrace请求(参数1 request)`PTRACE_CONT`实现：该方法先执行`ptrace(PTRACE_CONT,...)`使被追踪者tracee继续运行，直到遇到断点或程序结束。若运行到断点处，追踪者tracer还需将断点指令（0xcc）恢复成原始指令。

> 这里实际上还有另外一种实现方式：先判断前一字节是否为断点指令并恢复，后执行ptrace(PTRACE_CONT,...)


继续运行方法`continue_run()`的伪代码如下：

```cpp
ptrace(PTRACE_CONT, child_pid, ...);
waitpid(child_pid...); // wait for signal: SIG_EXIT or SIG_TRAP...
if (signal == SIG_EXIT) {
    exit();
}
    
if (has_breakpoint()) { 
    disable_breakpoint(); // recover from breakpoint
}
```

#### 单步指令ni

该指令可以直接通过ptrace请求`PTRACE_SINGLESTEP`实现，但为了观察到程序执行的指令码，本程序对该功能进行了扩展。

单步指令方法`exec_next_instr()`的伪代码如下：

```
if(has_breakpoint()) {
    disable_breakpoint();
}
long orig_eip = get_eip(); // get current instr address of tracee process 
instr = ptrace(PTRACE_PEEKTEXT, child_pid, orig_eip...); // get instr
ptrace(PTRACE_SINGLESTEP, child_pid...);  // execute single step
print(instr);	// print executed instr    
```

#### 打印值(p / print)


通过ptrace请求`PEEKDATA`实现了打印指定地址出的值的功能。


打印值方法`print_var()`的伪代码为：

```cpp
data = ptrace(PTRACE_PEEKTEXT, child_pid, addr...);
```


#### 查看信息 (i / info)

通过ptrace请求`PTRACE_GETREGS `实现了查看寄存器值的功能。


打印寄存器信息方法`print_regs()`的伪代码为：

```cpp
ptrace(PTRACE_GETREGS, child_pid, &regs...);
print regs.eax, regs.ebx, regs.ecx, regs.eip...
```    



## 0x4 演示
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


## 0x5 扩展
如何让调试器能调试c语言？需要引入一种调试信息的标准格式--DWARF，其存在在unix的可执行文件（ELF）的section中。DWARF is a widely used, standardized debugging data format. DWARF was originally designed along with Executable and Linkable Format (ELF)--[维基百科](https://en.wikipedia.org/wiki/Dwarf)。

### DWARF
elf文件中的调试信息section：

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/dwarf1.png)

符号名和对应的地址：objdump --dwarf=info dwarf_test

![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/dwarf2.png)

行号信息：objdump --dwarf=decodedline dwarf_test
![delete breakpoint]({{ site.url }}/images/linux/talk-about-ptrace/dwarf2.png)

## 0x6 Reference

1. [How debuggers work: Part 1 – Basics](http://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1)
2. [How debuggers work: Part 2 - Breakpoints](http://eli.thegreenplace.net/2011/01/27/how-debuggers-work-part-2-breakpoints)
3. [How debuggers work: Part 3 - Debugging information](http://eli.thegreenplace.net/2011/02/07/how-debuggers-work-part-3-debugging-information)
4. [Linux process states](https://idea.popcount.org/2012-12-11-linux-process-states/)
5. Robert Love. 《Linux内核设计与实现(原书第3版)》p24
6. [ Linux Programmer's Manual--ptrace](http://man7.org/linux/man-pages/man2/ptrace.2.html)
7. [Wikipedia: INT (x86 instruction)](https://en.wikipedia.org/wiki/INT_(x86_instruction))


