---
layout: article
title:  "c++ primer读书笔记--第十六章 模板和泛型编程"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记


在泛型编程中，我们所编写的类和函数能够多态地用于跨越编译时不相关的类型。一个类或一个函数可以用来操纵多种类型的对象。标准库中的容器、迭代器和算法是很好的泛型编程的例子。

## 16.1. 模板定义

### 16.1.1.  定义函数模板

    template <typename T> 
         int compare(const T &v1, const T &v2) 
         { 
             if (v1 < v2) return -1; 
             if (v2 < v1) return 1; 
             return 0; 
         }

模板定义以关键字 template开始，后接模板形参表，模板形参表是用尖括号括住的一个或多个模板形参的列表，形参之间以逗号分隔。 **模板形参表不能为空**。
**编译器将实例化 compare 的两个不同版本，编译器将用 int 代替 T 创建第一个版本，并用 string 代替 T 创建第二个版本。**

#### 模板形参表
模板形参表很像函数形参表，函数形参表定义了特定类型的局部变量但并不初始化那些变量，在运行时再提供实参来初始化形参。 

在 compare 内部，可以使用名字 T 引用一个类型，T表示哪个实际类型由编译器根据所用的函数而确定。 

#### 使用函数模板
使用函数模板时，编译器会推断哪个（或哪些）模板实参绑定到模板形参。一旦编译器确定了实际的模板实参，就称它实例化了函数模板的一个实例。
推导出实际模板实参后，编译器使用实参代替相应的模板形参产生编译该
版本的函数。

#### inline  函数模板
说明符放在模板
形参表之后、返回类型之前

     template <typename T> inline T min(const T&, const T&);

### 16.1.2.  定义类模板

    template <class Type> class Queue { 
         public: 
             Queue ();                // default constructor 
             Type &front ();          // return element from head of Queue 
             const Type &front () const; 
             void push (const Type &); // add element to back of Queue 
             void pop();              // remove element from head of Queue 
             bool empty() const;      // true if no elements in the Queue 
         private: 
             // ... 
         };

#### 使用类模板

        Queue<int> qi;                 // Queue that holds ints 
        Queue< vector<double> > qc;    // Queue that holds vectors of doubles
 
与调用函数模板形成对比，**使用类模板时，必须为模板形参显式指定实参**


### 16.1.3.  模板形参

#### 模板形参作用域
模板形参的名字可以在声明为模板形参之后直到模板声明或定义的末尾处
使用。
模板形参可以是表示类型的类型形参，也可以是表示常量表达式的非类型形参(即内置类型int等)。

下面的全局类型T会被模板函数屏蔽:

    typedef double T; 
         template <class T> T calc(const T &a, const T &b) 
         { 
              T tmp = a; 
              return tmp; 
         } 

与全局作用域中声明的对象、函数或类型同名的模板形参会屏蔽全局名字。

#### 用作模板形参的名字不能在模板内部重用。

    template <class T> T calc(const T &a, const T &b) 
         { 
             typedef double T; // error: redeclares template parameter T 
             T tmp = a; 
             return tmp; 
         }

        // error: illegal reuse of template parameter name V 
         template <class V, class V> V calc(const V&, const V&);

#### 模板声明

    template <class T> int compare(const T&, const T&) ; 

### 16.1.4.  模板类型形参

类型形参由关键字 class 或 typename 后接说明符构成。

#### typename  与  class   的区别
在函数模板形参表中，关键字 typename 和 class 具有相同含义，可以互换使用,但是一些旧程序只能支持class

#### **在模板定义内部指定类型**
除了定义数据成员或函数成员之外，类还可以定义类型成员。例如 size_type。

    template <class Parm, class U> 
         Parm fcn(Parm* array, U value) 
         { 
             Parm::size_type * p; 
         }

如果要在函数模板内部使用这样的类型。**必须显式地这样做（使用typename）**，因为编译器不能通过检查得知由类型形参定义的名字何时是一个类型何时是一个值。
默认情况下，编译器假定这样的名字指定数据成员，而不是类型。 

    typename Parm::size_type * p; // ok: declares p to be a pointer

