---
layout: article
title:  "c++ primer读书笔记 第十一章 泛型算法"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记
---



**标准库没有给容器添加大量的功能函数，而是选择提供一组算法，这些算法大都不依赖特定的容器类型，是“泛型”的，可作用在不同类型的容器和不同类型的元素上**。例如排序，或者查找某个特定的元素，或者查找最大或最小的元素，等等。

**大多数算法是通过遍历由两个迭代器标记的一段元素来实现其功能**。典型情况下，算法在遍历一段元素范围时，操纵其中的每一个元素。算法通过迭代器访问元素，这些迭代器标记了要遍历的元素范围。

## 概述

### 算法如何工作

**每个泛型算法的实现都独立于单独的容器**。这些算法还是大而不全的，并且不依赖于容器存储的元素类型。为了知道算法如何工作，让我们深入了解 **find 操作。该操作的任务是在一个未排序的元素集合中查找特定的元素**。
**find是基于迭代器的,所以多种容器类型都可以使用**:

    vector<int>::const_iterator result = 
                 find(vec.begin(), vec.end(), search_value);
     list<int>::const_iterator result = 
              find(lst.begin(), lst.end(), search_value);

**由于指针的行为与作用在内置数组上的迭代器一样，因此也可以使用 find 来搜索数组**： 

    int ia[6] = {27, 210, 12, 47, 109, 83}; 
     int search_value = 83;
    int *result = find(ia, ia + 6, search_value);

从概念上看，find 必须包含以下步骤：
1.  顺序检查每个元素。 
2.  如果当前元素等于要查找的值，那么返回指向该元素的迭代器。 
3.  否则，检查下一个元素，重复步骤 2，直到找到这个值，或者检查完所有的元素为止。 
4.  如果已经到达集合末尾，而且还未找到该值，则返回某个值，指明要查找的值在这个集合中不存在。


### 标准算法固有地独立于类型

find:这种算法只在一点上隐式地依赖元素类型：必须能够对元素做比较运算。
该算法的明确要求如下： 
1.  需要某种遍历集合的方式：能够从一个元素向前移到下一个元素。 
2.  必须能够知道是否到达了集合的末尾。 
3.  必须能够对容器中的每一个元素与被查找的元素进行比较。 
4.  需要一个类型指出元素在容器中的位置，或者表示找不到该元素。


### 迭代器将算法和容器绑定起来

**泛型算法用迭代器来解决第一个要求：遍历容器。所有迭代器都支持自增操作符，从一个元素定位下一个元素，并提供解引用操作符访问元素的值。**
除了 第 11.3.5 节将介绍的一个例外情况之外，迭代器还支持相等和不等操作符，用于判断两个迭代是否相等。 
每个算法都需要使用（至少）两个迭代器指出该算法操纵的元素范围。第一个迭代器指向第一个元素，而第二个迭代器则指向最后一个元素的下一位置(超出末端迭代器或哨兵)。
**用超出末端迭代器还可以很方便地处理第四个要求，只要以此迭代器为返回值，即可表示没有找到要查找的元素。**
**第三个要求——元素值的比较**，有两种解决方法。默认情况下，find 操作要元素类型定义了相等（ == ）操作符，算法使用这个操作符比较元素。如果元素类型不支持相等（ == ）操作符，或者打算用不同的测试方法来比较元素，则可使用第二个版本的 find 函数。这个版本需要一个额外的参数：实现元素比较的函数名字。

### 算法永不执行容器提供的操作

**泛型算法本身从不执行容器操作，只是单独依赖迭代器和迭代器操作实现**。算法基于迭代器及其操作实现，而并非基于容器操作。**但本质上暗示了：使用“普通”的迭代器时，算法从不修改基础容器的大小**。正如我们所看到的，算法也许会改变存储在容器中的元素的值，也许会在容器内移动元素，但是，算法从不直接添加或删除元素。

## 初窥算法
使用泛型算法必须包含 algorithm 头文件： 

    #include <algorithm>

标准库还定义了一组泛化的算术算法:

    #include <numeric>


### 只读算法

多算法只会读取其输入范围内的元素，而不会写这些元素。find 就是一个这样的算法。另一个简单的只读算法是 accumulate，该算法在 numeric 头文件中定义

    // sum the elements in vec starting the summation with the value 42 
         int sum = accumulate(vec.begin(), vec.end(), 42);

