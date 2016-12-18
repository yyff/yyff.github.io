---
layout: article
title:  "c++ primer读书笔记--第一章"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

## 1.编译执行
C++ 程序文件的后缀与运行的具体编译器有关。其他的形式还包括。
 prog1.cxx
 prog1.cpp
 prog1.cp
 prog1.C
 
**调用 GNU 或微软编译器**
调用 C++ 编译器的命令因编译器和操作系统的不同而不同， 常用的编译
器是 GNU 编译器和微软 Visual Studio 编译器。调用 GNU 编译器的默
认命令是 g++：


     $ g++ prog1.cc -o prog1


这里的 $ 是系统提示符。 这个命令产生一个为 prog1 或 prog1.exe 的
可执行文件。 在 UNIX 系统下， 可执行文件没有后缀； 而在 Windows 下，后缀为 .exe。-o prog1 是编译器参数以及用来存放可执行文件的文件
名。 如果省略 -o prog1， 那么编译器在 UNIX 系统下产生名为 a.out 而
在 Windows 下产生名为 a.exe 的可执行文件。

**访问 main 函数的返回值**
访问 main 函数的返回值的方式和系统有关。不论 UNIX 还是 Windows 系
统，执行程序后，必须发出一个适当的 echo 命令。UNIX 系统中，通过键入如
下命令获取状态：

    $ echo $?


要在 Windows 系统下查看状态，键入


    C:\directory> echo %ERRORLEVEL%

Exercise
1.1:
查看所用的编译器文档，了解它所用的文件命名规范。编
译并运行本节的 main 程序。

代码:


    int main(){
    	return 1;
    }

命令行操作和显示:

    C:\Users\yangfan>d:
    
    D:\>cd c++
    
    D:\C++>g++ firstcpp.cpp -o firstcpp
    
    D:\C++>firstcpp.exe
    
    D:\C++>echo %ERRORLEVEL%
    1

##  2.初窥输入/输出
书的大多数例子都使用了处理格式化输入和输出的 iostream 库。iostream 库的基础是两种命名为 istream 和 ostream 的类型，分别表示输入流和输出流。**流指要从某种 IO 设备上读入或写出的字符序列。**术语“流”试图说明字符是随着时间顺序生成或消耗的。

标准库定义了 4 个 IO 对象。处理输入时使用命名为 cin（读作 see-in）的 istream 类型对象。这个对象也称为标准输入。处理输出时使用命名为 cout（读作 see-out）的 ostream 类型对象，这个对象也称为标准输出。标准库还定义了另外两个 ostream 对象，分别命名为 cerr 和 clog（分别读作“see-err”和“see-log”）。**cerr 对象又叫作标准错误，通常用来输出警告****和错误信息给程序的使用者**。**而 clog 对象用于产生程序执行的一般信息。**
一般情况下，系统将这些对象与执行程序的窗口联系起来。这样，当我们从cin 读入时，数据从执行程序的窗口读入，当写到 cout、cerr 或 clog 时，输出写至同一窗口。运行程序时，大部分操作系统都提供了重定向输入或输出流的方法。利用重定向可以将这些流与所选择的文件联系起来。 

    
    #include <iostream>
     int main()
     {
     std::cout << "Enter two numbers:" << std::endl;
     int v1, v2;
     std::cin >> v1 >> v2;
     std::cout << "The sum of " << v1 << " and " << v2
     << " is " << v1 + v2 << std::endl;
     return 0;
     }

这个程序中使用的是 std::cout 和 std::endl，而不是 cout 和 endl。前缀 std:: 表明 cout 和 endl 是定义在命名空间 std 中的。使用命名空间程序员可以避免与库中定义的名字相同而引起无意冲突。因为标准库定义的名字是定义在命名空间中， 所以我们可以按自己的意图使用相同的名字。**std::cout 的写法使用了作用域操作符（ scope operator，:: 操作符）**，**表示使用的是定义在命名空间 std 中的cout。**

**<< operator（<< 操作符）** 
**输出操作符**。 把右操作数写到左操作数指定的输出流： cout << "hi" 把 hi
写入到标准输出流。输出操作可以链接在一起使用：cout << "hi << "bye"
输出 hibye。
**>> operator（>> 操作符）** 
**输入操作符**。从左操作数指定的输入流读入数据到右操作数：cin >> i 把
标准输入流中的下一个值读入到 i 中。输入操作能够链接在一起使用：
cin >> i >> j 先读入 i 然后再读入 j。


### 2.1读入未知数目的输入
我们不知道要对多少个数求和，而是要一直读数直到程序输入结束。


    while (std::cin >> value)
     sum += value; // equivalent to sum = sum + value

    
**从键盘输入文件结束符**
操作系统使用不同的值作为文件结束符。Windows 系统下我们通过键入control+z——同时键入“ctrl”键和“z”键，来输入文件结束符。Unix 系统中，包括 Mac OS—X 机器，通常用 control+d。

## 3类的简介
类机制是 C++ 中最重要的特征之一。事实上， C++ 设计的主要焦点就是使所定义的类类型的行为可以像内置类型一样自然。

### 3.1头文件的引用

    #include <iostream>
    #include "Sales_item.h"

标准库的头文件用尖括号 < > 括起来，非标准库的头文件用双引号 " " 括起来。
 

**大型项目中头文件要加上判断,避免重复定义**

    #ifdef _TEST_H 
    #define _TEST_H//一般是文件名的大写 头文件结尾写上一行： 
    #endif 

这样一个工程文件里同时包含两个test.h时，就不会出现重定义的错误了。 **分析：当第一次包含test.h时，由于没有定义_TEST_H，条件为真，这样就会包含（执行）#ifndef _TEST_H和 #endif之间的代码，当第二次包含test.h时前面一次已经定义了_TEST_H，条件为假，#ifndef _TEST_H和 #endif之间的代码也就不会再次被包含，这样就避免了重定义了。**

### 3.2什么是成员函数


    item1.same_isbn(item2)

调用命名为 item1 的 Sales_item 对象的成员函数。成员函数是由类定义
的函数，有时称为类方法。
当**调用成员函数**时，（通常）指定函数要操作的对象。**语法是使用点操作符（.）。**如 item1.same_isbn
执行成员函数和执行其他函数相似：要调用函数，可将**调用操作符（()）**放在函数名之后。调用操作符是一对圆括号，括住传递给函数的实参列表（可能为空）。如 item1.same_isbn(item2)