### 16.1.5.  非类型模板形参
**在调用函数时非类型形参将用值代替，值的类型在模板形参表中指定**。例如，下面的函数模板声明了 array_init是一个含有一个类型模板形参和一个非类型模板形参的函数模板。函数本身接受一个形参，该形参是数组的引用:

    template <class T, size_t N> void array_init(T (&parm)[N]) 
         { 
             for (size_t i = 0; i != N; ++i) { 
                 parm[i] = 0; 
             } 
         }

**模板非类型形参是模板定义内部的常量值**，在需要常量表达式的时候，可使用非类型形参（例如，像这里所做的一样）指定数组的长度。如上面的N。

当调用 array_init 时，编译器从数组实参计算非类型形参的值： 
 

    int x[42]; 
     array_init(x);  // instantiates array_init(int (&)[42]) 

### 16.1.6.  编写泛型程序
编写模板时，代码不可能针对特定类型，但模板代码总是要对将使用的类型做一些假设。例如，虽然 compare 函数从技术上说任意类型都是有效的，但实际上，实例化的版本可能是非法的。 

**产生的程序是否合法，取决于函数中使用的操作以及所用类型支持的操作。**
如果用不支持 < 操作符的对象调用 compare，则该调用将是无效的： 
 

    Sales_item item1, item2; 
    cout << compare(item1, item2) << endl;
程序会出错。Sales_item 类型没有定义 < 操作符，所以该程序不能编译。

#### 编写独立于类型的代码
编写模板代码时，对实参类型的要求尽可能少是很有益的。 
上述条件说明了编写泛型代码的两个重要原则：
•   模板的形参是 const 引用。
•   函数体中的测试只用 < 比较。

通过**将形参设为 const 引用，就可以允许使用不允许复制的类型**。大多数类型（包括内置类型和我们已使用过的除 IO 类型之外的所有标准库的类型）都允许复制。但是，也有不允许复制的类类型。将形参设为 const 引用，保证这种类型可以用于 compare 函数。 

#### **模板的编译时错误**
编译模板时，编译器可能会在三个阶段中标识错误：
第一阶段是**编译模板定义本身时**。在这个阶段中编译器一般不能发现许多错误，可以检测到诸如漏掉分号或变量名拼写错误一类的语法错误。 

第二个错误检测时间是**在编译器见到模板的使用时**。在这个阶段，编译器仍没有很多检查可做。对于函数模板的调用，许多编译器只检查实参的数目和类型是否恰当，编译器可以检测到实参太多或太少，也可以检测到假定类型相同的两个实参是否真地类型相同。对于类模板，编译器可以检测提供的模板实参的正确数目。 
第三个错误检测是在**实例化(编译器为模板函数生成对应的带有指定参数的函数)的时候**，只有在这个时候可以发现类型相关的错误。根据编译器管理实例化的方式有可能在链接时报告这些错误。

## 16.2. 实例化
模板是一个蓝图，它本身不是类或函数。编译器用模板产生指定的类或函数
的特定类型版本。

#### 类的实例化

     Queue<int> qi; 

上面代码编译器自动创建名为 Queue 的类。实际上，编译器通过重新编写 Queue 模板，**用类型 int 代替模板形参的每次出现而创建 Queue 类**。实例化的类就像已经编写的一样：

    template <class Type> class Queue<int> {   //类名变成Queue<int>
         public: 
             Queue();                  // this bound to Queue<int>* 
             int &front();             // return type bound to int 
             const int &front() const; // return type bound to int 
             void push(const int &);   // parameter type bound to int 
             void pop();               // type invariant code 
             bool empty() const;       // type invariant code 
         private: 
             // ... 
         }; 

#### 类模板形参是必需的
**类模板不定义类型，只有特定的实例才定义了类型**。特定的实例化是通过提供模板实参与每个模板形参匹配而定义的。模板实参在用逗号分隔并用尖括号括住的列表中指定： 

    Queue<int> qi;         // ok: defines Queue that holds ints 

#### 函数模板实例化

            compare(1, 0);             // ok: binds template parameter to int 
            compare(3.14, 2.7);        // ok: binds template parameter to double

### 16.2.1.  模板实参推断
第一个调用 compare(1, 0) 中，实参为 int 类型；第二个调用compare(3.14, 2.7) 中，实参为 double类型。从函数实参确定模板实参的类型和值的过程叫做模板实参推断。

#### 多个类型形参的实参必须完全匹配

    short si=1;
    compare(si, 1024);

这个调用是错误的，因为调用 compare 时的实参类型不相同。

