---
layout: article
title:  "c++ primer读书笔记--第七章 函数"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

函数可以看作程序员定义的操作。与内置操作符相同的是，每个函数都会实现一系列的计算，然后（大多数时候）生成一个计算结果。但与操作符不同的是，函数有自己的函数名，而且操作数没有数量限制。与操作符一样，函数可以重载，这意味着同样的函数名可以对应多个不同的函数。 
   
## 函数的定义

### 函数的调用

C++ 语言使用调用操作符（即一对圆括号）实现函数的调用。正如其他操作符一样，调用操作符需要操作数并产生一个结果。**调用操作符的操作数是函数名**和**一组**（有可能是空的）**由逗号分隔的实参**。函数调用的结果类型就是函数返回值的
类型，该**运算的结果本身就是函数的返回值**：

### 函数体是一个作用域

在函数体内定义的变量只在该函数中才可以访问。这种变量称为局部变量.

### 形参和实参

类似于局部变量，函数**的形参为函数提供了已命名的局部存储空间**。它们之间的差别在于**形参是在函数的形参表中定义的**，并由调用函数时传递函数的实参初始化。**实参则是一个表达式。**

### 函数返回类型

函数的返回类型可以是内置类型（如 int 或者 double）、类类型或复合类型（如 int& 或 string*），还可以是 void 类型。
函数**不能返回另一个函数或者内置数组类型，但可以返回指向函数的指针**，**或指
向数组元素的指针的指针：**

    // ok: pointer to first element of the array 
         int *foo_bar() { /* ... */ } 

### 函数形参表

函数形参表可以为空，但不能省略。没有任何形参的函数可以用空形参表或含有
单个关键字 void 的形参表来表示。

    void process() { /* ... */ }      // implicit void parameter list 
    void process(void){ /* ... */ }  // equivalent declaration

### 参数类型检查    

C++ 是一种静态强类型语句（第 2.3节），对于每一次的函数调用，编译时都会检查其实参是否与对应的形参类型相同，或具有可被转换（第 5.12 节）为该形参类型的类型。

## 参数传递

每次调用函数时，都会重新创建该函数所有的形参，此时所传递的实参将会
初始化对应的形参。
形参的初始化与变量的初始化一样：如果形参具有**非引用类型，则复制实参的值**，如果**形参为引用类型（第 2.5 节），则它只是实参的别名**。

### 指针形参

如果函数形参是**非 const类型的指针，则函数可通过指针实现赋值**，修改指针所指向对象的值： 

    void reset(int *ip) 
    { 
             *ip = 0; // changes the value of the object to which i p points 
             ip = 0;   // changes only the local value of ip; the argument 
    is unchanged 
    }
调用 reset 后，实参依然保持原来的值，但它所指向的对象的值将变为 0；
如果**保护指针指向的值**，则**形参需定义为指向 const 对象的指针**：

    void use_ptr(const int *p) 
         { 
              // use_ptr may read but not write to *p 
         } 
我们既可以用 int* 也可以用 const int* 类型的实参调用 **use_ptr 函数**；但仅能将 int* 类型的实参传递给 **reset 函数**。
**这是因为可以将指向 const 对象的指针初始化为指向非 const 对象，但不可以让指向非 const 对象的指针向 const 对象。**

> 解释:如果函数具有普通的非 const 引用形参，则显然不能通过 const对象进行调用。毕竟，此时函数可以修改传递进来的对象，这样就违背了实参的 const 特性。

 
### 引用形参 

为了使 swap 函数以期望的方式工作，**交换实参的值**，**需要将形参定义为引用类型：**

    // ok: swap acts on references to its arguments 
         void swap(int &v1, int &v2) 
         { 
             int tmp = v2; 
             v2 = v1; 
             v1 = tmp; 
         } 
与所有引用一样，引用形参直接关联到其所绑定的实参，而并非这些对象的副本。形参 v1 只是对象 i 的另一个名字，而 v2 则是对象 j 的另一个名字。

### 使用引用形参返回额外的信息

