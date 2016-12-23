---
layout: article
title:  "c++ primer读书笔记--第九章 顺序容器"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

![顺序容器](http://img.blog.csdn.net/20150707140021305)


## 顺序容器的定义

为了定义一个容器类型的对象，必须先包含相关的头文件

    #include <vector> 
     #include <list> 
     #include <deque>

**所有的容器都是类模板**（第 3.3 节）。要定义某种特殊的容器，必须在容器名后加一对尖括号，尖括号里面提供容器中存放的元素的类型： 
 

    vector<string>    svec;       // empty vector that can hold strings 
         list<int>         ilist;      // empty list that can hold ints 
         deque<Sales_item> items;      // empty deque that holds Sales_items

**所有容器类型都定义了默认构造函数**，用于创建指定类型的空容器对象默
认构造函数不带参数。

### 容器元素的初始化

除了默认构造函数，容器类型还提供其他的构造函数，使程序员可以指定元素初值.

![这里写图片描述](http://img.blog.csdn.net/20150707140044358)
 


  
### 初始化为一段元素的副本
尽管不能直接将一种容器内的元素复制给另一种容器，但系统允许通过传递一对迭代器（第 3.4 节）间接实现该实现该功能。使用迭代器时，不要求容器类型相同。容器内的元素类型也可以不相同，只要它们相互兼容，能够将要复制的元素转换为所构建的新容器的元素类型，即可实现复制。

    // initialize slist with copy of each element of svec 
         list<string> slist(svec.begin(), svec.end());

允许通过使用内置数组中的一对指针初始化容器:
 

    char *words[] = {"stately", "plump", "buck", "mulligan"}; 
     
     // calculate how many elements in words 
     size_t words_size = sizeof(words)/sizeof(char *); 
     
     // use entire array to initialize words2 
     list<string> words2(words, words + words_size);

### 分配和初始化指定数目的元素 

创建**顺序容器**时，可显式指定容器大小和一个（可选的）元素初始化式。

    const list<int>::size_type list_size = 64; 
         list<string> slist(list_size, "eh?"); // 64 strings, each is eh? 

### 容器内元素的类型约束

容器元素类型必须满足以下两个约束： 
•   元素类型必须支持赋值运算。 
•   元素类型的对象必须可以复制。
**除了引用类型和输入输出（IO）标准库类型外**，所有内置或复合类型都可用做元素类型。

### 容器操作的特殊要求

其中一种需外加类型要求的容器操作是**指定容器大小并提供单个初始化式
的构造函数**。**如果容器存储类类型的对象，那么只有当其元素类型提供默认构造函数时，容器才能使用这种构造函数。**
假设类 Foo 没有默认构造函数，但提供了需要一个 int 型形参的构造函数。

    vector<Foo> empty;     // ok: no need for element default constructor 
         vector<Foo> bad(10);   // error: no default constructor for Foo 
         vector<Foo> ok(10, 1); // ok: each element initialized to 1

> bad:vector没有这种构造函数 
ok:使用了vector中指定容器大小和一个元素初始化式(int型的1)的构造函数,
> 因为Foo中提供了需要一个int型形参的构造函数,所以这条语句正确.

### 容器的容器

因为容器受容器元素类型的约束，所以可定义元素是容器类型的容器。
注意，在指定容器元素为容器类型时，**必须如下使用空格**,否则，系统会认为 >> 是单个符号，为右移操作符 
 

        vector< vector<string> > lines; // ok: space required between close 
    > 
         vector< vector<string>> lines; // error: >> treated as shift operator 

## 迭代器和迭代器范围

迭代器为所有标准库容器类型所提供的运算:

![这里写图片描述](http://img.blog.csdn.net/20150707140140482)


vector  和  deque   容器的迭代器提供额外的运算

![这里写图片描述](http://img.blog.csdn.net/20150707140203337)


> list 容器的迭代器既不支持算术运算（加法或减法），也不支持关系运算（<=, <, >=,>），它只提供前置和后置的自增、自减运算以及相等（不等）运算。

### 迭代器范围

C++ 语言使用一对迭代器标记迭代器范围（iterator range），这两个迭代器分别指向同一个容器中的两个元素或超出末端的下一位置，通常将它们命名为 first 和 last，或 beg 和 end，用于标记容器中的一段元素范围。
此类元素范围称为左闭合区间（left-inclusive interval） ，其标准表示
方式为： 

     [ first, last ) 

### 使迭代器失效的容器操作

一些容器操作会修改容器的内在状态或移动容器内的元素。这样的操作使所有指向被移动的元素的迭代器失效，也可能同时使其他迭代器失效。

## 顺序容器都提供了一组有用的类型定义及操作

每种顺序容器都提供了一组有用的类型定义以及以下操作： 
•   在容器中添加元素。 
•   在容器中删除元素。 
•   设置容器大小。 
•   （如果有的话）获取容器内的第一个和最后一个元素。

### 容器定义的类型别名

我们已经使用过三种由容器定义的类型：size_type、iterator 和 const_iterator。所有容器都提供这三种类型以及表 9.5 所列出的其他类型。 

![这里写图片描述](http://img.blog.csdn.net/20150707140224231)


**最后三种类型使程序员无须直接知道容器元素的真正类型，就能使用它**。需要使用元素类型时，只要用 value_type 即可。如果要引用该类型，则通过 reference 和 const_reference 类型实现。在程序员编写自己的泛型程序（第十六章）时，这些元素相关类型的定义非常有用。
 
### begin   和  end   成员

![这里写图片描述](http://img.blog.csdn.net/20150707140326272)


如果容器不是 const，则这些操作返回 iterator 或 reverse_iterator 类型。如果
容器是 const，则其返回类型要加上 const_ 前缀，也就是 const_iterator 和 const_reverse_iterator 类型。

### 在顺序容器中添加元素

所有顺序容器都支持 **push_back** 操作（表 9.7），提供**在容器尾部插入一个元素的功能**。

    container.push_back(text_word); 

> **容器存放的是原始元素的副本**。被复制的原始值与新容器中的元素各不相关，此后，容器内元素值发生变化时，被复制的原值不会受到影响

在顺序容器中添加元素的操作

![这里写图片描述](http://img.blog.csdn.net/20150707140245052)


  
**添加元素可能会使迭代器失效**
任何 insert 或 push 操作都可能导致迭代器失效。当编写循环将元素插入到 vector 或 deque 容器中时，程序必须确保迭代器在每次循环后都得到更新。

**不要存储 end 操作返回的迭代器**
添加或删除 deque 或 vector 容器内的元素都会导致存储的迭代器失效。为了避免存储 end迭代器，可以在每次做完插入运算后重新计算 end 迭代器值(如vec.end())

### 关系操作符

器的比较是基于容器内元素的比较。容器的比较使用了元素类型定义的同一个关系操作符：两个容器做 != 比较使用了其元素类型定义的 != 操作符。如果容器的元素类型不支持某种操作符，则该容器就不能做这种比较运算。 
下面的操作类似于 string 类型的关系运算
•   如果两个容器具有相同的长度而且所有元素都相等，那么这两个容器就相
等；否则，它们就不相等。 
•   如果两个容器的长度不相同，但较短的容器中所有元素都等于较长容器中
对应的元素，则称较短的容器小于另一个容器。

### 顺序容器的大小操作

![这里写图片描述](http://img.blog.csdn.net/20150707140349287)


容器类型提供 **resize 操作来改变容器所包含的元素个数**。如果当前的**容器长度大于新的长度值，则该容器后部的元素会被删除**；如果当前的**容器长度小于新的长度值，则系统会在该容器后部添加新元素**.

    list<int> ilist(10, 42);   // 10 ints: each has value 42 
     
         ilist.resize(15);          // adds 5 elements of value 0 to back of ilist 
     
         ilist.resize(25, -1);  // adds 10 elements of value -1 to back of ilist 
     
         ilist.resize(5);           // erases 20 elements from the back of ilist

resize 操作可能会使迭代器失效。在 vector 或 deque 容器上做 resize 操作有可能会使其所有的迭代器都失效。
对于所有的容器类型，**如果 resize 操作压缩了容器，则指向已删除的元素迭代器失效**。

### 访问元素

如果容器非空，那么容器类型的 front 和 back 成员（表 9.9）将返回容器内第一个或最后一个元素的引用(也可以用*解引用迭代器):

    // check that there are elements before dereferencing an iterator 
         // or calling front or back 
         if (!ilist.empty()) { 
             // val and val2 refer to the same element 
             list<int>::reference val = *ilist.begin(); 
             list<int>::reference val2 = ilist.front(); 
     
             // last and last2 refer to the same element 
             list<int>::reference last = *--ilist.end(); 
             list<int>::reference last2 = ilist.back(); }

**访问顺序容器内元素的操作**          
![这里写图片描述](http://img.blog.csdn.net/20150707140410600)


使用越界的下标，或调用空容器的 front 或 back 函数，都会导致程序出现严重的错误。

### 删除元素

![这里写图片描述](http://img.blog.csdn.net/20150707140449423)


### 赋值与  swap

![这里写图片描述](http://img.blog.csdn.net/20150707140741339)


如果两个容器类型相同，其元素类型也相同，就可以使用赋值操作符（=）将一个容器赋值给另一个容器。如果在不同（或相同）类型的容器内，元素类型不相同但是相互兼容，则其赋值运算必须使用 assign 函数。
**由于 assign 操作首先删除容器中原来存储的所有元素，因此，传递给 assign 函数的迭代器不能指向调用该函数的容器内的元素。** 

####使用  swap  操作以节省删除元素的成本 

swap 操作实现交换两个容器内所有元素的功能。要交换的容器的类型必须匹配：操作数必须是相同类型的容器，而且所存储的元素类型也必须相同。**该操作不会删除或插入任何元素**，而且保证在常量时间内实现交换。**由于容器内没有移动
任何元素**，**因此迭代器不会失效**。

### vector 容器的自增长

已知元素是连续存储的，当我们在容器内添加一个元素时，果容器中已经没有空间容纳新的元素，此时，由于元素必须连续存储以便索引访问，所以不能在内存中随便找个地方存储这个新元素。于是，vector 必须重新分配存储空间，用来存放原来的元素以及新添加的元素：存放在旧存储空间中的元素被复制到新存储空间里，接着插入新元素，最后撤销旧的存储空间。
**为了使 vector 容器实现快速的内存分配，其实际分配的容量要比当前所需的空间多一些。vector 容器预留了这些额外的存储区，用于存放新添加的元素。**

### capacity  和  reserve   成员

vector 类提供了两个成员函数：capacity 和 
reserve 使程序员可与 vector 容器内存分配的实现部分交互工作。
可如下预留额外的存储空间：

    ivec.reserve(50);  // sets capacity to at least 50; might be more

![这里写图片描述](http://img.blog.csdn.net/20150707140804971)



##  容器的选用


### 插入操作如何影响容器的选择

**list 容器**表示不连续的内存区域，允许向前和向后逐个遍历元素。在任何位置都可高效地 insert 或 erase 一个元素。插入或删除 list 容器中的一个元素不需要移动任何其他元素。另一方面，list容器不支持随机访问，访问某个元素要求遍历涉及的其他元素。 

对于 **vector 容器**，除了容器尾部外，其他任何位置上的插入（或删除）操作都要求移动被插入（或删除）元素右边所有的元素。例如，假设有一个拥有 50 个元素的vector 容器，我们希望删除其中的第 23 号元素，则 23 号元素后面的所有元素都必须向前移动一个位置。否则， vector容器上将会留下一个空位（hole），而 vector 容器的元素就不再是连续存放的了。

**deque 容器**拥有更加复杂的数据结构。从 deque 队列的两端插入和删除元素都非常快。在容器中间插入或删除付出的代价将更高。deque 容器同时提供了 list 和 vector 的一些性质：
•   与 vector 容器一样，在 deque 容器的中间 insert 或 erase 元素效率
比较低。 
•   不同于 vector 容器，deque 容器提供高效地在其首部实现 insert 和 
erase 的操作，就像在容器尾部的一样。 
•   与 vector 容器一样而不同于 list 容器的是， deque 容器支持对所有
元素的随机访问。 
•   在 deque 容器首部或尾部插入元素不会使任何迭代器失效，而首部或尾
部删除元素则只会使指向被删除元素的迭代器失效。在 deque 容器的任
何其他位置的插入和删除操作将使指向该容器元素的所有迭代器都失效

### 元素的访问如何影响容器的选择

vector 和 deque 容器都支持对其元素实现高效的随机访问.在 list 容器的
元素之间移动的唯一方法是顺序跟随指针。

### 选择容器的提示

下面列举了一些选择容器类型的法则： 
1.  如果程序要求随机访问元素，则应使用 vector 或 deque 容器。 
2.  如果程序必须在容器的中间位置插入或删除元素，则应采用 list 容器。 
3.  如果程序不是在容器的中间位置，而是在容器首部或尾部插入或删除元素，则应采用 deque 容器。 
4.  如果只需在读取输入时在容器的中间位置插入元素，然后需要随机访问元素，则可考虑在输入时将元素读入到一个 list 容器，接着对此容器重新排序，使其适合顺序访问，然后将排序后的 list容器复制到一个 vector 容器。

## 再谈string


**string 类型和容器类型共有的操作**

![这里写图片描述](http://img.blog.csdn.net/20150707140828713)


  
**string 类型特有的版本**
![这里写图片描述](http://img.blog.csdn.net/20150707140853943)
 


  
### 只适用于 string 类型的操作

string 类型提供了容器类型不支持其他几种操作，如表 9.16 所示： 
•   substr 函数，返回当前 string 对象的子串。 
•   append 和 replace 函数，用于修改 string 对象。 
•   一系列 find 函数，用于查找 string 对象。

![这里写图片描述](http://img.blog.csdn.net/20150707140906121)


![这里写图片描述](http://img.blog.csdn.net/20150707140923294)


  
![这里写图片描述](http://img.blog.csdn.net/20150707140938682)


### string 类型的查找操作

string 类提供了 6 种查找函数（表 9.19），每种函数以不同形式的 find 命名。这些操作全都返回 string::size_type 类型的值

![这里写图片描述](http://img.blog.csdn.net/20150707140953450)


  
### 字符串比较

除了关系操作符，string 类型还提供了一组 compare 操作（表9.21），用于实现字典顺序的比较。这些操作的结果类似于 C语言中的库函数 strcmp（第 4.3 节）。假设有语句： 

     s1.compare (args);

compare 函数返回下面列出的三种可能值之一： 
1.  正数，此时 s1 大于 args 所代表的 string 对象。 
2.  负数，此时 s1 小于 args 所代表的 string 对象。 
3.  0，此时 s1 恰好等于 args 所代表的 string 对象。 

![这里写图片描述](http://img.blog.csdn.net/20150707141016872)

 


## 容器适配器

除了顺序容器，标准库还提供了三种顺序容器适配器：queue、priority_queue 和 stack。

![这里写图片描述](http://img.blog.csdn.net/20150707141030652)


使用适配器时，必须包含相关的头文件： 
 

    #include <stack>    // stack adaptor 
     #include <queue>    // both queue and priority_queue adaptors

### 适配器的初始化

所有适配器**都定义了两个构造函数**：**默认构造函数用于创建空对象**，**而带一个容器参数的构造函数将参数容器的副本作为其基础值**。例如，假设 deq 是 deque<int> 类型的容器，则可用 deq 初始化一个新的栈，如下所示： 
 

    stack<int> stk(deq);      // copies elements from deq into stk

### 覆盖基础容器类型

默认的 stack 和 queue 都基于 deque 容器实现，而 priority_queue 则在 vector 容器上实现。在创建适配器时，**通过将一个顺序容器指定为适配器的第二个类型实参，可覆盖其关联的基础容器类型(deque)**： 

    // empty stack implemented on top of vector 
         stack< string, vector<string> > str_stk; 
     
         // str_stk2 is implemented on top of vector and holds a co py of svec 
         stack<string, vector<string> > str_stk2(svec); 

**其关联的容器必须满足一定的约束条件:**

**stack** 适配器所关联的基础容器可以是任意一种顺序容器类型。因此，stack 栈**可以建立在 vector、list 或者 deque 容器之上**。

而 **queue** 适配器要求其关联的基础容器必须提供 push_front 运算，因此**只能建立在 list 容器**上。

**priority_queue** 适配器要求提供随机访问功能，因此**可建立在 vector 或 deque容器上**，但不能建立在 list 容器上。

![这里写图片描述](http://img.blog.csdn.net/20150707141054902)

 
     


![这里写图片描述](http://img.blog.csdn.net/20150707141109599)


  
## 小结

最经常使用的容器类型是 vector，它支持对元素的快速随机访问。可高效地在 vector 容器尾部添加和删除元素，而在其他任何位置上的插入或删除运算则要付出比较昂贵的代价。deque 类与 vector 相似，但它还支持在 deque首部的快速插入和删除运算。list 类只支持元素的顺序访问，但在 list 内部任何位置插入和删除元素都非常快速。 

在容器中添加或删除元素可能会使已存在的迭代器失效。当混合使用迭代器操作和容器操作时，必须时刻留意给定的容器操作是否会使迭代器失效。