如果 compare 的设计者想要允许实参的常规转换，则函数必须用两个类型形参来定义：

    template <typename A, typename B> 
         int compare(const A& v1, const B& v2) 
         { 
             if (v1 < v2) return -1; 
             if (v2 < v1) return 1; 
             return 0; 
         }

#### 类型形参的实参的受限转换

         short s1, s2; 
         int i1, i2; 
         compare(i1, i2);           // ok: instantiate compare(int, int) 
         compare(s1, s2);           // ok: instantiate compare(short, short) 

一般而论，不会转换实参以匹配已有的实例化，相反，会产生新的实例。
编译器只会执行两种转换： 
•   **const 转换**：**接受 const 引用或 const 指针的函数可以分别用非 const 对象的引用或指针来调用**，无须产生新的实例化。
**如果函数接受非引用类型，形参类型实参都忽略 const**，即，无论传递 const 或非 const 对象给接受非引用类型的函数，都使用相同的实例化。 

•   **数组或函数到指针的转换**：如果模板形参不是引用类型，则对数组或函数
类型的实参应用常规指针转换。数组实参将当作指向其第一个元素的指
针，函数实参当作指向函数类型的指针。

非引用形参：

     template <typename T> T fobj(T, T); // arguments are copied
     string s1("a value"); 
     const string s2("another value"); 
     fobj(s1, s2);     // ok: calls f(string, string), const is  ignored

引用形参：

    template <typename T> 
    T fref(const T&, const T&);       // reference arguments 
    fref(s1, s2);     // ok: non const object s1 converted to const reference

#### 应用于非类型参数的常规转换
用普通类型定义的形参可以使用常规转换（第 7.1.2 节），下面的函数模板 sum 有两个形参： 
 

    template <class Type> Type sum(const Type &op1, int op2) 
     { 
         return op1 + op2; 
     }

上述模板函数：实参传给int型形参op2时能进行隐式类型转换（double，short等）。

#### 模板实参推断与函数指针
假定有一个函数指针指向返回 int值的函数，该函数接受两个形参，都是 const int 引用，**可以用该指针指向 compare 的实例化。** 
 

    template <typename T> int compare(const T&, const T&); 
    // pf1 points to the instantiation int compare (const int&, const int&) 
    int (*pf1) (const int&, const int&) = compare; 

pf1 的类型是一个指针，指向“接受两个 const int& 类型形参并返回 int 值的函数”，**形参的类型决定了 T 的模板实参的类型为 int 型，指针 pf1 引用的是将 T 绑定到 int 的实例化。**

**获取函数模板实例化的地址**的时候，上下文必须是这样的：**它允许为每个模板形参确定唯一的类型或值。**

### 16.2.2.  函数模板的显式实参
**在某些情况下，不可能推断模板实参的类型**。当函数的返回类型必须与形参表中所用的所有类型都不同时，最常出现这一问题。在这种情况下，**有必要覆盖模板实参推断机制，并显式指定为模板形参所用的类型或值**。

    // neither T nor U works as return type 
         sum(3, 4L); // second type is larger; want U sum(T, U) 
         sum(3L, 4); // first type is larger; want T sum(T, U) 

解决这一问题的一个办法，可能是强制 sum 的调用者将较小的类型强制转换（第 5.12.4 节）为希望作为结果使用的类型：

    // ok: now either T or U works as return type 
         int i; short s; 
         sum(static_cast<int>(s), i); // ok: instantiates int sum(i nt, int)

#### 在返回类型中使用类型形参
指定返回类型的一种方式是引入第三个模板形参，它必须由调用者显式指定： 

	 template <class T1, class T2, class T3> 
     T1 sum(T2, T3);

这个版本增加了一个模板形参以指定返回类型，调用者必须在每次调用 sum 时为该形参显式提供实参。

    long val3 = sum<long>(i, lng); // ok: calls long sum(int, long)

显示指定< long >直接对应模板函数第一个参数T1
如果这样编写 sum 函数： 
     

        // poor design: Users must explicitly specify all three template parameters 
         template <class T1, class T2, class T3> 
         T3 alternative_sum(T2, T1);

则总是必须为所有三个形参指定实参：

    // error: can't infer initial template parameters 
     long val3 = alternative_sum<long>(i, lng); 
     
     // ok: All three parameters explicitly specified
     long val2 = alternative_sum<long, int, long>(i, lng);
     