函数只能返回单个值，但有些时候，函数有不止一个的内容需要返回。例如，定义一个 find_val 函数。在一个整型 vector 对象的元素中搜索某个特定值。如果找到满足要求的元素，则返回指向该元素的迭器；否则返回一个迭代器，指向该 vector 对象的 end操作返回的元素。此外，如果该值出现了不止一次，我们还希望函数可以返回其现的次数。在这种情况下，返回的迭代器应该指向具有要寻找的值的第一个元素。
如何定义既返回一个迭代器又返回出现次数的函数？**1我们可以定义一种包含一个迭代器和一个计数器的新类型**。**2更简便的解决方案是给 find_val传递一个额外的引用实参(传递给引用形参occurs)，用于返回出现次数的统计结果：**

    // returns an iterator that refers to the first occurrence of value 
         // the reference parameter occurs contains a second return value 
         vector<int>::const_iterator find_val( 
             vector<int>::const_iterator beg,             // first element 
             vector<int>::const_iterator end,             // one past last element 
             int value,                                    // the value we want 
             vector<int>::size_type &occurs)              // number of times it occurs  
         { 
             // res_iter will hold first occurrence, if any 
             vector<int>::const_iterator res_iter = end; 
             occurs = 0; // set occurrence count parameter 
             for ( ; beg != end; ++beg) 
                 if (*beg == value) { 
                     // remember first occurrence of value 
                     if (res_iter == end) 
                        res_iter = beg; 
                     ++occurs; // increment occurrence count 
                 } 
             return res_iter;  // count returned implicitly in occurs 
         }

### 利用  const   引用避免复制

虽然**复制实参**对于内置数据类型的对象或者规模较小的类类型对象来说没有什么问题，但是**对于大部分的类类型或者大型数组，它的效率（通常）太低了**；
编写一个比较两个 string 对象长度的函数作为例子。这个函数需要访问每个 string 对象的 size，但不必修改这些对象。由于 string对象可能相当长，所以我们希望避免复制操作。使用 **const引用就可避免复制**：

    // compare the length of two strings 
         bool isShorter(const string &s1, const string &s2) 
         { 
             return s1.size() < s2.size(); 
         } 
其每一个形参都是 const string 类型的引用。因为形参是引用，所以不复制实参。又因为**形参是 const 引用**，所以 isShorter **函数不能使用该引用来修改实参**。

> 应该将不需要修改的引用形参定义为 const 引用。普通的非 const 引用形参参既不能用 const对象初始化，也不能用字面值或产生右值的表达式实参初始化。

### 传递指向指针的引用


     // swap values of two pointers to int 
         void ptrswap(int *&v1, int *&v2) 
         { 
             int *tmp = v2; 
             v2 = v1; 
             v1 = tmp; 
         }
定义应从右至左理解：**v1 是一个引用，与指向 int型对象的指针相关联。也就是说，v1 只是传递进 ptrswap 函数的任意指针的别名。**

下列操作交换的pi和pj指向的对象：

         int i = 10; 
         int j = 20; 
         int *pi = &i;  // pi points to i 
         int *pj = &j; // pj points to j 
         ptrswap(pi, pj); // now pi points to j; pj points to i

### vector  和其他容器类型的形参

从避免复制 vector 的角度出发，应考虑将形参声明为引用类型。事实上，C++ 程序员倾向于通过**传递指向容器中需要处理的元素的迭代器来传递容器：**

    // pass iterators to the first and one past the last element to print 
         void print(vector<int>::const_iterator beg, 
                    vector<int>::const_iterator end) 
         { 
             while (beg != end) { 
                 cout << *beg++; 
                 if (beg != end) cout << " "; // no space after last element 
             } 
             cout << endl; 
         } 

### 数组形参

数组有两个特殊的性质，影响我们定义和使用作用在数组上的函数：一是不能复制数组（第 4.1.1 节）；二是使用数组名字时，数组名会自动转化为指向其第一个元素的指针（第 4.2.4 节）。
数组形参的定义：

    // three equivalent definitions of printValues 
         void printValues(int*) { /* ... */ } 
         void printValues(int[]) { /* ... */ } 
         void printValues(int[10]) { /* ... */ }