accumulate 带有三个形参。头两个形参指定要累加的元素范围。第三个形参则是累加的初值。
用于指定累加起始值的第三个实参是必要的，因为 accumulate 对将要累加的元素类型一无所知,这个事实有两层含义:首先，**调用该函数时必须传递一个起始值**，否则，accumulate 将不知道使用什么起始值。其次，**容器内的元素类型必须与第三个实参的类型匹配，或者可转换为第三个实参的类型**。在 accumulate 内部，第三个实参用作累加的起点；容器内的元素按顺序连续累加到总和之中。

考虑下面的例子，可以使用 accumulate 把 string 型的 vector 容器中的
元素连接起来：

    // concatenate elements from v and store in sum 
         string sum = accumulate(v.begin(), v.end(), string(""));

注意：程序显式地创建了一个 string 对象，用该函数调用的第三个实参。传递一个字符串字面值，将会导致编译时错误。

#### find_first_of  的使用

这个算法带有两对迭代器参数来标记两段元素范围，在第一段范围内查找与第二段范围中任意元素匹配的元素，然后返回一个迭代器，指向第一个匹配的元素。**如果找不到元素，则返回第一个范围的 end 迭代器**。

    list<string>::iterator it = roster1.begin();
    it = find_first_of(it, roster1.end(), 
                      roster2.begin(), roster2.end());


### 写容器元素的算法

#### 写入输入序列的元素

写入到输入序列的算法本质上是安全的——只会写入与指定输入范围数量相同的元素。 
一个简单算法是 fill 函数:

    fill(vec.begin(), vec.end(), 0); // reset each element to 0 
         // set subsequence of the range to 10 
         fill(vec.begin(), vec.begin() + vec.size()/2, 10);


#### 不检查写入操作的算法
fill_n 函数带有的参数包括：一个迭代器、一个计数器以及一个值。该函数从迭代器指向的元素开始，将指定数量的元素设置为给定的值。
初学者常犯的错误的是：在没有元素的空容器上调用 fill_n 函数（或者类似的写元素算法）。

    vector<int> vec; // empty vector 
         // disaster: attempts to write to 10 (nonexistent) element s in vec 

         fill_n(vec.begin(), 10, 0); 
对指定数目的元素做写入运算，或者写到目标迭代器的算法，都要检查目标的大小是否足以存储要写入的元素。

#### 引入 back_inserter

**确保算法有足够的元素存储输出数据的一种方法是使用插入迭代器**。插入迭
代器是可以给基础容器添加元素的迭代器,**用插入迭代器赋值时，则会在容器中添加一个新元素，其值等于赋值运算的右操作数的值。**

    back_insert_iterator<Container> back_inserter (Container& x);

**back_inserter是一个函数,返回一个back_insert_iterator<Container>对象.**

    back_insert_iterator<Container>& operator= (const typename Container::value_type& value)
        { container->push_back(value); return *this; }

**back_insert_iterator重新定义了赋值运算符(=):调用push_back给容器尾部插入元素(即等号的右值).**
使用 back_inserter 可以生成一个指向 fill_n 写入目标的迭代器：

        vector<int> vec;  
         fill_n (back_inserter(vec), 10, 0); // appends 10 elements to vec

现在，**fill_n 函数每写入一个值，都会通过 back_inserter 生成的插入迭代器实现**。效果相当于在 vec 上调用 push_back，在 vec 末尾添加 10 个元素，每个元素的值都是 0。
#### 写入到目标迭代器的算法
第三类算法向目标迭代器写入未知个数的元素。
copy 带有三个迭代器参数：**头两个指定输入范围，第三个则指向目标序列的一个元素。**
   
        //copy elements from ilst into ivec 
        vector<int> ivec; // empty vector 
        copy (ilst.begin(), ilst.end(), back_inserter(ivec));

#### 算法的 _copy  版本

有些算法提供所谓的“复制（copying）”版本。这些算法对输入序列的元素做出处理，但**不修改原来的元素，而是创建一个新序列存储元素的处理结果**。
replace 算法对输入序列做读写操作，将序列中特定的值替换为新的值。该算法带有四个形参：一对指定输入范围的迭代器和两个值。每一个等于第一值的元素替换成第二个值。

    //replace any element with value of 0 by 42 
         replace(ilst.begin(), ilst.end(), 0, 42);