#### 显式实参与函数模板的指针
可以使用显式模板实参的另一个例子是第 16.2.1 节中有二义性程序，通过使用显式模板实参能够消除二义性： 
 

        template <typename T> int compare(const T&, const T&); 
         // overloaded versions of func; each take a different function pointer type 
         void func(int(*) (const string&, const string&)); 
         void func(int(*) (const int&, const int&)); 
         func(compare<int>); // ok: explicitly specify which version of compare 

需要在调用中传递 compare 实例给名为 func 的重载函数。显式模板形参需要指出应使用哪个 compare 实例以及调用哪个 func 函数。

## 16.3. 模板编译模型
当编译器看到模板定义的时候，它不立即产生代码。只有在看到用到模板时，如**调用了函数模板或调用了类模板的对象的时候，编译器才产生特定类型的模板实例。**

一般而言，**当调用函数的时候，编译器只需要看到函数的声明**。
类似地，**定义类类型的对象时，类定义必须可用，但成员函数的定义不是必须存在的。因此，应该将类定义和函数声明放在头文件中，而普通函数和类成员函数的定义放在源文件中。**

**模板则不同：要进行实例化，编译器必须能够访问定义模板的源代码**。当调用函数模板或类模板的成员函数的时候，编译器需要函数定义，需要那些通常放在源文件中的代码。

标准 C++ 为编译模板代码定义了两种模型。在两种模型中，构造程序的方式很大程度上是相同的：**类定义和函数声明放在头文件中，而函数定义和成员定义放在源文件中**。两种模型的不同在于，编译器怎样使用来自源文件的定义。如本书所述，**所有编译器都支持第一种模型，称为“包含”模型**，只有一些编译器支持第二种模型，“分别编译”模型。 
 
要编译使用自己的类模板和函数模板的代码，必须查阅编译器的用户指南，看看编译器怎样处理实例化。

### 包含编译模型
在包含编译模型中，编译器必须看到用到的所有模板的定义。一般而言，可以通过在声明函数模板或类模板的头文件中添加一条 #include指示使定义可用，该 #include 引入了包含相关定义的源文件：

    	 // header file utlities.h 
         #ifndef UTLITIES_H // header gaurd (Section 2.9.2, p. 69) 
         #define UTLITIES_H 
         template <class T> int compare(const T&, const T&); 
         // other declarations 
     
         #include "utilities.cc" // 取得compare的定义
         #endif 
     
         // implemenatation file utlities.cc 
         template <class T> int compare(const T &v1, const T &v2) 
         { 
             if (v1 < v2) return -1; 
             if (v2 < v1) return 1; 
             return 0; 
         } 

这一策略使我们能够保持头文件和实现文件的分享，但是需要保证编译器在编译使用模板的代码时能看到两种文件。

某些使用包含模型的编译器，特别是**较老的编译器，可以产生多个实例**。**如果两个或多个单独编译的源文件使用同一模板，这些编译器将为每个文件中的模板产生一个实例。**
在链接的时候，或者在预链接阶段，编译器会选择一个实例化而丢弃其他的。在这种情况下，如果有许多实例化同一模板的文件，编译时性能会显著降低。

#### 分别编译模型
在分别编译模型中，编译器会为我们跟踪相关的模板定义（使用 export  关键字让一个模板只能定义为导出一次）。

    export template <typename Type> 
         Type sum(Type t1, Type t2)
         
通常，类声明必须放在头文件中，**头文件中的类定义体不应该使用关键字 export，如果在头文件中使用了 export，则该头文件只能被程序中的一个源文件使用**。


#### 类模板中的名字查找
模板包含两种名字： 
1.  独立于模板形参的那些名字 
2.  依赖于模板形参的那些名字 
设计者的责任是，保证所有不依赖于模板形参的名字在模板本身的作用
域中定义
模板用户的责任是，在实例化类模板的成员或函数模板的时候，用户必须保证这些声明是可见的。

适当使用头文件的结构良好的程序都容易满足这两个要求。
**模板的作者**应提供头文件，**该头文件包含在类模板或在其成员定义中使用的所有名字的声明**。
**用户必须保证包含了模板类型的头文件**，以及定义用作成员类型的头文件。 

## 16.4. 类模板成员

### Queue   的实现策略

