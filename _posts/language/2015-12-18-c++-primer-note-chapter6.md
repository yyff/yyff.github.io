---
layout: article
title:  "c++ primer读书笔记--第六章 语句"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

## continue 语句
continue 语句 导致最近的循环语句的**当次迭代提前结束**。

## break  语句
break 语句用于**结束最近的 while、do while、for 或 switch 语句**，并将程序的执行权传递给紧接在被终止语句之后的语句。

## try  块和异常处理
C++ 的异常处理中包括： 
1.  throw 表达式，错误检测部分使用这种表达式来说明遇到了不可处理的错误。可以说，throw 引发了异常条件。 
2.  try 块，错误处理部分使用它来处理异常。try 语句块以 try 关键字开始，并以一个或多个 catch 子句结束。在 try 块中执行的代码所抛出（throw）的异常，通常会被其中一个catch子句处理。由于它们“处理”异常，catch 子句也称为处理代码。 
3.  由标准库定义的一组异常类，用来在 throw 和相应的 catch 之间传递有关的错误信息。 
例如:

   

     if (!item1.same_isbn(item2)) 
        std::cerr << "Data must refer to same ISBN"


改为

    if (!item1.same_isbn(item2)) 
             throw runtime_error("Data must refer to same ISBN");

### try   块 
try 块的通用语法形式是： 

     try { 
         program-statements 
     } catch (exception-specifier) { 
         handler-statements 
     } catch (exception-specifier) { 
         handler-statements 
     } //...

每个 catch 子句包括三部分：关键字 catch，圆括号内单个类型或者单个对象的声明——称为异常说明符，以及通常用花括号括
起来的语句块。
如果不存在处理该异常的 catch 子句，程序的运行就要跳转到名为 terminate  的标准库函数，该函数在 exception 头文件中定义。

实例:

    while (cin >> item1 >> item2) { 
             try { 
                 // execute code that will add the two Sales_items 
                 // if the addition fails, the code throws a runtime_error 
    exception 
             } catch (runtime_error err) { 
                 // remind the user that ISBN must match and prompt for 
    another pair 
                 cout << err.what() 
                      << "\nTry Again? Enter y or n" << endl; 
                 char c; 
                 cin >> c; 
                 if (cin && c == 'n') 
                     break;     // break out of the while loop 
             } 
         } 
通过输出 err.what() 的返回值提示用户。大家都知道 err 返回 runtime_error 类型的值，因此可以推断出 what 是 runtime_error 类的一个成员函数。

### 标准异常
标准库异常类定义在四个头文件
中： 
1.  exception 头文件定义了最常见的异常类，它的类名是 exception。这个类只通知异常的产生，但不会提供更多的信息。 
2.  stdexcept 头文件定义了几种常见的异常类，这些类型在表 6.1 中列出。
![这里写图片描述](http://img.blog.csdn.net/20150701171740032)
new 头文件定义了 **bad_alloc** 异常类型，**提供因无法分配内在而由 new（第 5.11 节）抛出的异常。** 
3.  type_info 头文件定义了 bad_cast 异常类型，这种类型将第 18.2 节讨论。


### 使用预处理器进行调试
C++ 程序员有时也会使用类似的技术有条件地执行用于调试的代码。

     int main() 
         { 
         #ifndef NDEBUG 
         cerr << "starting main" << endl; 
         #endif 
         // ...

如果 NDEBUG 未定义，那么程序就会将信息写到 cerr 中。
默认情况下，NDEBUG 未定义，这也就意味着必须执行 #ifndef 和 #endif 之间的代码。在开发程序的过程中，只要保持 NDEBUG 未定义就会执行其中的调试语句。开发完成后，要将程序交付给客户时，可通过定义NDEBUG 预处理变量，（有效地）删除这些调试语句。大多数的编译器都提供定义 NDEBUG命令行选项： 
 

    $ CC -DNDEBUG main.C 

 
这样的命令行行将于在 main.c 的开头提供 #define NDEBUG 预处理命令。
预处理器还定义了其余四种在调试时非常有用的常量： 
__FILE__ 文件名 
__LINE__ 当前行号 
__TIME__ 文件被编译的时间 
__DATE__ 文件被编译的日期 

    if (word.size() < threshold) 
             cerr << "Error: " << _ _FILE_ _ 
                  << " : line " << _ _LINE_ _ << endl 
                  << "       Compiled on " << _ _DATE_ _ 
                  << " at " << _ _TIME_ _ << endl 
                  << "      Word read was " << word 
                  << ": Length too short" << endl; 

如果给这个程序提供一个比 threshold 短的 string 对象，则会产生下面的错误信息：

> Error: wdebug.cc : line 21 
>                Compiled on Jan 12 2005 at 19:44:40 
>                Word read was "foo": Length too short

**另一个常见的调试技术是使用 NDEBUG 预处理变量以及 assert 预处理宏**。assert 宏是在 cassert 头文件中定义的，所有使用 assert 的文件都必须包含这个头文件。
预处理宏有点像函数调用。assert 宏需要一个表达式作为它的条件： 

     **assert(expr)**

**只要 NDEBUG 未定义，assert 宏就求解条件表达式 expr**，如果结果为 **false，assert 输出信息并且终止程序的执行**。如果该表达式有一个**非零（例如，true）值，则 assert 不做任何操作。**
与异常不同（异常用于处理程序执行时预期要发生的错误），程序员使用 assert 来**测试“不可能发生”的条件**。例如，对于处理输入文本的程序，可以预测全部给出的单词都比指定的阈值长。那么程序可以包含这样一个语句： 
 

    assert(word.size() > threshold); 

在测试过程中，assert 等效于检验数据是否总是具有预期的大小。一旦开发和测试工作完成，程序就已经建立好，并且定义了 NDEBUG,那assert就会不起作用。assert 仅用于检查确实不可能的条件，这只对程序的调试有帮助，但不能用来代替运行时的逻辑检查，也不能代替对程序可能产生的错误的检测。

