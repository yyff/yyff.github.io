---
layout: article
title:  "c++ primer读书笔记--第五章 表达式"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

C++ 还支持操作符重载，允许程序员自定义用于类类型时操作符的含义。标准库正是使用这种功能定义用于库类型的操作符。

## 算术操作符

![这里写图片描述](http://img.blog.csdn.net/20150630195420482)


## 关系操作符和逻辑操作符


![这里写图片描述](http://img.blog.csdn.net/20150630195432505)
  

## 位操作符
  
![这里写图片描述](http://img.blog.csdn.net/20150630195448863)


**对于位操作符，由于系统不能确保如何处理其操作数的符号位，所以强烈建议使用 unsigned 整型操作数。**


### bitset  对象或整型值的使用

    bitset<30> bitset_quiz1;//声明
    bitset_quiz1[27];//取第27位
     bitset_quiz1.set(27); //第 27 位设置为 1
    bitset_quiz1.reset(27);//第 27 位设置为 0

### IO  操作符为左结合
像其他二元操作符一样，移位操作符也是左结合的。
移位操作符具有中等优先级：其优先级比算术操作符低，但比关系操作符、赋值操作符和条件操作符优先级高。
   

     cout << "hi" << " there" << endl;

 
执行为： 
 

    ( (cout << "hi") << " there" ) << endl;

## 赋值操作符
赋值操作符的左操作数必须是非 const 的左值。下面的赋值语句是不合法的： 

     int i, j, ival; 
     const int ci = i;  // ok: initialization not assignment 
     1024 = ival;       // error: literals are rvalues 
     i + j = ival;      // error: arithmetic expressions are rvalues 
     ci = ival;         // error: can't write to ci

### 赋值操作的右结合性
与下标和解引用操作符一样，赋值操作也返回左值。同理，只要被赋值的每个操作数都具有相同的通用类型，C++语言允许将这多个赋值操作写在一个表达式中;与其他二元操作符不同，赋值操作具有右结合特性。当表达式含有多个赋值操作符时，从右向左结合： 
 

    int ival, jval; 
     ival = jval = 0; // ok: each assigned 0

### 赋值操作具有低优先级


### 复合赋值操作符

		+=   -=   *=   /=   %=   // arithmetic operators
     <<= >>=   &=   ^=   |=   // bitwise operators 


## 箭头操作符
C++ 语言为包含点操作符和解引用操作符的表达式提供了一个同义词：箭头操作符（->）。点操作符（第 1.5.2 节）用于获取类类型对象的成员：

    item1.same_isbn(item2); // run the same_isbn member of item1

如果有一个**指向 Sales_item 对象的指针**（或迭代器），则在**使用点操作符前，需对该指针（或迭代器）进行解引用**： 
 

        Sales_item *sp = &item1; 
         (*sp).same_isbn(item2); // run same_isbn on object to which sp points 

C++ 为在点操作符后使用的解引用操作定义了一个同义词：箭头操作符（->）。假设有一个指向类类型对象的指针（或迭代器），下面的表达式相互等价： 

    sp->same_isbn(item2); // equivalent to (*sp).same_isbn(item2)


## 条件操作符
条件操作符是 C++ 中唯一的三元操作符，它允许将简单的 if-else 判断语句嵌入表达式中。条件操作符的语法格式为： 
 

    cond ? expr1 : expr2;

条件操作符首先计算 cond 的值，如果 cond 的值为 0，则条件为 false；如果 cond 非 0，则条件为 true。然后，条件为 true 时计算 expr1 ，否则计算 expr2 。

## sizeof 操作符
sizeof 操作符的作用是**返回一个对象或类型名的长度**，返回值的类型为 size_t（第 3.5.2 节），长度的单位是字节（第 2.1 节）。size_t 表达式的结果是编译时常量，该操作符有以下三种语法形式：

    sizeof (type name); 
         sizeof (expr); //expr=表达式
         sizeof expr;

注:将 sizeof 应用在表达式 expr 上，将获得该表达式的结果的类型长度
所以**将 sizeof 用于 expr 时，并没有计算表达式 expr 的值。**特别是在 sizeof *p 中，指针 p 可以持有一个无效地址，因为不需要对 p 做解引用操作。

### 使用 sizeof 的结果部分地依赖所涉及的类型：
•   对 char 类型或值为 char 类型的表达式做 sizeof 操作保证得 1。 
•   对引用类型做 sizeof 操作将返回存放此引用类型对象所需的内在空间大小。 
•   对指针做 sizeof 操作将返回存放指针所需的内在大小；注意，如果要获取该指针所指向对象的大小，则必须对指针进行解引用。
•   对数组做 sizeof 操作等效于将**对其元素类型做 sizeof 操作的结果乘上数组元素的个数**。

注:因为 sizeof 返回整个数组在内存中的存储长度，所以用 sizeof 数组的结
果除以 sizeof 其元素类型的结果，即可**求出数组元素的个数**：

    int sz = sizeof(ia)/sizeof(*ia);


### 逗号操作符
逗号表达式是一组由逗号分隔的表达式，**这些表达式从左向右计算。逗号表达式的结果是其最右边表达式的值。**如果最右边的操作数是左值，则逗号表达式的值也是左值。

    int a1,a2,b=2,c=7,d=5;
    a1=(++b,c--,d+3);
    a2=++b,c--,d+3;

*对于给a1赋值的代码，有三个表达式，用逗号分开，所以最终的值应该是最后一个表达式的值，也就是(d+3)的值，为8，所以a1的值为8。
*对于给a2赋值的代码，也是有三个表达式，这时的三个表达式为a2=++b、c--、d+3,(这是因为赋值运算符比逗号运算符优先级高)所以最终表达式的值虽然也为8，但a2=4（第2行代码运算完时，b=3，即第3行代码运行时，b的值为3）。

## 复合表达式计算

### 优先级

![这里写图片描述](http://img.blog.csdn.net/20150630195512681)



 


  [1]: ./images/1435654006268.jpg "1435654006268.jpg"
  [2]: ./images/1435654288492.jpg "1435654288492.jpg"
  [3]: ./images/1435654361692.jpg "1435654361692.jpg"
  [4]: ./images/1435659186091.jpg "1435659186091.jpg"
  

    if (ia[index++] < ia[index])

 
此表达式的行为没有明确定义。问题在于：< 操作符的左右操作数都使用了 index 变量，但是，左操作数更改了该变量的值。假设 index 初值为 0，编译器可以用下面两种方式之一求该表达式的值： 
 

    if (ia[0] < ia[0]) // execution if rhs is evaluated first 
     if (ia[0] < ia[1]) // execution if lhs is evaluated first 

改为下述语句可以明确该表达式:

    if (ia[index] < ia[index + 1]) { 
             // do whatever 
         } 
         ++index;


##  new  和 delete 表达式
使用 new 和 delete 表达式**动态创建和释放数组**，这两种表达式也可用于动态创建和释放单个对象。
定义变量时，必须指定其数据类型和名字。而**动态创建对象时，只需指定其数据类型，而不必为该对象命名**。取而代之的是，new 表达式返回指向新创建对象的指针，我们通过该指针来访问此对象： 
 

    int i;              // named, uninitialized int variable 
     int *pi = new int;  // pi points to dynamically allocated, 
                         // unnamed, uninitialized int

### 动态创建对象的初始化
动态创建的对象可用初始化变量的方式实现初始化： 
 

    int i(1024);              // value of i is 1024
     int *pi = new int(1024);  // object to which pi points is 1024 
     string s(10, '9');                   // value of s is "999 9999999" 
     string *ps = new string(10, '9');    // *ps is "9999999999" 
     delete ps;//回收ps指向的空间

### const   对象的动态分配和回收

    // allocate and initialize a const object 
         const int *pci = new const int(1024); 

尽管程序员不能改变 const 对象的值，但可撤销对象本身。

    delete pci; // ok: deletes a const object

##  类型转换

### 隐式转换
转换为不损失精度的数,int->double

    int ival = 0; 
         ival = 3.541 + 3; // typically compiles with a warning


本例中，整数 3 被转换为 double 类型，然后执行浮点类型的加法操作，得 double 类型的结果 6.541。再讲double 类型转换为int型的6


### 算术转换 

最简单的转换为整型提升：对于所有比 int 小的整型，包括 char、signed char、unsigned char、short 和 unsigned short，如果该类型的所有可能的值都能包容在 int 内，它们就会被提升为 int型，否则，它们将被提升为 unsigned int。如果将 bool 值提升为 int ，则false 转换为 0，而 true 则转换为 1。

#### 有符号与无符号类型之间的转换

unsigned 操作数的转换依赖于机器中整型的相对大小，因此，这类转换本质上依赖于机器。

#### 转换与枚举类型 

注:枚举型是一个集合，集合中的**元素(枚举成员)是一些命名的整型常量**，元素之间用逗号,隔开。
C++ 自动将枚举类型（第 2.7 节）的对象或枚举成员（ enumerator ）转换为整型，其转换结果可用于任何要求使用整数值的地方。

#### 转换为  const   对象

当使用非 const 对象初始化 const 对象的引用时，系统将非 const 对象转换为 const 对象。此外，还可以将非 const 对象的地址（或非 const 指针）转换为指向相关 const 类型的指针：

#### 命名的强制类型转换(cast)

命名的强制类型转换符号的一般形式如下： 

     cast-name<type>(expression);

其中 cast-name 为 static_cast、dynamic_cast、const_cast 和reinterpret_cast 之一，type 为转换的目标类型，而 expression 则是被强制转换的值。强制转换的类型指定了在 expression上执行某种特定类型的转换。

#### 旧式强制类型转换
 

    char *pc = (char*) ip;


 
 **效果与使用 reinterpret_cast 符号相同**，但这种强制转换的可视性比较差，难以跟踪错误的转换。
 
 旧式强制转换符号有下列两种形式： 
 

    type (expr); // Function-style cast notation 
     (type) expr; // C-language-style cast notation