![这里写图片描述](http://img.blog.csdn.net/20150918210140908)

1.  QueueItem 类表示 Queue 的链表中的节点，该类有两个数据成员item 和 next： 
o  item 保存 Queue 中元素的值，它的类型随 Queue 的每个实例而变化。 
o  next 是队列中指向下一 QueueItem 对象的指针。 Queue中的每个元素保存在一个 QueueItem 对象中。 
2.  Queue 类将提供第 16.1.2 节描述的接口函数，Queue类也有两个数据成员：head 和 tail，这些成员是 QueueItem 指针。

### QueueItem   类


    template <class Type> class QueueItem { 
         // private class: no public section 
             QueueItem(const Type &t): item(t), next(0) { } 
             Type item;           // value stored in this element 
             QueueItem *next;     // pointer to next element in the Queue 
         }; 

QueueItem 类为私有类——它没有公用接口。我们这个类只是为实现 Queue，并不想用于一般目的，因此，它没有公用成员。需要将 Queue 类设为 QueueItem 类的友元，以便 Queue 类成员能够访问 QueueItem 的成员。


### Queue   类 

    template <class Type> class Queue { 
         public: 
             // empty Queue 
             Queue(): head(0), tail(0) { } 
             // copy control to manage pointers to QueueItems in the Queue 
             Queue(const Queue &Q): head(0), tail(0) 
                                           { copy_elems(Q); } 
             Queue& operator=(const Queue&); 
             ~Queue() { destroy(); } 
             // return element from head of Queue 
             // unchecked operation: front on an empty Queue is undefined 
             Type& front()             { return head->item; } 
             const Type &front() const { return head->item; } 
             void push(const Type &);   // add element to back of Queue 
             void pop ();           // remove element from  head of Queue 
             bool empty () const {      // true if no elements in the Queue 
                 return head == 0; 
             } 
         private: 
             QueueItem<Type> *head;     // pointer to first element in Queue 
             QueueItem<Type> *tail;     // pointer to last element in Queue 
             // utility functions used by copy constructor, assignment, and  destructor 
             void destroy();                // delete all the elements 
             void copy_elems(const Queue&); // copy elements from p arameter 
         };

Queue 类实现了几个成员函数： 
•   默认构造函数，将 head 和 tail 指针置 0,指明当前 Queue 为空。 
•   复制构造函数，初始化 head 和 tail，并调用 copy_elems从它的初始器复制元素。 
•   几个 front 函数，返回头元素的值。这些函数不进行检查：像标准 queue 中的类似操作一样，用户不能在空 Queue 上运行 front 函数。 
•   empty 函数，返回 head 与 0 的比较结果。如果 head 为 0，Queue 为空；否则，Queue 是非空的。

### 模板作用域中模板类型的引用 
通常，当使用类模板的名字的时候，必须指定模板形参。这一规则有个例外：**在类本身的作用域内部，可以使用类模板的非限定名。**
因此，复制构造函数定义其实等价于： 

    Queue<Type>(const Queue<Type> &Q): head(0), tail(0) 
                     { copy_elems(Q); } 
                     

编译器不会为类中使用的其他模板的模板形参进行这样的推断，因此，在声明伙伴类 QueueItem 的指针时，必须指定类型形参：
 

    QueueItem<Type> *head;    // pointer to first element in Queue

即，在 Queue<int> 实例化的内部，head 和 tail 的类型是 QueueItem<int>*。

### 16.4.1.  类模板成员函数

•   必须以关键字 template 开关，后接类的模板形参表。 
•   必须指出它是哪个类的成员。 
•   类名必须包含其模板形参

在类外定义的 Queue 类的成员函数的开关应该是： 
   

      template <class T> ret-type Queue<T>::member-name

#### destroy   函数 

为了举例说明在类外定义的类模板成员函数，我们来看 destroy 函数： 
 

    template <class Type> void Queue<Type>::destroy() 
     { 
         while (!empty()) 
             pop(); 
     }

这个定义可以从左至右读作： 
•   用名为 Type 的类型形参定义一个函数模板； 
•   它返回 void； 
•   它是在类模板 Queue< Type > 的作用域中(即它是Queue< Type >的成员函数)。

#### pop   函数 
pop 成员的作用是除去 Queue 的队头值： 

     template <class Type> void Queue<Type>::pop() 
     { 
         QueueItem<Type>* p = head; // keep pointer to head so 
         head = head->next;         // head now points to next element 
         delete p;                  // delete old head element
    }

#### push  函数

    template <class Type> void Queue<Type>::push(const Type &val) 
         { 
             QueueItem<Type> *pt = new QueueItem<Type>(val); 
             if (empty()) 
                 head = tail = pt; 
             else { 
                 tail->next = pt; 
                 tail = pt; 
             } 
         }

#### copy_elems  函数
设计该函数的目的是供赋值操作符和复制构造函数使用：

    template <class Type> 
         void Queue<Type>::copy_elems(const Queue &orig) 
         { 
             for (QueueItem<Type> *pt = orig.head; pt; pt = pt->next) 
                 push(pt->item); // copy the element 
          } 

#### 类模板成员函数的实例化
**对象的模板实参能够确定成员函数模板形参**，这一事实意味着，调用类模板成员函数比调用类似函数模板更灵活。

    Queue<int> qi; // instantiates class Queue<int> 
     short s = 42; 
     int i = 42; 
     // ok: s converted to int and passed to push 
     qi.push(s); // instantiates Queue<int>::push(const int&) 
     qi.push(i); // uses Queue<int>::push(const int&)

成员函数的类型在使用时才进行实例化。
    
     Queue<string> qs; 
     qs.push("hello"); // instantiates Queue<string>::push

#### 何时实例化类和成员
Queue 类中的 QueueItem成员是指针。类模板的指针定义不会对类进行实
例化，只有用到这样的指针时才会对类进行实例化。
Queue 类中的 QueueItem成员是指针。类模板的指针定义不会对类进行实
例化，只有用到这样的指针时才会对类进行实例化。
例如push函数中的：

    QueueItem<Type> *pt = new QueueItem<Type>(val);

### 16.4.2.  非类型形参的模板实参
Screen 类定义一个新版本：

    template <int hi, int wid> 
         class Screen { 
         public: 
             // template nontype parameters used to initialize data  members 
             Screen(): screen(hi * wid, '#'), cursor (0), 
                       height(hi), width(wid) { } 
             // ... 
         private: 
             std::string            screen; 
             std::string::size_type cursor; 
             std::string::size_type height, width; 
         };

这个模板有两个形参，均为非类型形参。当用户定义 Screen 对象时，必须为每个形参提供常量表达式以供使用。类在默认构造函数中使用这些形参设置默认 Screen 的尺寸。

    Screen<24,80> hp2621; // screen 24 lines by 80 characters

非类型模板实参编译时必须是常量表达式。

### 16.4.3.  类模板中的友元声明
1.  普通非模板类或函数的友元声明，将友元关系授予明确指定的类或函数。 
2.  类模板或函数模板的友元声明，授予对友元所有实例的访问权。 
3.  只授予对类模板或函数模板的特定实例的访问权的友元声明。

#### 普通友元 
非模板类或非模板函数可以是类模板的友元： 
 

    template <class Type> class Bar { 
         friend class FooBar; 
         friend void fcn(); 
     }; 

#### 一般模板友元关系 
友元可以是类模板或函数模板： 

     template <class Type> class Bar { 
         template <class T> friend class Foo1; //所有类型的Foo1可以访问Bar类
         template <class T> friend void templ_fcn1(const T&); 
        
     };

**Foo1 的友元声明是说，Foo1 的任意实例都可以访问 Bar 的任意实例的私有元素**，类似地，temp_fcn1 的任意实例可以访问 Bar 的任意实例。

#### 特定的模板友元关系
    

     template <class T> class Foo2; 
     template <class T> void templ_fcn2(const T&); 
     template <class Type> class Bar { 
          friend class Foo2<char*>;  //特定类型char*的Foo2可以访问Bar类
          friend void templ_fcn2<char*>(char* const &); 
     };
     
**下面形式的友元声明更为常见：** 
 

    template <class T> class Foo3; 
     template <class T> void templ_fcn3(const T&); 
     template <class Type> class Bar { 
         friend class Foo3<Type>;  //根据模板Bar的形参类型来指定特定类型的友元
         friend void templ_fcn3<Type>(const Type&); 
     };
     
### 16.4.4. Queue和QueueItem   的友元声明
QueueItem 类必须将 Queue 类设为友元：

    template <class Type> class Queue; 
         template <class Type> class QueueItem { 
             friend class Queue<Type>; 
          }; 

#### Queue   输出操作符
因为希望能够输出任意类型 Queue的内容，所以需要将输出操作符也设为模板：

    template <class Type> 
         ostream& operator<<(ostream &os, const Queue<Type> &q) 
         { 
             os << "< "; 
             QueueItem<Type> *p; 
             for (p = q.head; p; p = p->next) 
                     os << p->item << " "; 
             os <<">"; 
             return os; 
         } 

为使输出操作符重载函数能访问QueueItem的item，next和Queue的head，必须在其模板类中输出（<<）操作符声明为友元函数：

    template <class T> 
         std::ostream& operator<<(std::ostream&, const Queue<T>&); 
         
         template <class Type> class QueueItem { 
             friend class Queue<Type>; 
             // needs access to item and next 
             friend std::ostream& 
             operator<< <Type> (std::ostream&, const Queue<Type>&); 
         }; 
         template <class Type> class Queue { 
             // needs access to head 
             friend std::ostream& 
             operator<< <Type> (std::ostream&, const Queue<Type>&); 
         };

#### 类型依赖性与输出操作符

    os << p->item << " "; 

这条语句的item的类型必须支持输出操作符

### 16.4.5.  成员模板
任意类（模板或非模板）可以拥有本身为类模板或函数模板的成员，这种成员称为成员模板，成员模板不能为虚。

#### 定义成员模板

    template <class Type> class Queue { 
         public: 
             template <class It> 
             Queue(It beg, It end): 
                   head(0), tail(0) { copy_elems(beg, end); } 
             // replace current Queue by contents delimited by a pair of iterators 
             template <class Iter> void assign(Iter, Iter); 
             // rest of Queue class as before 
         private: 
             template <class Iter> void copy_elems(Iter, Iter); 
         };

成员声明的开关是自己的模板形参表。构造函数和 assign成员各有一个模板类型形参，**这些函数使用该类型形参作为其函数形参的类型，它们的函数形参是指明要复制元素范围的迭代器**。

#### 在类外部定义成员模板

    template <class T> template <class Iter> 
         void Queue<T>::assign(Iter beg, Iter end) 
         { 
             destroy();            // remove existing elements in this Queue 
             copy_elems(beg, end); // copy elements from the input range 
         }
当成员模板是类模板的成员时，它的定义必须包含类模板形参以及自己的模板形参。

    template <class T> template <class Iter> 

#### 成员模板和实例化
与其他成员一样，成员模板只有在程序中使用时才实例化。
**成员模板有两种模板形参：由类定义的和由成员模板本身定义的。**
  

      short a[4] = { 0, 3, 6, 9 }; 
         // instantiates Queue<int>::Queue(short *, short *) 
         Queue<int> qi(a, a + 4); // copies elements from a into qi 
         vector<int> vi(a, a + 4);
         // instantiates Queue<int>::assign(vector<int>::iterator,vector<int>::iterator) 
         qi.assign(vi.begin(), vi.end());

该构造函数本身模板形参的类型由编译器根据 a 和 a+4 的类型推断，而该类型为 short 指针。因此，qi 的定义将实例化:
     
     void Queue<int>::Queue(short *, short *);

这个构造函数的效果是，从名为 a 的数组中复制 short 类型的元素到 qi。

对 assign 的调用将实例化 qi 的成员。qi 具有 Queue< int > 类型，因此，这个调用将实例化名为 assign 的 Queue< int >成员。该函数本身是函数模板，像对任意其他函数模板一样，编译器从传给调用的实参推断 assign 的模板实参，推断得到的类型是 vector< int >::iterator，即，这个调用将实例化 
 

    void Queue<int>::assign(vector<int>::iterator, 
                             vector<int>::iterator);
                             

### 16.4.7类模板的 static 成员

    template <class T> class Foo { 
         public: 
            static std::size_t count() { return ctr; }
         private: 
            static std::size_t ctr; 
         };

Foo 类的每个实例化都共享static 成员
  

      Foo<int> fi; //fi共享Foo<int>类的static成员
         Foo<string> fs;//fs共享Foo<string>类的static成

#### 使用类模板的  static  成员
可以通过类类型的对象访问类模板的 static成员，或者通过使用作用域操作符(带类型)直接访问成员。

    ct = fi.count();              // ok: uses
     ct = Foo<int>::count();  // 

**与任意其他成员函数一样，static成员函数只有在程序中使用时才进行实例化。**

## 16.5. 一个泛型句柄类
**句柄类就是智能指针类。**
在第十五章定义了两个句柄类：Sales_item 类（第 15.8 节）和 Query 类（第 15.9 节）。这两个类管理继承层次中对象的指针，句柄的用户不必管理指向这些对象的指针，用户代码可以使用句柄类来编写。**句柄能够动态分配和释放相关继承类的对象，并且将所有“实际”工作转发给继承层次中的底层类。**
本节将实现一个**泛型句柄类（generic handle class）**，提供管理使用计数和基础对象的操作。

### 16.5.1.  定义句柄类
Handle 类行为类似于指针：复制 Handle对象将不会复制基础对象，复制
之后，两个 Handle 对象将引用同一基础对象。

    template <class T> class Handle { 
         public: 
             // 句柄类构造函数绑定一个对象 
             Handle(T *p = 0): ptr(p), use(new size_t(1)) { } 
             
             // overloaded operators to support pointer behavior
             T& operator*(); 
             T* operator->(); 
             const T& operator*() const; 
             const T* operator->() const; 
             
             // copy control: normal pointer behavior, but last Handle deletes the object 
             Handle(const Handle& h): ptr(h.ptr), use(h.use) 
                                                 { ++*use; } 
             Handle& operator=(const Handle&); 
             ~Handle() { rem_ref(); } 
         private: 
             T* ptr;          // 共享对象
             size_t *use;     // count of how many Handles point to *ptr 
             void rem_ref() 
                 { if (--*use == 0) { delete ptr; delete use; } 
         }; 
         
    template <class T> 
         inline Handle<T>& Handle<T>::operator=(const Handle &rhs) 
         { 
             ++*rhs.use;      // protect against self-assignment 
             rem_ref();       // decrement use count and delete poi nters if 
    needed 
             ptr = rhs.ptr; 
             use = rhs.use; 
             return *this; 
         }
         
    //如果 Handle 没有绑定到对象，则试图访问对象将抛出一个异常。
    template <class T> inline T& Handle<T>::operator*() 
         { 
             if (ptr) return *ptr; 
             throw std::runtime_error 
                            ("dereference of unbound Handle");
                            
    template <class T> inline T* Handle<T>::operator->() 
         { 
             if (ptr) return ptr; 
             throw std::runtime_error 
                            ("access through unbound Handle"); 
         } 

### 16.5.2.  使用句柄

     
           // user allocates but must not delete the object to which the Handle is attached 
           Handle<int> hp(new int(42)); 
           { 
               Handle<int> hp2 = hp; // copies pointer; use count incremented 
               cout << *hp << " " << *hp2 << endl; // prints 42 42 
               *hp2 = 10;           // changes value of shared unde rlying int 
           }   // hp2 goes out of scope; use count is decremented 
           cout << *hp << endl; // prints 10 

#### 使用  Handle  对象对指针进行使用计数

可以通过用 Handle<Item_base>: 对象代替 Item_base 指针而删去复制控制成员：

    class Sales_item { 
         public: 
             // default constructor: unbound handle 
             Sales_item(): h() { } 
             // copy item and attach handle to the copy 
             Sales_item(const Item_base &item): h(item.clone()) { } 
             // no copy control members: synthesized versions work 
            // member access operators: forward their work to the Handle class 
             const Item_base& operator*() const { return *h; } 
             const Item_base* operator->() const 
                                    { return h.operator->(); } 
         private: 
             Handle<Item_base> h; // 计数句柄，use-counted handle 
         };

因为 **Sales_item 的这个版本没有指针成员，所以不需要复制控制成员**，Sales_item 的这个版本可以安全地使用合成的复制控制成员。管理使用计数和相关 Item_base 对象的工作在 Handle 内部完成。

调用 net_price 函数的语句值得仔细分析一下（**iter为Sales_item迭代器**）： 
 

    sum += (*iter)->net_price(items.count(*iter));


•    (*iter) 返回 Sales_item对象
•   因此，(*iter)-> 返回 h.operator->()
•    h.operator->() 返回该 Handle 对象保存的 Item_base 指针ptr。 
•   编译器对该 Item_base 指针解引用，并调用指针所指对象的 net_price 成员。
