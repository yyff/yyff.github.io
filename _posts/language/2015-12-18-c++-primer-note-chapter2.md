---
layout: article
title:  "c++ primer读书笔记--第二章 变量和基本类型"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记





### 整型字面值规则

定义字面值整数常量可以使用以下三种进制中的任一种：十进制、八进制和
十六进制。 当然这些进制不会改变其二进制位的表示形式。 例如， 我们能将值 20定义成下列三种形式中的任意一种：
 **20 // decimal,十进制**
 **024 // octal,8进制 前面加0**
 **0x14 // hexadecimal,16进制,前面加0x**
 
###非打印字符的转义序列
有些字符是不可打印的。不可打印字符实际上是不可显示的字符，比如退格或者控制符。还有一些在语言中有特殊意义的字符，例如单引号、双引号和反斜线符号。不可打印字符和特殊字符都用转义字符书写。转义字符都以反斜线符号开始，C++ 语言中定义了如下转义字符：
**换行符 \n** 
**水平制表符 \t**
**纵向制表符 \v** 
**退格符 \b**
**回车符 \r** 
**进纸符 \f**
**报警（响铃）符 \a** 
**反斜线 \\**
**疑问号 \?** 
**单引号 \'**
**双引号 \"**

### 多行字面值
处理长字符串有一个更基本的（但不常使用）方法，这个方法依赖于很少使用的程序格式化特性： 在一行的末尾加一反斜线符号可将此行和下一行当作同一行处理。宏定义(define)中也可以使用


    // ok: A \ before a newline ignores the line break
     std::cou\
     t << "Hi" << st\
     d::endl;

等价于


     std::cout << "Hi" << std::endl;

### 关键概念：强静态类型
C++ 是一门静态类型语言，在编译时会作类型检查。在大多数语言中，对象的类型限制了对象可以执行的操作。如果某种类型不支持某种操作，那么这种类型的对象也就不能执行该操作。


### 什么是对象？
C++ 程序员经常随意地使用术语对象。一般而言，对象就是内存中具有类型的区域。说得更具体一些，计算左值表达式就会产生对象。

### 变量名
变量名，即变量的标识符，可以由字母、数字和下划线组成。变量名必须以字母或下划线开头，并且区分大小写字母
C++ 保留了一组词用作该语言的关键字。关键字不能用作程序的标识符。