### 对容器元素重新排序的算法

假设我们要分析一组儿童故事中所使用的单词。例如，可能想知道它们使用了多少个由六个或以上字母组成的单词。每个单词只统计一次，不考虑它出现的次数，也不考虑它是否在多个故事中出现。要求以长度的大小输出这些单词，对于同样长的单词，则以字典顺序输出。 
假定每本书的文本已经读入并保存在一个 string 类型的 vector 对象中，它的名字是words。现在，应该怎么解决包括统计单词出现次数这个问题呢？为了解此问题，要做下面几项操作： 
1.  去掉所有重复的单词。 
2.  按单词的长度排序。 
3.  统计长度等于或超过 6 个字符的单词个数。

#### 去除重复
1排序

    // sort words alphabetically so we can find the duplicates 
         sort(words.begin(), words.end());

2去重复

            vector<string>::iterator end_unique = 
            unique(words.begin(), words.end());
            words.erase(end_unique, words.end());
            //用容器的erase函数删除重复

单词按次序排列后，现在的问题是：让故事中所用到的每个单词都只保留一个副本。**unique 算法**很适合用于解决这个问题，**它带有两个指定元素范围的迭代器参数**。 **该算法删除相邻的重复元素，然后重新排列输入范围内的元素**，并且**返回一个迭代器，表示无重复的值范围的结束**。
 