虽然不能直接传递数组，但是函数的形参可以写成数组的形式。上面的三种定义是等价的，形参类型都是 int*。 

### 形参的长度会引起误解

编译器忽略为任何数组形参指定的长度。

    // parameter treated as const int*, size of array is ignored 
         void printValues(const int ia[10]) 
         { 
              // this code assumes array has 10 elements; 
              // disaster if argument has fewer than 10 elements! 
              for (size_t i = 0; i != 10; ++i) 
              { 
                  cout << ia[i] << endl; 
              } 
         }

上述代码假定所传递的数组至少含有 10 个元素，但下面的调用都是合法的（少于10个）：

    int main() 
         { 
             int i = 0, j[2] = {0, 1}; 
             printValues(&i);      // ok: &i is int*; probable run-time 
    error 
             printValues(j);      // ok: j is converted to pointer to 0th 
             // element; argument has type int*; 
                                  // probable run-time error 
             return 0; 
         }
         
在这两个调用中，由于函数 printValues 假设传递进来的数组至少含有 10 个元素，因此造成数组内在的越界访问。程序的执行可能产生错误的输出，也可能崩溃，这取决于越界访问的内存中恰好存储的数值是什么。

> 当编译器检查数组形参关联的实参时，它只会检查实参是不是指针、指针的类型和数组元素的类型时是否匹配，而不会检查数组的长度。

### 通过引用传递数组

如果形参是数组的引用，**编译器不会将数组实参转化为指针**，而是传递数组的引用本身。在这种情况下，数组大小成为形参和实参类型的一部分。**编译器检查数组的实参的大小与形参的大小是否匹配**： 

    // ok: parameter is a reference to an array; size of array is fixed 
         void printValues(int (&arr)[10]) { /* ... */ } 
         int main() 
         { 
             int i = 0, j[2] = {0, 1}; 
             int k[10] = {0,1,2,3,4,5,6,7,8,9}; 
             printValues(&i); // error: argument is not an array of  10 ints 
             printValues(j);  // error: argument is not an array of  10 ints 
             printValues(k);  // ok: argument is an array of 10 ints 
             return 0; 
         }
### 多维数组的传递

和其他数组一样，多维数组以指向 0 号元素的指针方式传递。

    // first parameter is an array whose elements are arrays of 10 ints 
         void printValues(int (*matrix)[10], int rowSize); 
我们也可以用数组语法定义多维数组。上面的语句将 matrix 声明为指向含有 10 个 int 型元素的数组的指针。一维数组一样，编译器忽略第一维的长度，所以最好不要把它包括在形参表内：

     // first parameter is an array whose elements are arrays of 10 ints 
         void printValues(int matrix[][10], int rowSize);
matrix形参是一个指针，指向数组的数组中的元素，这些元素本身就是含有 10 个 int 型对象的数组。

## 传递给函数的数组的处理

有三种常见的编程技巧确保函数的操作不超出数组实参的边界。**第一种方法是在数组****本身放置一个标记来检测数组的结束**。C风格字符串就是采用这种方法的一个例子，它是一种字符数组，并且以空字符 null 作为结束的标记。

### 使用标准库规范

**第二种方法是传递指向数组第一个和最后一个元素的下一个位置的指针。**

    void printValues(const int *beg, const int *end) 
         { 
             while (beg != end) { 
                 cout << *beg++ << endl; 
              } 
         } 
         int main() 
         { 
             int j[2] = {0, 1}; 
             // ok: j is converted to pointer to 0th element in j 
             //     j + 2 refers one past the end of j 
             printValues(j, j + 2); 
             return 0; 
         } 

### 显式传递表示数组大小的形参

**第三种方法是将第二个形参定义为表示数组的大小**。

    // const int ia[] is equivalent to const int* ia 
         // size is passed explicitly and used to control access to elements 
    of ia 
         void printValues(const int ia[], size_t size) 
         { 
              for (size_t i = 0; i != size; ++i) { 
                  cout << ia[i] << endl; 
              } 
         } 
         int main() 
         { 
             int j[] = { 0, 1 }; // int array of size 2 
             printValues(j, sizeof(j)/sizeof(*j)); 
             return 0; 
         }


