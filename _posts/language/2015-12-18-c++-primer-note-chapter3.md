---
layout: article
title:  "c++ primer读书笔记--第三章 标准库类型"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

## 命名空间的 using  声明
使用 using 声明可以在不需要加前缀 namespace_name:: 的情况下访问命名空间中的名字。using声明的形式如下： 
   
      using namespace::name;

如：

    #include <iostream>
    #include <string>
    using std::cin; 
    using std::string;
    
    string s;       // ok: string is now a synonym for std::string 
    cin >> s;

**注:标准库头文件用<>,其他的用""**  
## 标准库 string 类型
### 几种初始化 string 对象的方式

![这里写图片描述](http://img.blog.csdn.net/20150628182045730)


### 使用 getline  读取整行文本
  
这个函数接受两个参数：
一个输入流对象和一个 string 对象。getline 函数从输入流的下一行读取，并
**保存读取的内容到不包括换行符**。和输入操作符不一样的是，**getline 并不忽略
行开头的换行符**。只要 getline 遇到换行符，即便它是输入的第一个字符，
getline 也将停止读入并返回。如果第一个字符就是换行符，则 string 参数将
被置为空 string。
### 常用的 string 操作

![这里写图片描述](http://img.blog.csdn.net/20150628181621756)


  
### string::size_type  类型
size 操作返回的是 string::size_type类型的值。string类类型和许多其他库类型都定义了一些配套类型（companion  type）。**通过这些配套类型，库类型的使用就能与机器无关**（machine-independent）。size_type  就是这些配套类型中的一种。
#### string 关系操作符
关系操作符比较两个 string 对象时采用了和（大小写敏感的）**字典排序**相同的策略： 
•   如果两个 string 对象长度不同，且短的 string 对象与长的 string 对
象的前面部分相匹配，则短的 string 对象小于长的 string 对象。 
•   如果 string 对象的字符不同，则比较第一个不匹配的字符。
### 和字符串字面值的连接

 
当进行 string 对象和字符串字面值混合连接操作时，+ 操作符的左右操作
数必须至少有一个是 string 类型的： 

    string s3 = s1 + ", " + s2 + "\n"; // right 
    
     string s4 = "hello" + ", ";      // error: no string operand
     

### cctype  中的函数

![这里写图片描述](http://img.blog.csdn.net/20150628181638311)


注: **C 标准库头文件命名形式为 name 而 C++ 版本则命名为 cname ，少了
后缀，.h 而在头文件名前加了 c** ,表示这个头文件源自 C 标准库。因此，
cctype 与 ctype.h 文件的内容是一样的  通常，C++ 程序中应采用 cname 这种头文件的版本，而不采用 name.h 版本，这样，标准库中的名字在命名空间 std 中保持一致  

## 标准库 vector 类型

![这里写图片描述](http://img.blog.csdn.net/20150628181656633)

vector 是同一种类型的对象的集合，每个对象都有一个对应的整数索引值。
vector 是一个类模板（class template）。使用模板可以编写一个类定义
或函数定义，而用于多个不同的数据类型。必须说明 vector 保存何种对象的类型，通过将类型放在类型放在类模板名称后面的尖括号中来指定类型： 

     vector<int> ivec;               // ivec holds objects of type int 
     vector<Sales_item> Sales_vec;   // holds Sales_items

  **注:vector 不是一种数据类型，而只是一个类模板，可用来定义任意多种数据类型。vector 类型的每一种都指定了其保存元素的类型。因此，vector<int> 和 vector<string> 都是数据类型。**
   
     

 
 
 


注:虽然可以对给定元素个数的 vector 对象预先分配内存，但更有效的方法是先初始化一个空 vector 对象，然后再动态地增加元素（我们随后将学习如何进行这样的操作）。

### vector  对象的操作

![这里写图片描述](http://img.blog.csdn.net/20150628181714752)


 empty 和 size 操作类似于 string 的相关操作（3.2.3 节）。成员函数 
size 返回相应 vector 类定义的 size_type 的值。 
 
使用 size_type 类型时，必须指出该类型是在哪里定义的。
vector 类型总是包括总是包括 vector 的元素类型.如:

    vector<int>::size_type        // ok 
         vector::size_type            // error

### vector  的下标操作:         

    for (vector<int>::size_type ix = 0; ix != ivec.size(); ++ix) 
             ivec[ix] = ix;//error:下标操作不能添加新元素
             ivec.push_back(ix);  // ok: adds new element with value ix

注:下标操作可以进行重置已有元素,但若元素不存在,必须先用push_back方法分配内存并增加新元素

上述for循环中的判断条件ix != ivec.size()中,循环可以容易地增加新元素，如果确实增加了新元素的话，那么测试已保存的 size 值作为循环的结束条件就会有问题，因为没有将新加入的元素计算在内。所以我们倾向于在每次循环中测试 size 的当前值，而不是在进入循环前，存储 size 值的副本。
## 迭代器简介
除了使用下标来访问 vector 对象的元素外，标准库还提供了另一种访问元
素的方法：使用迭代器（iterator）。迭代器是一种检查容器内元素并遍历元素
的数据类型。
各容器类都定义了自己的 iterator 类型，用于访问容器内的元素。换句话说，每个容器都定义了一个名为 iterator 的类型，而这种类型支持（概念上的）迭代器的各种操作。 

每种容器都定义了一对命名为 begin 和 end 的函数，用于返回迭代器。如
果容器中有元素的话，由 begin 返回的迭代器指向第一个元素：
    

     vector<int>::iterator iter = ivec.begin();

由 end 操作返回的迭代器指向 vector 的“末端元素的下一个”。“超出末端迭代器”（off-the-end iterator）。表明它指向了一个不存在的元素。

由 end 操作返回的迭代器并不指向 vector 中任何实际的元素，相反，它只是起一个**哨兵**（sentinel）的作用，表示我们已处理完 vector 中所有元素。
通过迭代器改变原来的重置操作:
原来版本:

    // reset all the elements in ivec to 0 
         for (vector<int>::size_type ix = 0; ix != ivec.size(); ++ix) 
                 ivec[ix] = 0;

迭代器版本:

    // equivalent loop using iterators to reset all the elemen ts in ivec 
    to 0 
         for (vector<int>::iterator iter = ivec.begin(); 
                                    iter != ivec.end(); ++iter) 
             *iter = 0;  // set element to which iter refers to 0

### const_iterator类型 
前面的程序用 vector::iterator 改变 vector 中的元素值。每种容器类型
还定义了一种名为 const_iterator 的类型，**该类型只能用于读取容器内元素，
但不能改变其值。**        

    for (vector<string>::const_iterator iter = text.begin(); 
                                      iter != text.end(); ++ iter) 
             *iter = " ";     // error: *iter is const

不要把 const_iterator 对象与 const 的 iterator 对象混淆起来。声明
**一个 const 迭代器时，必须初始化迭代器。一旦被初始化后，就不能改变它的
值(即下列的cit的值不能被改变)**：

            vector<int> nums(10);  // nums is nonconst 
            const vector<int>::iterator cit = nums.begin(); 
            ++cit;//error,the value of cit cant be changed
            

结论:

    // an iterator that cannot write elements 
         vector<int>::const_iterator 
         // an iterator whose value cannot change 
         const vector<int>::iterator 


**定位于 vector 中间元素**

    vector<int>::iterator mid = vi.begin() + vi.size() / 2;

## 标准库 bitset
### bitset  对象的定义和初始化

![这里写图片描述](http://img.blog.csdn.net/20150628181728810)

             
             
         


  
例如:

    bitset<16> bitvec1(0xffff);          // bits 0 ... 15 are  set to 1 
    string strval("1100"); 
    bitset<32> bitvec4(strval); //bitvec4 的位模式中第 2 和 3 的位置为 1，其余位置都为 0。

### bitset  对象上的操作

![这里写图片描述](http://img.blog.csdn.net/20150628181749359)

 如果需要知道置为 1 的二进制位的个数，可以使用 count 操作，该操作返
回置为 1 的二进制位的个数： 
 

        size_t bits_set = bitvec.count(); // returns number of bit s that are 
    on 

 
count 操作的返回类型是标准库中命名为 **size_t 类型**。size_t 类型定义
在 cstddef 头文件中，该文件是 C 标准库的头文件 stddef.h 的 C++ 版本。
它是一个与机器相关的 unsigned 类型，其大小足以保证存储内在中对象的大
小。 
与 vector 和 string 中的 size 操作一样，bitset 的 size 操作返
回 bitset 对象中二进制位的个数，返回值的类型是 size_t:： 
 

    size_t sz = bitvec.size(); // returns 32


### 访问 bitset 对象中的位

     // assign 1 to even numbered bits 
         for (int index = 0; index != 32; index += 2) 
                     bitvec[index] = 1;//或用bitvec.set(index);

### 输出二进制位

    cout << "bitvec2: " << bitvec2 << endl;

输出结果为： 

     bitvec2: 00000000000000001111111111111111

本章疑点:
**size_t**  
在 cstddef 头文件中定义的机器相关的无符号整型，该类型足以保存最大
数组的长度。 在 cstddef 头文件中定义的机器相关的无符号整型，该类型足以保存最大数组的长度。 

**size_type**   
由 string 类类型和 vector 类类型定义的类型，用以保存任意 string 对象或 vecotr 对象的长度。标准库类型将 size_type 定义为 unsigned 类型。

**:: operator（::操作符）**   
作用域操作符。::操作符在其左操作数的作用域内找到其右操作数的名
字。用于访问某个命名空间中的名字，如 std::cout，表明名字 cout 来
自命名空间 std。同样地，可用来从某个类取名字，如 string::size_type，
表明 size_type 是由 string 类定义的。


 