![11](http://img.blog.csdn.net/20150708220745340)


         


用 unique“删除”了相邻的重复值。给“删除”加上引号是因为 **unique实际上并没有删除任何元素，而是将无重复的元素复制到序列的前端**，从而覆盖相邻的重复元素。unique **返回的迭代器指向超出无重复的元素范围末端的下一位置**。

#### 使用容器操作删除元素

算法不直接修改容器的大小。如果需要添加或删除元素，则必须使用容器操作,在本例中调用 erase 实现该功能。

#### 定义需要的实用函数

配套的实用函数，称为谓词。谓词是做某些检测的函数，返回用于条件判断的类型，指出条件是否成立。 
为了实现排序，必须定义一个谓词函数来实现两个 string对象的比较，并返回一个 bool 值，指出第一个字符串是否比第二个短：

    // comparison function to be used to sort by word length 
         bool isShorter(const string &s1, const string &s2) 
         { 
             return s1.size() < s2.size(); 
         }

另一个所需的谓词函数将判断给出的 string 对象的长度是否不小于 6： 
   

      // determine whether a length of a given word is 6 or more 
         bool GT6(const string &s) 
         { 
              return s.size() >= 6; 
         }

#### 排序算法

标准库定义了四种不同的排序算法，上面只使用了最简单的 sort 算法，使 words 按字典次序排列。除了 sort 之外，标准库还定义了 stable_sort 算法，**stable_sort 保留相等元素的原始相对位置**。
sort 和 stable_sort 都是重载函数。其中一个版本使用元素类型提供的小
于（<）操作符实现比较。
**第二个重载版本带有第三个形参：比较元素所使用的谓词函数的名字**。

    // sort words by size, but maintain alphabetic order for words of 
    the same size 
         stable_sort(words.begin(), words.end(), isShorter);

#### 统计长度不小于 6 的单词

    vector<string>::size_type wc = 
                      count_if(words.begin(), words.end(), GT6); 

执行 count_if 时，首先读取它的头两个实参所标记的范围内的元素。**每读出一个元素，就将它传递给第三个实参表示的谓词函数**。

## 再谈迭代器
C++ 语言还提供了另外三种迭代器：
•   **插入迭代器**：这类迭代器与容器绑定在一起，实现在容器中插入元素的功能。 
•   **iostream 迭代器**：这类迭代器可与输入或输出流绑定在一起，用于迭代遍历所关联的 IO 流。 
•   **反向迭代器**：这类迭代器实现向后遍历，而不是向前遍历。所有容器类型都定义了自己的 reverse_iterator 类型，由 rbegin 和 rend 成员函数返回。

### 插入迭代器
C++ 语言提供了三种插入器，其差别在于插入元素的位置不同。
 •   back_inserter，创建使用 push_back 实现插入的迭代器。 
•   front_inserter，使用 push_front 实现插入。 
•   inserter，使用 insert 实现插入操作。除了所关联的容器外，inserter 
还带有第二实参：指向插入起始位置的迭代器

#### front_inserter 需要使用 push_front
front_inserter 的操作类似于 back_inserter：该函数将**创建一个迭代器，调用它所关联的基础容器的push_front 成员函数代替赋值操作。**

#### inserter 将产生在**指定位置实现插入**的迭代器 
inserter 适配器提供更普通的插入形式。这种适配器带有**两个实参**：**所关联的容器**和**指示起始插入位置的迭代器**。 

     
     list<int>::iterator it = 
                find (ilst.begin(), ilst.end(), 42); 
     replace_copy (ivec.begin(), ivec.end(), 
                inserter (ilst, it), 100, 0);

#### iostream 迭代器
虽然 iostream 类型不是容器，但标准库同样提供了在 iostream 对象上使用的迭代器：istream_iterator 用于读取输入流，而 ostream_iterator 则用于写输出流（表11.1）。这些迭代器将它们所对应的流视为特定类型的元素序列。使用流迭代器时，可以用泛型算法从流对象中读数据（或将数据写到流对象中）。

![这里写图片描述](http://img.blog.csdn.net/20150708220841149)


流迭代器只定义了最基本的迭代器操作：自增、解引用和赋值。此外，可比较两个 istream 迭代器是否相等（或不等）。而 ostream 迭代器则不提供比较运算（表 11.2）。

![这里写图片描述](http://img.blog.csdn.net/20150708220901959)


流迭代器都是类模板：任何已定义输入操作符（>> 操作符）的类型都可以定义istream_iterator。类似地，任何已定义输出操作符（<< 操作符）的类型也可定义 ostream_iterator。 
在创建流迭代器时，必须指定迭代器所读写的对象类型：

        istream_iterator<int> cin_it(cin);    // reads ints1 from cin 
         istream_iterator<int> end_of_stream;  // end iterator value 
         // writes Sales_items from the ofstream named outfile 
         // each element is followed by a space 
         ofstream outfile; 
         ostream_iterator<Sales_item> output(outfile, " ");

**ostream_iterator 对象必须与特定的流绑定在一起**。在创建 istream_iterator 时，可直接将它绑定到一个流上。**另一种方法是在创建时不提供实参，则该迭代器指向超出末端位置**。ostream_iterator 不提供超出末端迭代器。
在创建 ostream_iterator对象时，可提供**第二个（可选的）实参，指定将元素写入输出流时使用的分隔符。分隔符必须是 C 风格字符串**。因为它是 C 风格字符串，所以必须以空字符结束；否则，其行为将是未定义的。 

#### istream_iterator 对象上的操作

构造与流绑定在一起的 istream_iterator 对象时将对迭代器定位，以便第一次对该迭代器进行解引用时即可从流中读取第一个值。
考虑下面例子，可使用 istream_iterator 对象将标准输入读到 vector 对象中。 

         istream_iterator<int> in_iter(cin); // read ints from cin 
         istream_iterator<int> eof; // istream "end" iterator 
         while (in_iter != eof) 
                 vec.push_back(*in_iter++); 

其中 **eof 迭代器定义为空的 istream_iterator 对象，用作结束迭代器**。

更有趣的是可以这样重写程序： 
  

       istream_iterator<int> in_iter(cin); // read ints from cin 
         istream_iterator<int> eof;      // istream "end" iterator 
         vector<int> vec(in_iter, eof);  // construct vec from an iterator range
 
用一对标记元素范围的迭代器构造 vec 对象。这些迭代器是 istream_iterator 对象，这就意味着这段范围的元素是通过读取所关联的流来获得的。**这个构造函数的效果是读 cin，直到到达文件结束或输入的不是 int 型数值为止**。

#### ostream_iterator 对象的使用
可使用 ostream_iterator 对象将一个值序列写入流中:

   
         ostream_iterator<string> out_iter(cout, "\n"); 
         istream_iterator<string> in_iter(cin), eof; 
         while (in_iter != eof) 
            *out_iter++ = *in_iter++; 

**这个程序读 cin，并将每个读入的值依次写到 cout 中不同的行中(因为ostream_iterator对象的构造函数使用了分隔符'\n')。**

#### 在类类型上使用 istream_iterator

**提供了输入操作符（>>）的任何类型都可以创建 istream_iterator 对象**。

    istream_iterator<Sales_item> item_iter(cin); 

#### 流迭代器的限制
•   不可能从 ostream_iterator 对象读入，也不可能写到 istream_iterator 对象中。 
•   **一旦给 ostream_iterator对象赋了一个值，写入就提交了**。赋值后，没有办法再改变这个值。此外，ostream_iterator 对象中每个不同的值都只能正好输出一次。 
•   **ostream_iterator 没有 -> 操作符**。

#### 与算法一起使用流迭代器 

算法是基于迭代器操作实现的的。

        istream_iterator<int> cin_it(cin);    // reads ints from cin 
         istream_iterator<int> end_of_stream;  // end iterator value 
         // initialize vec from the standard input: 
         vector<int> vec(cin_it, end_of_stream); 
         sort(vec.begin(), vec.end()); 
         
         ostream_iterator<int> output(cout, " "); 
         unique_copy(vec.begin(), vec.end(), output);

如果程序的输入是： 

     23 109 45 89 6 34 12 90 34 23 56 23 8 89 23 

输出则是： 

     6 8 12 23 34 45 56 89 90 109 

### 反向迭代器

反向迭代器是一种反向遍历容器的迭代器。

![这里写图片描述](http://img.blog.csdn.net/20150708220929154)


#### 反向迭代器需要使用自减操作符

标准容器上的迭代器既支持自增运算，也支持自减运算。但是，流迭代器却不然，由于不能反向遍历流，因此**流迭代器不能创建反向迭代器**。

#### 反向迭代器与其他迭代器之间的关系

**输出 line 中的第一个单词：**

    string::iterator comma = find(line.begin(), line.end(), ','); 
         cout << string(line.begin(), comma) << endl; 

如果在 line 中有一个逗号，则 comma 指向这个逗号；否则，comma 的值为 line.end()。

**输出列表中最后一个单词，可使用反向迭代器：**
 

    string::reverse_iterator rcomma = 
                            find(line.rbegin(), line.rend(), ',');

**直接尝试输出：**

    

    cout << string(line.rbegin(), rcomma) << endl;

会产生假的输出。例如，如果输入是： 

     FIRST,MIDDLE,LAST

则将输出 TSAL
使用反向迭代器时，以逆序从后向前处理 string 对象。为了得到正确的输出，必须**将反向迭代器** line.rbegin() 和 rcomma **转换为从前向后移动的普通迭代器**。**只需调用所有反向迭代器类型都提供的成员函数 base 转换 rcomma 即可**： 

![这里写图片描述](http://img.blog.csdn.net/20150708220949289)
 


改为：

    // ok: get a forward iterator and read to end of line 
         cout << string(rcomma.base(), line.end()) << endl;


#### const 迭代器
在第 11.1 节使用 find 的程序中，我们将 result 定义为 **const_iterator 类型**。这样做是因为我们**不希望使用这个迭代器来修改容器中的元素**。
另一方面，虽然第 11.2.1 节的程序也不打算改变容器内的任何元素，但是**它却使用了普通的非 const 迭代器来保存 find_first_of 的返回值**。这两种处理存在细微的差别，值得解释一下。
算法要求用于指定范围的两个迭代器必须具有完全一样的类型。roster1.end() 返回的迭代器依赖于 roster1 的类型。如果该容器是 const 对象，则返回的迭代器是 const_iterator 类型；否则，就是普通的 iterator 类型。
原因是，**在第二个例子中，程序将迭代器用作 find_first_of 的实参**： 
 

    find_first_of(it, roster1.end(), 
                   roster2.begin(), roster2.end()) ;

该函数调用的输入范围由 it 和调用 roster1.end() 返回的迭代器指定。**算法要求用于指定范围的两个迭代器必须具有完全一样的类型**。roster1.end() 返回的迭代器依赖于 roster1 的类型。如果该容器是 const 对象，则返回的迭代器是 const_iterator 类型；否则，就是普通的 iterator 类型。在这个程序中，**roster1 不是 const 对象，因而 end 返回的只是一个普通的迭代器。**

#### 五种迭代器

算法要求的迭代器操作分为五个类别:

![这里写图片描述](http://img.blog.csdn.net/20150708221021095)


1. 输入迭代器 可用于读取容器中的元素
o  相等和不等操作符（==，!=），比较两个迭代器。 
o  前置和后置的自增运算（++），使迭代器向前递进指向下一个元素。 
o  用于**读取元素的解引用操作符**（*），**此操作符只能出现在赋值运算的右操作数上**。 
o  箭头操作符（->），这是 (*it).member 的同义语，也就是说，对迭代器进行解引用来获取其所关联的对象的成员
2.  输出迭代器
输出迭代器可用于向容器写入元素
o  前置和后置的自增运算（++），使迭代器向前递进指向下一个元素。
o  **解引用操作符（*），只能出现在赋值运算的左操作数上**。给解引用的输出迭代器赋值，将对该迭代器所指向的元素做写入操作。
3.  前向迭代器 用于读写指定的容器。这类迭代器只会以一个方向遍历序列。
4.  双向迭代器  从两个方向读写容器。需要使用双向迭代器的泛型算法包括 reverse。
5.  随机访问迭代器  提供在常量时间内访问容器任意位置的功能。
支持下面的操作：
o  关系操作符 <、<=、> 和 >=，比较两个迭代器的相对位置。 
o  迭代器与整型数值 n 之间的加法和减法操作符 +、+=、- 和 -=，结果是迭代器在容器中向前（或退回）n 个元素。 
o  两个迭代器之间的减法操作符（--），得到两个迭代器间的距离。 
o  下标操作符 iter[n]，这是 *(iter + n) 的同义词。
**需要随机访问迭代器的泛型算法包括 sort 算法**。vector、deque 和 string 迭代器是随机访问迭代器，用作访问内置数组元素的指针也是随机访问迭代器。

**map、set 和 list 类型提供双向迭代器**，而 **string、vector 和 deque 容器上定义的迭代器都是随机访问迭代器都是随机访问迭代器**，**用作访问内置数组元素的指针也是随机访问迭代器**。

## 泛型算法的结构

### 算法的命名规范

第一种模式包括**测试输入范围内元素的算法**，第二种模式则**应用于对输入范围内元素重新排序的算法**。

#### 区别带有一个值或一个谓词函数参数的算法版本 

很多算法通过检查其输入范围内的元素实现其功能。这些算法通常要用到标准关系操作符：== 或 <。其中的大部分算法会提供第二个版本的函数，允许程序员提供比较或测试函数取代操作符的使用。 

    sort (beg, end);         // use < operator to sort the elements 
         sort (beg, end, comp);   // use function named comp to sort the elements
 
#### 区别是否实现复制的算法版本

无论算法是否检查它的元素值，都可能重新排列输入范围内的元素。在默认情况下，这些算法将重新排列的元素写回其输入范围。标准库也为这些算法提供另外命名的版本，**将元素写到指定的输出目标**。**此版本的算法在名字中添加了 _copy 后缀**： 

    reverse(beg, end); 
    reverse_copy(beg, end, dest);

## 容器特有的算法

list 容器上的迭代器是双向的，而不是随机访问类型，因此，在此容器上不能使用需要随机访问迭代器的算法。

![这里写图片描述](http://img.blog.csdn.net/20150708221114907)

list 容器特有的算法与其泛型算法版本之间有两个至关重要的差别。

其中一个差别是 **remove 和 unique 的 list 版本****修改了其关联的基础容器**：**真正删除了指定的元素**。

另一个差别是 list 容器提供的 **merge 和 splice 运算会破坏它们的实参**。使用 merge的泛型算法版本时，合并的序列将写入目标迭代器指向的对象，而它的两个输入序列保持不变。但是，使用 list 容器的 merge 成员函数时，则会破坏它的实参 list 对象——**当实参对象的元素合并到调用merge 函数的 list 对象时，实参对象的元素被移出并删除**。 

## 小结

C++ 标准化过程：创建和扩展了标准库。容器和算法库是标准库的基础。这些算法具有相同的结构。

算法与类型无关：它们通常在一个元素序列上操作，这些元素可以存储在标准库容器类型、内置数组甚至是生成的序列（例如读写流所生成的序列）上

代器可通过其所支持的操作来分类。标准库定义了五种迭代器类别：输入、输出、前向、双向和随机访问迭代器。

正如迭代器根据操作来分类一样，算法的迭代器形参也通过其所要求的迭代器操作来分类。只需要读取其序列的算法通常只要求输入迭代器的操作。而写目标迭代器的算法则通常只要求输出迭代器的操作，依此类推。