使用了形参 size 来确定要输出的元素的个数。

### main:  处理命令行选项

假设我们的主函数 main 位于名为 prog 的可执行文件中，可如下将实参选项传递给程序：

    prog -d -o ofile data0 

 
这种用法的处理方法实际上是在主函数 main 中定义了两个形参： 
 

    int main(int argc, char *argv[]) { ... } 
第二个形参 argv 是一个 C 风格字符串数组，上述表示 argv 是指向 char* 的指针。
argv 会保存下面几个 C 风格字符串： 

    argv[0] = "prog"; 
    argv[1] = "-d"; 
    argv[2] = "-o"; 
    argv[3] = "ofile"; 
    argv[4] = "data0"; 

### 含有可变形参的函数

C++ 中的省略符形参是为了编译使用了 varargs 的 C 语言程序。在无法列举出传递给函数的所有实参的类型和数目时，可以使用省略符形参。省略符暂停了类型检查机制。它们的出现告知编译器，当调用函数时，可以有 0 或多个实参，而实参的类型未知。省略符形参有下列两种形式：

         void foo(parm_list, ...); 
         void foo(...); 

## return 语句

return 语句用于结束当前正在执行的函数，并将控制权返回给调用此函数的函数。

### 主函数  main  的返回值

将主函数 main 返回的值视为状态指示器。返回 0 表示程序运行成功，其他大部分返回值则表示失败。非 0返回值的意义因机器不同而不同，**为了使返回值独立于机器，cstdlib头文件定义了两个预处理变量**（第 2.9.2 节），分别用于表示程序运行成功和失败：

	#include <cstdlib> 
	int main() 
	{ 
	 if (some_failure) 
	     return EXIT_FAILURE; 
	 else 
	     return EXIT_SUCCESS; 
	}

### 返回非引用类型 

在求解表达式时，如果需要一个地方储存其运算结果，编译器会创建一个没有命名的对象，这就是**临时对象。**

    // return plural version of word if ctr isn't 1 
         string make_plural(size_t ctr, const string &word, 
                                        const string &ending) 
         { 
             return (ctr == 1) ? word : word + ending; 
         }

这个函数**要么返回其形参 word 的副本**，**要么返回一个未命名的临时 string 对象**，这个临时对象是由字符串 word 和 ending 的相加而产生的。这两种情况下，**return 都在调用该函数的地方复制了返回的 string 对象**。

### 返回引用类型 
当函数返回引用类型时，没有复制返回值。相反，**返回的是对象本身**。
下面的函数返回两个 string 类型形参中较短的那个字符串的引用：
   

     // find longer of two strings 
             const string &shorterString(const string &s1, const string &s2) 
             { 
                 return s1.size() < s2.size() ? s1 : s2; 
             }

> 千万不要返回局部对象的引用(或指针)：当函数执行完毕时，将释放分配给局部对象的存储空间。此时，对局部对象的引用就会指向不确定的内存。
 
 

    // Disaster: Function returns a reference to a local object 
         const string &manip(const string& s) 
         { 
              string ret = s; 
              // transform ret in some way 
              return ret; // Wrong: Returning reference to a local object! 
         }

当函数执行完毕，字符串 ret 占用的储存空间被释放，函数返回值指向了对于这个程序来说不再有效的内存空间。
         
### 引用返回左值

返回引用的函数返回一个左值。因此，这样的函数可用于任何要求使用左值的地方：

    char &get_val(string &str, string::size_type ix) 
         { 
             return str[ix]; 
         } 
         int main()
         { 
             string s("a value"); 
             cout << s << endl;   // prints a value 
             get_val(s, 0) = 'A'; // changes s[0] to A 
     
             cout << s << endl;   // prints A value 
             return 0; 
         } 

**给函数返回值赋值可能让人惊讶，由于函数返回的是一个引用，因此这是正确的，该引用是被返回元素的同义词。**
如果不希望引用返回值被修改，返回值应该声明为 const： 
 

    const char &get_val(){}

