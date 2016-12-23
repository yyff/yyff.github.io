---
layout: article
title:  "c++ primer读书笔记--第四章 数组和指针"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

## 数组

### 数组的定义和初始化
数组的维数必须用值大于等于 1 的**常量表达式定义**

    int staff_size = 27;  // nonconst
    double salaries[staff_size];     // error: non const  variable
    int test_scores[get_size()];     // error: non const expression 

虽然 staff_size 是用字面值常量进行初始化，但 staff_size 本身是一个非 const 对象，只有在运行时才能获得它的值，因此，使用该变量来定义数组维数是非法的.

### 显式初始化数组元素

    int ia[array_size] = {0, 1, 2};

如果没有显式提供元素初值，则数组元素会像普通变量一样初始化： 
•   在**函数体外定义**的内置数组，其元素均初始化为 0。 
•   在**函数体内定义**的内置数组，其元素无初始化。 
•   不管数组在哪里定义，如果其**元素为类类型**，则自动调用该类的默认构造函数进行初始化；如果该类没有默认构造函数，则必须为该数组的元素提
供显式初始化。
•   除非显式地提供元素初值，否则**内置类型**的局部数组的元素没
有初始化。此时，除了给元素赋值外，其他使用这些元素的操
作没有定义。
如果维数大于列出的元素初值个数，则只初始化前面的数组元素；剩下的其他元素，若是**内置类型则初始化为0**，若是**类类型则调用该类的默认构造函数进行初始化**：

    string str_arr[5] = {"hi", "bye"};// Equivalent to str_arr = {"hi", "bye", "", "", ""} 

注:数组的长度是固定的,与 vector 类型不同，数组不提供 push_back 或者其他的操作在数组中添加新元素，

### 数组操作
与 vector 元素一样，数组元素可用下标操作符（第 3.3.2节）来访问在用下标访问元素时，**vector 使用 vector::size_type**作为下标的类型，而**数组下标**的正确类型则是 **size_t**（第3.5.2 节）。
**数组和vector复制的比较:**
**数组复制:**

    const size_t array_size = 10; 
    int ia1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; 
    int ia2[array_size]; 
    for (size_t ix = 0; ix != array_size; ++ix) 
    ia2[ix] = ia1[ix];

**vector复制:**

    vector<int> ivec1(10, 20);//每个元素初始化为20 
    vector<int> ivec2; 
    for (vector<int>::iterator iter = ivec1.begin(); 
    iter != ivec1.end(); ++iter) 
    ivec2.push_back(*iter); 


## 指针
指针保存的是另一个对象的地址.

建议:尽量避免使用指针和数组,现代 C++程序采用 vector类型和迭代器取代一般的数组、采用 string 类型取代 C 风格字符串。

### 指针变量的定义
C++ 语言使用 * 符号把一个标识符声明为指针： 
    
  

    vector<int> *pvec; // pvec can point to a vector<int> 
          int *ip1, *ip2; // ip1 and ip2 can point to an int


### 指针可能的取值
一个有效的指针必然是以下三种状态之一:保存一个特定对象的地址；指向某个对象后面的另一对象；或者是 0值。**若指针保存0值，表明它不指向任何对象**。未初始化的指针是无效的，直到给该指针赋值后，才可使用它。

    int *pi = 0;       // pi initialized to address no object
    int *pi3;          // ok, but dangerous, pi3 is unini tialized 

> tip:如果可能的话，除非所指向的对象已经存在，否则不要先定义指针，这样可避免定义一个未初始化的指针。

### 指针初始化和赋值操作的约束 
1.  **0 值常量表达式**（第 2.7 节），例如，在编译时可获得 0 值的整型const 对象或字面值常量 0。 
2.  **类型匹配的对象**的地址。 
3.  另一**对象末的下一地址**。 
4.  **同类型的另一个有效指针**。

### void*  指针
C++ 提供了一种特殊的指针类型 void* ，它可以保存任何类型对象的地址：

    double obj = 3.14;
    void *pv = &obj;       // obj can be an object of any type

void* 表明该指针与一地址值相关，但不清楚存储在此地址上的对象的类型。

void* 指针只支持几种有限的操作：与另一个指针进行比较；向函数传递 void* 指针或从函数返回 void* 指针；给另一个 void* 指针赋值。

> 不允许使用 void* 指针操纵它所指向的对象。

### 指针和引用的比较
考虑以下两个程序段。第一个程序段将一个指针赋给另一指针： 
      

    int ival = 1024, ival2 = 2048; 
          int *pi = &ival, *pi2 = &ival2; 
          pi = pi2;    // pi now points to ival2

 
赋值结束后，pi 所指向的 ival 对象值保持不变，**赋值操作修改了 pi 指
针的值，使其指向另一个不同的对象**。现在考虑另一段相似的程序，使用两个引用赋值：
       

 

    int &ri = ival, &ri2 = ival2; 
          ri = ri2;    // assigns ival2 to ival 


这个赋值操作修改了 ri 引用 ival 对象的值，而并非引用本身。**赋值后，
这两个引用还是分别指向原来关联的对象，此时这两个对象的值相等**