![这里写图片描述](http://img.blog.csdn.net/20151218102947158)

### 变量命名习惯
*变量名一般用小写字母。例如，通常会写成 index，而不写成 Index 或
INDEX。(常量名用全大写)
*包含多个词的标识符书写为在每个词之间添加一个下划线， 或者每个内嵌
的词的第一个字母都大写。例如通常会写成 student_loan 或studentLoan，而不写成 studentloan。

### 变量初始化
**变量定义指定了变量的类型和标识符，也可以为对象提供初始值。**定义时指定了初始值的对象被称为是已初始化的。

    int ival(1024); // direct-initialization,直接初始化--效率高
     int ival = 1024; // copy-initialization,复制初始化

初始化内置类型的对象只有一种方法：提供一个值，并且把这个值复制到新定义的对中。**对内置类型来说，复制初始化和直接初始化几乎没有差别。**每个类都可能会定义一个或几个特殊的成员函数（第 1.5.2 节）来告诉我们如何初始化类类型的变量。**定义如何进行初始化的成员函数称为构造函数**。和其他函数一样，构造函数能接受多个参数。一个类可以定义几个构造函数，每个构造函数必须接受不同数目或者不同类型的参数。


    std::string all_nines(10, '9'); // all_nines= "9999999999"


本例中，**初始化 all_nines 的唯一方法是直接初始化。有多个初始化式时不能使用复制初始化。**

### 声明和定义
**变量的定义**用于为变量分配存储空间，还可以为变量指定初始值。在一个程序中，变量有且仅有一个定义。

**声明**用于向程序表明变量的类型和名字。定义也是声明：当定义变量时我们声明了它的类型和名字。可以通过使用 extern 关键字声明变量名而不定义它。不定义变量的声明包括对象名、对象类型和对象类型前的关键字 extern：


    extern int i; // declares but does not define i
    int i; // declares and defines i

**extern 声明不是定义，也不分配存储空间。**事实上，它只是说明变量定义在程序的其他地方。**程序中变量可以声明多次，但只能定义一次。**

只有当声明也是定义时，声明才可以有初始化式，因为只有定义才分配存储空间。初始化式必须要有存储空间来进行初始化。如果声明有初始化式，那么它可被当作是定义，即使声明标记为 extern：如:


    extern double pi = 3.1416; // definition

### 定义 const 对象

const 限定符提供了一个解
决办法，它把一个对象转换成一个常量(不能被修改)。

    const int bufSize = 512; // input buffer size

#### const 对象默认为文件的局部变量

> const 对象默认为文件的局部变量，非const对象默认为文件的全局变量（即默认为extern类型，可以直接在外部文件用extern来使用）。

**在全局作用域（函数外部）里定义非 const 变量时，它在整个程序中都可以访问。**我们可以把一个非 const 变更定义在一个文件中，假设已经做了合适的声明，就可在另外的文件中使用这个变量：


     // file_1.cc
     int counter; // definition
     // file_2.cc
     extern int counter; // uses counter from file_1
     ++counter; // increments counter defined in file_1

与其他变量不同，除非特别说明，**在全局作用域声明的 const 变量是定义该对象的文件的局部变量**。此变量只存在于那个文件中，不能被其他文件访问。**通过指定 const 变更为 extern，就可以在整个程序中访问 const 对象**：


     // file_1.cc
     // defines and initializes a const that is accessible to other files
     extern const int bufSize = fcn();
     // file_2.cc
     extern const int bufSize; // uses bufSize from file_1
     // uses bufSize defined in file_1
     for (int index = 0; index != bufSize; ++index)
	// ...

### 引用
**引用就是对象的另一个名字。**
引用是一种复合类型，通过在变量名前添加“&”符号来定义。复合类型是指用其他类型定义的类型。
**引用必须用与该引用同类型的对象初始化：**


     int ival = 1024;
     int &refVal = ival; // ok: refVal refers to ival
     int &refVal2; // error: a reference must be initialized
     int &refVal3 = 10; // error: initializer must be an object

#### const 引用
const 引用是指向 const 对象的引用：将普通的引用绑定到 const 对
象是不合法的。

     const int ival = 1024;
     const int &refVal = ival; // ok: both reference and object are const
     int &ref2 = ival; // error: non const reference to a const object

### 枚举
我们经常需要为某些属性定义一组可选择的值。例如，文件打开的状态可能
会有三种：输入、输出和追加。

    const int input = 0;
     const int output = 1;
     const int append = 2;

#### 定义和初始化枚举
枚举的定义包括关键字 enum，其后是一个可选的枚举类型名，和一个用花括号括起来、用逗号分开的枚举成员 列表。
默认地，第一个枚举成员赋值为 0，后面的每个枚举成员赋的值比前面的大1。
    
     // input is 0, output is 1, and append is 2
     enum open_modes {input, output, append};

可以为一个或多个枚举成员提供初始值，用来初始化枚举成员的值必须是一
个常量表达式。


    // shape is 1, sphere is 2, cylinder is 3, polygon is 4
     enum Forms {shape = 1, sphere, cylinder, polygon};

#### 每个 enum 都定义一种唯一的类型
每个 enum 都定义了一种新的类型。**枚举类型的对象的初始化或赋值，只能通过其枚举成员或同一枚举类型的其他对象来进行：**


    // point2d is 2, point2w is 3, point3d is 3, point3w is 4
     enum Points { point2d = 2, point2w,
     point3d = 3, point3w };
	
    Points pt3d = point3d; // ok: point3d is a Points enumerator
     Points pt2w = 3; // error: pt2w initialized with int
     pt2w = polygon; // error: polygon is not a Points enumerator
     pt2w = pt3d; // ok: both are objects of Points enum type


### 定义类


    class Sales_item {
     public:
     // operations on Sales_item objects will go here
     private:
     std::string isbn;
     unsigned units_sold;
     double revenue;
     };


类体可以为空。类体定义了组成该类型的数据和操作。这些操作和数据是类的一部分，也称为类的成员。**操作称为成员函数**（第 1.5.2 节），而**数据则称为数据成员** 。类也可以包含 0 个到多个 private 或 public 访问标号。**访问标号控制类的成员在类外部是否可访问。**使用该类的代码可能只能访问 public 成员。

C++ 支持另一个关键字 struct，它也可以定义类类型。它们的唯一区别是:**如果使用 class 关键字来定义类，那么定义在第一个访问标号前的任何成员都隐式指定为 private；如果使用 struct 关键字，那么这些成员都是public。**

#### 编译和链接多个源文件
命令行下编译main.cc 和Sales_item.cc生成mian.exe (#为命令行的注释)

    # puts the executable in main.exe
     $ CC -c main.cc Sales_item.cc -o main

#### 头文件用于声明而不是用于定义
当设计头文件时， 记住定义和声明的区别是很重要的。 定义只可以出现一次，而声明则可以出现多次（第 2.3.5 节）。下列语句是一些定义，所以不应该放在头文件里：


     extern int ival = 10; // initializer, so it's a definition
     double fica_rate; // no extern, so it's a definition

#### 一些 const 对象定义在头文件中
const 变量（第 2.4 节）默认时是定义该变量的文件的局部变量。定义const对象这种行为有一个很重要的含义：**当我们在头文件中定义了 const 变量后，每个包含该头文件的源文件都有了自己的 const 变量，其名称和值都一样。**
如果 const 变量不是用常量表达式初始化，那么它就不应该在头文件中定义。**在源文件定义时，和其他的变量一样，该 const 变量应该在一个源文件中定义并初始化。应在头文件中为它添加 extern 声明，以使其能被多个文件共享。**

### 头文件经常需要其他头文件
因此，设计头文件时，应使其可以多次包含在同一源文件中，这一点很重要。我们必须保证多次包含同一头文件不会引起该头文件定义的类和对象被多次定义。使得头文件安全的通用做法，是使用预处理器定义**头文件保护符**。


    #ifndef SALESITEM_H
     #define SALESITEM_H
     // Definition of Sales_itemclass and related functions goes here
     #endif

### 小结
1.类型可以为 const 或非 const；**const 对象必须要初始化，且其值不能被修改**。另外，我们还可以定义复合类型，如引用。**引用为对象提供了另一个名字**。复合类型是用其他类型定义的类型。

2.C++ 是一种静**态类型语言：变量和函数在使用前必须先声明。变量可以声明多次但是只能定义一次。**定义变量时就进行初始化几乎总是个好主意。

3.**使用命名空间的目的是对标识符的名称进行本地化，以避免命名冲突。在C++中，变量、函数和类都是大量存在的。**如果没有命名空间，这些变量、函数、类的名称将都存在于全局命名空间中，会导致很多冲突。比如，如果我们在自己的程序中定义了一个函数toupper()，这将重写标准库中的toupper()函 数，这是因为这两个函数都是位于全局命名空间中的。命名冲突还会发生在一个程序中使用两个或者更多的第三方库的情况中。此时，很有可能，其中一个库中的名 称和另外一个库中的名称是相同的，这样就冲突了。这种情况会经常发生在类的名称上。比如，我们在自己的程序中定义了一个Stack类，而我们程序中使用的某个库中也可能定义了一个同名的类，此时名称就冲突了。
Namespace 关键字的出现就是针对这种问题的。由于这种机制对于声明于其中的名称都进行了本地化，就使得相同的名称可以在不同的上下文中使用，而不会引起名称的冲突。或许命名空间最大的受益者就是C++中的标准库了。在命名空间出现之前，整个C++库都是定义在全局命名空间中的（这当然也是唯一的命名空间）。引入命名空间后，C++库就被定义到自己的名称空间中了，称之为std。这样就减少了名称冲突的可能性。我们也可以在自己的程序中创建自己的命名空间，这样可以对我们认为可能导致冲突的名称进行本地化。这点在我们创建类或者是函数库的时候是特别重要的。