## 递归

直接或间接调用自己的函数称为递归函数。递归函数必须定义一个终止条件；主函数 main 不能调用自身。

## 函数声明

正如变量必须先声明后使用一样，函数也必须在被调用之前先声明。一个函数只能定义一次，但是可声明多次。
函数声明由**函数返回类型、函数名和形参列表**组成。形参列表必须包括形参类型，但是不必对形参命名。这三个元素被称为**函数原型**，函数原型**描述了函数的接口。**

> 注:函数原型为定义函数的程序员和使用函数的程序员之间提供了接口。在使用函数时，程序员只对函数原型编程即可。

函数声明中的形参名会被忽略，如果在声明中给出了形参的名字，它应该用作辅
助文档：

    void print(int *array, int size); 

等于

    void print(int *, int);


### 在头文件中提供函数声明

变量可在头文件中声明（第 2.9 节），而在源文件中定义。同理，函数也应当在头文件中声明，并在源文件中定义。

### 默认实参

默认实参是通过给形参表中的形参提供明确的初始值来指定的。程序员可为一个或多个形参定义默认值。但是，如果有一个形参具有默认实参，那么，它后面所有的形参都必须有默认实参。
下面的函数创建并初始化了一个 string对象，用于模拟窗口屏幕。此函数为窗口屏幕的高、宽和背景字符提供了默认实参： 

    string screenInit(string::size_type height = 24, 
                           string::size_type width = 80, 
                           char background = ' ' );

调用包含默认实参的函数时，可以为该形参提供实参，也可以不提供。如果提供了实参，则它将覆盖默认的实参值；否则，函数将使用默认实参值。
但是，在一个文件中，只能为一个形参指定默认实参一次。

    // ff.h 
         int ff(int = 0); 
     
         // ff.cc 
         #include "ff.h" 
         int ff(int i = 0) { /* ... */ } // error 

## 局部对象

### 自动对象

只有当定义它的函数被调用时才存在的对象称为自动对象。形参也是自动对象。形参所占用的存储空间在调用函数时创建，而在函数结束时撤销。
 
### 静态局部对象 

一个**变量如果位于函数的作用域内，但生命期跨越了这个函数的多次调用**，这种变量往往很有用。则应该将这样的对象定义为 static（静态的）。
static 局部对象确保不迟于在程序执行流程**第一次经过该对象的定义语句时进行初始化**。这种对象一旦被创建，在**程序结束前都不会撤销**。

## 内联函数

7.3.2 节编写的那个返回两个 string 形参中较短的字符串的函数： 

    // find longer of two strings 
         const string &shorterString(const string &s1, const string &s2) 
         { 
             return s1.size() < s2.size() ? s1 : s2; 
         } 

**为这样的小操作定义一个函数的好处是：** 
•   阅读和理解函数 shorterString的调用，要比读一条用等价的条件表达式取代函数调用表达式并解释它的含义要容易得多。 
•   如果需要做任何修改，修改函数要比找出并修改每一处等价表达式容易得多。 
•   使用函数可以确保统一的行为，每个测试都保证以相同的方式实现。 
•   函数可以重用，不必为其他应用重写代码。

> 但是，将shorterString写成函数有一个潜在的缺点：调用函数比求解等价表达式要慢得多。在大多数的机上，调用函数都要做很多工作；调用前要先保存寄存器，并在返回时恢复；复制实参;程序还必须转向一个新位置执行。

### inline (内联)函数避免函数调用的开销

假设我们将 shorterString 定义为内联函数，则调用： 
     

    cout << shorterString(s1, s2) << endl;

 
 
在编译时将展开为： 
     

    cout << (s1.size() < s2.size() ? s1 : s2) 
              << endl; 

### 把  inline  函数放入头文件

**内联函数应该在头文件中定义**，这一点不同于其他函数。
inline 函数可能要在程序中定义不止一次，只要 inline 函数的定义在某个源文件中只出现一次，而且在所有源文件中，其定义必须是完全相同的。**把 inline 函数的定义放在头文件中，可以确保在调用函数时所使用的定义是相同的**，并且保证在调用点该函数的定义对编译器可见。 
在头文件中加入或修改 inline 函数时，使用了该头文件的所有源文件都必须重新编译。