### 指向指针的指针
指针本身也是可用指针指向的内存对象。指针占用内存空间存放其值，因此
指针的存储地址可存放在指针中。

    int ival = 1024; 
              int *pi = &ival; // pi points to an int 
              int **ppi = &pi; // ppi points to a pointer to int

            
![这里写图片描述](http://img.blog.csdn.net/20150629185407421)
  


  为了真正地访问到 ival 对象，必须对 ppi 进行两次解引用 ,**ppi

### 指针的算术操作
通过指针的算术操作来获取指定内容的存储地
址。指针的算术操作和迭代器的算术操作（第 3.4.1 节）以相同的方式实现（也
具有相同的约束）。


    ptrdiff_t n = ip2 - ip;

两个指针减法操作的结果是标准库类型（library type）ptrdiff_t  的数据。与 size_t 类型一样，ptrdiff_t 也是一种与机器相关的类型，在 cstddef头文件中定义。size_t 是 unsigned 类型，而 **ptrdiff_t 则是 signed 整型。**

### const对象的指针和const指针
如果指针指向 const 对象，则不允许用指针来改变其所指的 const 值。为了保证这个特性，C++ 语言强制要求指向 const 对象的指针也必须具有 const 特性： 

    const double pi = 3.14; 
              double *ptr = &pi;        // error: ptr is a plain pointer 
              const double *cptr = &pi; // ok: cptr is a pointer to const

const 指针——本身的值不能修改： 
     

 

    int errNumb = 0; 
          int *const curErr = &errNumb; // curErr is a constant  pointer


### 指针和 typedef

    typedef string *pstring; 
              const pstring cstr;

**错误的解释:cstr 为指向 string 类型的 const 对象**        
      

        const string *cstr; // wrong interpretation of const pstring cstr

错误的原因在于将 typedef 当做文本扩展了。声明 const pstring 时，
**const 修饰的是 pstring** 的类型，这是一个指针。因此，该声明语句应该是把 
cstr 定义为指向 string 类型对象的 const 指针，这个定义等价于：

    // cstr is a const pointer to string 
              string *const cstr; // equivalent to const pstring cstr
 

> 阅读 const 声明语句产生的部分问题，源于 const 限定符既可以放 在类型前也可以放在类型后：

    string const s1;   // s1 and s2 have same type, 
              const string s2;   // they're both strings that are const 


## C  风格字符串
cstring 是 string.h 头文件的 C++ 版本，而 string.h 则是 C 语言提供
的标准库。

![这里写图片描述](http://img.blog.csdn.net/20150629185423389)

>  传递给这些标准库函数例程的指针必须具有非零值，并且指向以 null 结束 的字符数组中的第一个元素。

※在使用处理 C 风格字符串的标准库函数时，牢记字符串必须以结束符 null 
结束.
※必须确保目标字符串具有足够的大小
※使用  strn  函数处理C 风格字符串更安全(规定了操作字符的个数)
strncpy(largeStr, cp1, 17); // size to copy includes  the null


## 动态数组的定义
动态分配数组时，只需指定类型和数组长度，不必为数组对象命名，**new表达式返回指向新分配数组的第一个元素的指针**： 
     

 

    int *pia = new int[10]; // array of 10 uninitialized ints

也可使用跟在数组长度后面的一对**空圆括号，对数组元素做值初始化**（第 
3.3.1 节）： 
      

    int *pia2 = new int[10] (); // array of 10 uninitiali zed ints 


圆括号要求编译器对数组做值初始化，在本例中即把数组元素都设置为 0。

### const   对象的动态数组
因为数组元素都是 const 对象，无法赋值。实现这个要求的唯一方法是对数组做值初始化：

          const int *pci_bad = new const int[100]; 
          // error: uninitialized const array
          const int *pci_ok = new const int[100](); 
          // ok: value-initialized const array

### 动态空间的释放
delete [] 表达式释放指针所指向的数组空间： 
      

    delete [] pia; 

空方括号对是必不可少的：它告诉编译器该指针指向的是自由存储区中的数组，而并非单个对象。 
### 指针和多维数组

    int ia[3][4];      // array of size 3, each element is an  array of 
    ints of size 4 
         int (*ip)[4] = ia; // ip points to an array of 4 ints 
         ip = &ia[2];       // ia[2] is an array of 4 ints 

         
*ip 是 int[4] 类型——即 ip 是一个指向含有 4 个元素的数组的指针。

### 用 typedef  简化指向多维数组的指针

     int ia[3][4]; 
    typedef int int_array[4]; 
         int_array *ip = ia; 
    //可使用 typedef 类型输出 ia 的元素： 
         for (int_array *p = ia; p != ia + 3; ++p) 
             for (int *q = *p; q != *p + 4; ++q) 
                  cout << *q << endl;

外层的 for 循环首先初始化 p 指向 ia 的第一个内部数组，然后一直循环
到 ia 的三行数据都处理完为止。++p 使 p 加 1，等效于移动指针使其指向 ia 的下一行（例如：下一个元素）。
内层的 for 循环实际上处理的是存储在内部数组中的 int 型元素值。首先
让 q 指向 p 所指向的数组的第一个元素。