## 类的成员函数

和任何函数一样，成员函数也包含下面四个部分： 
•   函数返回类型。 
•   函数名。 
•   用逗号隔开的形参表（也可能是空的）。 
•   包含在一对花括号里面的函数体。
前面三部分组成函数原型。函数原型定义了所有和函数相关的类型信息：函数返回类型是什么、函数的名字、应该给这个函数传递什么类型的实参。**函数原型必须在类中定义。**但是，**函数体则既可以在类中也可以在类外定义。编译器隐式地将在类内定义的成员函数当作内联函数。**

    class Sales_item { 
         public: 
             // operations on Sales_item objects 
             double avg_price() const; 
             bool same_isbn(const Sales_item &rhs) const 
                  { return isbn == rhs.isbn; } 
         // private members as before 
         private: 
             std::string isbn; 
             unsigned units_sold; 
             double revenue; 
         };

类的成员函数可以访问该类的 private 成员。

    bool same_isbn(const Sales_item &rhs) const 
             { return isbn == rhs.isbn; }

调用成员函数时，实际上是使用对象来调用的。例如，调用函数 same_isbn，是通过名为 total 的对象来执行 same_isbn函数的,如下: 
 

    if (total.same_isbn(trans));

每个成员函数都有一个额外的、隐含的形参将该成员函数与调用该函数的类对象捆绑在一起。**当调用名为 total的对象的same_isbn时**，**这个对象也传递给了函数**。而same_isbn 函数使用 isbn 时，就隐式地使用了调用该函数的对象的 isbn 成员。这个函数调用的效果是比较 total.isbn 和 trans.isbn 两个值。

### this  指针的引入

每个成员函数（除了在第 12.6 节介绍的 static 成员函数外）都有一个额外的、隐含的形参 this。**在调用成员函数时**，**形参 this初始化为调用函数的对象的地址。**

### const   成员函数的引入

const 改变了隐含的 this 形参的类型。在调用 total.same_isbn(trans) 时，隐含的 this 形参将是一个指向 total 对象的 const Sales_Item*类型的指针。

                 double avg_price() const; 
                 bool same_isbn(const Sales_item &rhs) const 
                      { return isbn == rhs.isbn; } 

用这种方式使用 const 的函数称为**常量成员函数**。由于 this 是指向 const 对象的指针，**const 成员函数不能修改调用该函数的对象。**因此，函数 avg_price 和函数 same_isbn 只能读取而不能修改调用它们的对象的数据成员。

> const 对象、指向 const 对象的指针或引用只能用于调用其 const 成员函数，如果尝试用它们来调用非 const成员函数，则是错误的。

### 在类外定义成员函数

在类的定义外面定义成员函数必须指明它们是类的成员。函数名： 

     Sales_item::avg_price

使用作用域操作符（第 1.2.2 节）指明函数 avg_price 是在类 Sales_item 的作用域范围内定义的。完整定义如下:

    double Sales_item::avg_price() const 
    {
        if (units_sold) 
             return revenue/units_sold; 
         else 
             return 0; 
    }

### 编写  Sales_item  类的构造函数

**构造函数是特殊的成员函数。**与其他成员函数不同，**构造函数和类同名**，而且**没有返回类型**。一个类可以有多个构造函数，每个构造函数必须有与其他构造函数不同数目或类型的形参。
Sales_item 类只需要显式定义一个构造函数：没有形参的**默认构造函数**。默认
构造函数说明当定义对象却没有为它提供（显式的）初始化式时应该怎么办。

    class Sales_item { 
         public: 
             Sales_item(): units_sold(0), revenue(0.0) { } 
         private: 
             unsigned units_sold; 
             double revenue; 
         };

构造函数是放在类的 public 部分的。通常构造函数会作为类的接口的一部分。

#### 构造函数和初始化列表

在**冒号和花括号之间的代码称为构造函数的初始化列表**。构造函数的初始化列表为类的一个或多个数据成员指定初值。
如果没有为一个类显式定义任何构造函数，编译器将自动为这个类生成默认构造函数。

### 类代码文件的组织

我们将类 Sales_item 放在名为 Sales_item.h 的文件中定义。任何需使用这个类的程序，都必须包含这个头文件。而 Sales_item 的成员函数的定义则应该放在名为 Sales_item.cc 的文件中。这个文件同样也必须包含 Sales_item.h 头文件。

## 重载函数

出现在**相同作用域中的两个函数**，如果**具有相同的名字而形参表不同**，则称为重载函数。
通过省去为函数起名并记住函数名字的麻烦，函数重载简化了程序的实现，使程
序更容易理解。编译器将根据所传递的实参类型和个数来判断调用的是哪个函数。
**任何程序都仅有一个 main 函数的实例。main 函数不能重载。** 

### 函数重载和重复声明的区别 

如果两个函数声明的返回类型和形参表完全匹配，则将第二个函数声明视为第一个的重复声明。

### 何时不重载函数名


    myScreen.home(); // we think this one! 
         myScreen.move(); 

重载过后的函数失去了原来函数名所包含的信息，如此一来，程序变得晦涩难懂了。

### 重载与作用域

如果局部地声明一个函数，则该函数将屏蔽而不是重载在外层作用域中声明的同名函数。由此推论，**每一个版本的重载函数都应在同一个作用域中声明。**

    void print(const string &);     
         void print(double);   // overloads the print function 
         void fooBar(int ival) 
         { 
             void print(int);   // new scope: hides previous instances of 
    print 
             print("Value: ");  // error: print(const string &) is hidden 
             print(ival); // ok: print(int) is visible 
             print(3.14); // ok: calls print(int); print(double) is hidden 
         } 

调用 print 时，编译器首先检索这个名字的声明，找到只有一个 int 型形参的 print 函数的局部声明。**一旦找到这个名字，编译器将不再继续检查这个名字是否在外层作用域中存在**，即编译器将认同找到的这个声明即是程序需要调用的函数，余下的工作只是检查该名字的使用是否有效。 
**因为在 C++中，名字查找发生在类型检查之前**。

> 一般来说，局部地声明函数是一种不明智的选择。函数的声明应放在头文件中。

### 函数匹配与实参转换

**函数重载确定**，即**函数匹配,是将函数调用与重载函数集合中的一个函数相关联的过程**。通过自动提取函数调用中实际使用的实参与重载集合中各个函数提供的形参做比较，编译器实现该调用与函数的匹配。匹配结果有三种可能： 
1.  **编译器找到与实参最佳匹配的函数，并生成调用该函数的代码。** 
2.  **找不到形参与函数调用的实参匹配的函数**，在这种情况下，**编译器将给出编译错误信息**。 
3.  **存在多个与实参匹配的函数**，但没有一个是明显的最佳选择。这种情况也是，**该调用具有二义性**。

### 重载确定的三个步骤

#### 1候选函数

函数重载确定的第一步是确定该调用所考虑的重载函数集合，该集合中的函数称为**候选函数**。候选函数是与被调函数同名的函数。

#### 2选择可行函数

第二步是从候选函数中选择一个或多个函数，它们能够用该调用中指定的实参来调用。因此，选出来的函数称为可行函数。可行函数必须满足两个条件：**第一**，函数的形参个数与该调用的**实参个数相同**；**第二**，每一个实参的类型必须与对应形参的**类型匹配**，**或者可被隐式转换为对应的形参类型。**

#### 3寻找最佳匹配

这里所谓“**最佳**”的细节将在下一节中解释，其**原则是实参类型与形参类型越接近则匹配越佳。**因此，实参类型与形参类型之间的**精确类型匹配比需要转换的匹配好**。

### 实参类型转换

为了确定最佳匹配，编译器将实参类型到相应形参类型转换划分等级。转换等级以降序排列如下： 
1.  **精确匹配**。实参与形参类型相同。 
2.  通过**类型提升**（即小升大）实现的匹配（第 5.12.2 节）。 
3.  通过**标准转换**（即大转小）实现的匹配（第 5.12.3 节）。（） 
4.  通过**类类型转换**实现的匹配（第 14.9 节将介绍这类转换）。
5.  
## 指向函数的指针

**函数指针是指指向函数而非指向对象的指针。**像其他指针一样，函数指针也指向某个特定的类型。函数类型由其返回类型以及形参表确定，而与函数名无关：

    // pf points to function returning bool that takes two const string 
    references 
         bool (*pf)(const string &, const string &);

这个语句将 pf 声明为指向函数的指针，它所指向的函数带有两个 const string& 类型的形参和 bool 类型的返回值。 
***pf 两侧的圆括号是必需的，否则变为函数名为pf，返回类型是一个指针的函数。**

### 用 typedef  简化函数指针的定义

函数指针类型相当地冗长。使用 typedef 为指针类型定义同义词，可将函数指针的使用大大简化：
 

    typedef bool (*cmpFcn)(const string &, const string &); 

**该定义表示 cmp Fcn是一种指向函数的指针类型的名字。该指针类型为“指向返回bool类型并带有两个const string引用形参的函数的指针”**。在要使用这种函数指针类型时，只需直接使用cmpFcn即可，不必每次都把整个类型声明全部写出来。

### 指向函数的指针的初始化和赋值

**在引用函数名但又没有调用该函数时，函数名将被自动解释为指向函数的指针。**假设有函数：

    // compares lengths of two strings 
         bool lengthCompare(const string &, const string &);
除了用作函数调用的左操作数以外，对 lengthCompare 的任何使用都被解释为如下类型的指针： 
 

    bool (*)(const string &, const string &);
    
**可使用函数名对函数指针做初始化或赋值：** 
 

        cmpFcn pf1 = 0;             // ok: unbound pointer to function 
         cmpFcn pf2 = lengthCompare; // ok: pointer type matches fu nction's 
    type 
         pf1 = lengthCompare;        // ok: pointer type matches fu nction's 
    type 
         pf2 = pf1;                  // ok: pointer types match 

**此时，直接引用函数名等效于在函数名上应用取地址操作符**：

         cmpFcn pf1 = lengthCompare; 
         cmpFcn pf2 = &lengthCompare;

> 函数指针只能通过同类型的函数或函数指针或 0 值常量表达式进行初始化或赋值。
指向不同函数类型的指针之间不存在转换。

### 通过指针调用函数

下面三种同作用：
   

     cmpFcn pf = lengthCompare; 
             lengthCompare("hi", "bye"); // direct call 
             pf("hi", "bye");            // equivalent call: pf1 implicitly dereferenced 隐式解引用
             (*pf)("hi", "bye");         // equivalent call: pf1 explicitly dereferenced

### 函数指针形参

第三个参数自动被视为（隐式）指向函数的指针

    // third parameter is a function type and is automatically  treated 
    as a pointer to 
     function 
         void useBigger(const string &, const string &, 
                        bool(const string &, const string &)); 

显示定义指向函数的指针：                
     

    // equivalent declaration: explicitly define the parameter as a 
    pointer to function 
         void useBigger(const string &, const string &, 
                        bool (*)(const string &, const string &));


### 返回指向函数的指针

函数可以返回指向函数的指针。

    int (*ff(int))(int*, int); 

 

> 阅读函数指针声明的最佳方法是从声明的名字开始由里而 外理解。

ff(int) 
**将 ff 声明为一个函数，它带有一个 int 型的形参。该函数返回** 

     int (*)(int*, int); 

**它是一个指向函数的指针**，所指向的函数返回 int 型并带有两个分别是 int* 型和 int 型的形参。 

使用 typedef 可使该定义更简明易懂：

    typedef int (*PF)(int*, int);
    PF ff(int);  // ff returns a pointer to function


### 指向重载函数的指针

    extern void ff(vector<double>);
    extern void ff(unsigned int); 
    void (*pf1)(unsigned int) = &ff; // ff(unsigned)

**指针的类型必须与重载函数的一个版本精确匹配**。如果没有精确匹配的函数，则对该指针的初始化或赋值都将导致编译错误：


 

         






