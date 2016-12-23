---
layout: article
title:  "c++ primer读书笔记--第十章 关联容器"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

关联容器和顺序容器的本质差别在于：**关联容器通过键（key）存储和读取元素**，而**顺序容器则通过元素在容器中的位置顺序存储和访问元素**。
关联容器（Associative containers）支持通过键来高效地查找和读取元素。**两个基本的关联容器类型是 map 和 set**。**map 的元素以键－值（key-value）对**的形式组织：键用作元素在 map中的索引，而值则表示所存储和读取的数据。**set 仅包含一个键**，并有效地支持关于某个键是否存在的查询。
**map和set的例子**：在做某种文本处理时，可使用 set 保存要忽略的单词。而字典则是 map 的一种很好的应用：单词本身是键，而它的解释说明则是值。 

![这里写图片描述](http://img.blog.csdn.net/20150707190817988)


## 引言：pair 类型
开始介绍关联容器之前，必须先了解一种与之相关的简单的标准库类型——pair
**pairs  类型提供的操作**

![这里写图片描述](http://img.blog.csdn.net/20150707190841206)

pair 类型的使用相当繁琐，可考虑利用 typedef 简化其声明：

    typedef pair<string, string> Author; 
         Author proust("Marcel", "Proust"); 
         Author joyce("James", "Joyce"); 

### pairs 对象的操作

其成员都是仅有的，分别命名为 first 和second。只需使用普通的点操作符——成员访问标志即可访问其成员：

    string firstBook; 
         // access and test the data members of the pair 
         if (author.first == "James" && author.second == "Joyce") 
             firstBook = "Stephen Hero";

### 生成新的 pair 对象
标准库还定义了一个 make_pair 函数，由传递给它的两个实参生成一个新的 pair 对象。

    pair<string, string> next_auth; 
         string first, last; 
         while (cin >> first >> last) { 
             // generate a pair from first and last 
             next_auth = make_pair(first, last); 
             // process next_auth... 
         }
由于 **pair 的数据成员是公有的，因而可如下直接地读取输入**：

    while (cin >> next_auth.first >> next_auth.second) { 
             // process next_auth... 
         }

## 关联容器

关联容器共享大部分——但并非全部——的顺序容器操作。关联容器不提供front、 push_front、 pop_front、back、push_back 以及 pop_back 操作。
关联容器构造函数:

        C<T> c;          // creates an empty container 
         // c2 must be same type as c1 
         C<T> c1(c2);    // copies elements from c2 into c1 
         // b and e are iterators denoting a sequence 
         C<T> c(b, e);   // copies elements from the sequence into c

关联容器不能通过容器大小来定义，因为这样的话就无法知道键所对应的值是什么。

### 根据键排列元素

容器元素根据键的次序排列:在迭代遍历关联容器时，我们可确保按键的顺序的访问元素，而与元素在容器中的存放位置完全无关。

## map  类型
要使用 map  对象，则必须包含 map 头文件。**在定义 map 对象时，必须分别指明键和值的类型（value type）**

    map<string, int> word_count; // empty map from string to int

### map 的构造函数

![这里写图片描述](http://img.blog.csdn.net/20150707190852971)

### 键类型的约束
在使用**关联容器**时，它的键不但有一个类型，而且**还有一个相关的比较函数**。默认情况下(允许自定义)，**标准库使用键类型定义的 < 操作符来实现键（key type）的比较**。
对于键类型，**唯一的约束就是必须支持 < 操作符**，至于是否支持其他的关系或相等运算，则不作要求。 

### map 定义的类型

![这里写图片描述](http://img.blog.csdn.net/20150707190910267)

需谨记 **value_type 是 pair 类型，它的值成员可以修改，但键成员不能修改。**

## map迭代器进行解引用将产生 pair 类型的对象
对**迭代器进行解引用时**，将获得一个引用，**指向容器中一个 value_type 类型的值**。对于 map 容器，其 **value_type 是 pair 类型**： 

    map<string, int>::iterator map_it = word_count.begin(); 
    // *map_it is a reference to a pair<const string, int>object 
         cout << map_it->first;                  // prints the key  for this element 
         cout << " " << map_it->second;          // prints the valu e of the element 
         map_it->first = "new key";              // error: key is const 
         ++map_it->second;     // ok: we can change value through a n iterator

**对迭代器进行解引用将获得一个 pair 对象，它的 first成员存放键，为 const，而 second 成员则存放值。**

### 给 map 添加元素
该项工作可使用 insert 成员实现；或者，先用下标操作符获取元素(即键值)，然后给获取的元素赋值。

#### 使用下标访问 map 对象

    map <string, int> word_count; // empty map 
     
         // insert default initialzed element with key Anna; then assign 1 
    to its value 
         word_count["Anna"] = 1;

将发生以下事情： 
1.  在 word_count 中查找键为 Anna 的元素，没有找到。 
2.  将一个新的键－值对插入到 word_count 中。它的键是 const string 类型
的对象，保存 Anna。而它的值则采用值初始化，这就意味着在本例值为 0。
3.  将这个新的键－值对插入到 word_count 中。  
4.  读取新插入的元素，并将它的值赋为 1。

使用下标访问 map 与使用下标访问数组或 vector 的行为截然不同：用下标访问不存在的元素将导致在 map容器中添加一个新元素，它的键即为该下标值。如果该键已在容器中，则 map 的下标运算与 vector 的下标运算行为相同：返回该键所关联的值。只有在**所查找的键不存在时，map 容器才为该键创建一个新的元素，并将它插入到此 map 对象中。**
显然，**map 迭代器返回 value_type 类型的值**——包含 const key_type 和 mapped_type 类型成员的 pair 对象；**下标操作符则返回一个mapped_type 类型的值**。

### 下标行为的编程意义

对于 map 容器，如果下标所表示的键在容器中不存在，则添加新元素，这一特性可使程序惊人地简练:

    // count number of times each word occurs in the input 
     map<string, int> word_count; // empty map from string to int 
     string word; 
     while (cin >> word) 
       ++word_count[word];

这段程序创建一个 map 对象，用来记录每个单词出现的次数。

### map::insert 的使用

![这里写图片描述](http://img.blog.csdn.net/20150707190932384)


### 以 insert 代替下标运算
而插入元素的
另一个方法是：直接使用 insert 成员，其语法更紧凑：

    // if Anna not already in word_count, inserts new element  with value 1 
         word_count.insert(map<string, int>::value_type("Anna", 1));

这个 insert 函数版本的实参： 
     map<string, int>::value_type(anna, 1) 

 
是一个新创建的 pair 对象，将直接插入到 map 容器中。**谨记value_type 是 pair<const K, V> 类型的同义词，K 为键类型，而 V是键所关联的值的类型。**insert 的实参创建了一个适当的 pair 类型新对象，该对象将插入到 map 容器。
**简化insert 的实参**
传递给 insert 的实参相当笨拙。可用两种方法简化：**使用 make_pair:** 
 

    word_count.insert(make_pair("Anna", 1));

 
 
**或使用 typedef** 
 

    typedef map<string,int>::value_type valType; 
     word_count.insert(valType("Anna", 1)); 

下面是使用 insert 重写的单词统计程序：

    map<string, int> word_count; // empty map from string to int 
         string word; 
         while (cin >> word) { 
             // inserts element with key equal to word and value 1; 
             // if word already in word_count, insert does nothing 
             pair<map<string, int>::iterator, bool> ret = 
                       word_count.insert(make_pair(word, 1)); 
             if (!ret.second)          // word already in word_count 
                 ++ret.first->second;  // increment counter 
         }

ret 的定义和自增运算可能比较难解释         

    pair<map<string, int>::iterator, bool> ret = 
                 word_count.insert(make_pair(word, 1));

首先，应该很容易看出我们定义的是一个 pair 对象，它的 second成员为 bool 类型。而它的 first 成员则比较难理解，这是 map<string, int> 容器所定义的迭代器类型。
根据操作符的优先级次序,可如下从添加圆括号开始理解自增操作：


    ++((ret.first)->second); // equivalent expression 

下面**对这个表达式一步步地展开解释**： 
•   **ret 存储 insert 函数返回的 pair 对象。该 pair 的 first成员是一个 map 迭代器，指向插入的键。** 
•   ret.first 从 insert 返回的 pair 对象中获取 map 迭代器。 
•   ret.first->second 对该迭代器进行解引用，获得一个value_type 类型的对象。这个对象同样是 pair 类型的，它的 second成员即为我们所添加的元素的值部分。 
•   ++ret.first->second 实现该值的自增运算。

### 查找并读取 map 中的元素
下标操作符给出了读取一个值的最简单方法： 
 

    map<string,int> word_count; 
     int occurs = word_count["foobar"];

如果该键不在 map 容器中，那么下标操作会插入一个具有该键的新元素,其关联的值为 0。
map 容器提供了两个操作：count 和 find，用于检查某个键是否存在而不会插入该键。

![这里写图片描述](http://img.blog.csdn.net/20150707190949426)

 


对于 map 对象，count 成员的返回值只能是 0 或 1。map 容器只允许一个
键对应一个实例，所以 count 可有效地表明一个键是否存在。

    int occurs = 0; 
         if (word_count.count("foobar")) 
             occurs = word_count["foobar"]; 

执行 count 后再使用下标操作符，实际上是对元素作了两次查找。如果希望当元素存在时就使用它，则应该用 find 操作。

**find 操作返回指向元素的迭代器，如果元素不存在，则返回 end 迭代器：** 
 

    int occurs = 0; 
     map<string,int>::iterator it = word_count.find("foobar"); 
     if (it != word_count.end()) 
         occurs = it->second;
         

### 从 map 对象中删除元素 
从 map 容器中删除元素的 erase 操作有三种变化形式（表 10.7）。与顺序容器一样，可向 erase传递一个或一对迭代器，来删除单个元素或一段范围内的元素。其删除功能类似于顺序容器，但有一点不同：**map 容器的 erase 操作返回 void，而顺序容器的 erase 操作则返回一个迭代器**，指向被删除元素后面的元素。 
除此之外，map 类型还提供了**一种额外的 erase 操作，其参数是 key_type类型的值，如果拥有该键的元素存在，则删除该元素。erase 函数返回被删除元素的个数**。对于 map 容器，该值必然是 0 或 1。如果返回 0，则表示欲删除的元素在 map 不存在。

![这里写图片描述](http://img.blog.csdn.net/20150707191001469)


### map 对象的迭代遍历

与其他容器一样，map 同样提供 begin 和 end 运算，以生成用于遍历整个容器的迭代器。

            map<string, int>::const_iterator map_it = word_count.begin();
            while (map_it != word_count.end()) { 
             cout << map_it->first << " occurs " 
                  << map_it->second << " times" << endl; 
             ++map_it;  
     } 

## set 类型
set 容器只是单纯的键的集合。
除了两种例外情况，set 容器支持大部分的 map 操作，包括下面几种： 
•   第 10.2 节列出的所有通用的容器操作。 
•   表 10.3 描述的构造函数。 
•   表 10.5 描述的 insert 操作。 
•   表 10.6 描述的 count 和 find 操作。 
•   表 10.7 描述的 erase 操作。
set 不支持下标操作符，而且没有定义 mapped_type 类型。在 set 容器中，value_type 不是 pair 类型，而是与 key_type 相同的类型。它们指的都是 set 中存储的元素类型。

### set 容器的定义和使用

    vector<int> ivec;
    set<int> iset(ivec.begin(), ivec.end());

### 在 set  中添加元素
可使用 insert 操作在 set 中添加元素： 
 

    set<string> set1;         // empty set 
     set1.insert("the");       // set1 now has one element

另一种用法是，调用 insert 函数时，提供一对迭代器实参，插入其标记范
围内所有的元素。  

    set2.insert(ivec.begin(), ivec.end());     // set2 has 10 elements

### 从 set  中获取元素

set 容器不提供下标操作符。为了通过键从 set 中获取元素，可使用 find 运算。 如果只需简单地判断某个元素是否存在，同样可以使用 count 运算，返回 set 中该键对应的元素个数(0或1)。

    iset.find(1)     // returns iterator that refers to the el ement with key ==1 

正如不能修改 map 中元素的键部分一样，set 中的键也为 const。在获得
指向 set 中某元素的迭代器后，只能对其做读操作，而不能做写操作：

    set<int>::iterator set_it = iset.find(1); 
     *set_it = 11;               // error: keys in a set are read-only
     cout << *set_it << endl;    // ok: can read the key

## multimap 和 multiset 类型

由于键不要求是唯一的，因此每次调用 insert 总会添加一个元素(**不管key是否相同都添加**)。
带有一个键参数的 erase 版本将删除拥有该键的所有元素，并返回删除元素的个数。而带有一个或一对迭代器参数的版本只删除指定的元素，并返回 void 类型：

    // erase all elements with this key; returns number of elements removed 
        multimap<string, string>::size_type cnt = 
                                   authors.erase(search_item);

### 在 multimap 和 multiset 中查找元素
关联容器 map 和 set 的元素是按顺序存储的,如果某个键对应多个实例，则这些实例在容器中将相邻存放。

#### 使用 find 和 count  操作

    sz_type entries = authors.count(search_item);
    multimap<string,string>::iterator iter = 
                                  authors.find(search_item);
            for (sz_type cnt = 0; cnt != entries; ++cnt, ++iter) 
            cout << iter->second << endl; // print each title

#### 与众不同的面向迭代器的解决方案

可用于普通的 map 和 set 容器，但更常用于 multimap 和 multiset。**所有这些操作都需要传递一个键，并返回一个迭代器**。

![这里写图片描述](http://img.blog.csdn.net/20150707191014510)

若该键没有关联的元素，则 lower_bound 和 upper_bound 返回相同的迭代器：都指向同一个元素或同时指向 multimap 的超出末端位置。 

#### enual_range  函数 

equal_range 函数**返回存储一对迭代器的 pair 对象**。如果该值存在，则 pair 对象中的**第一个迭代器指向该键关联的第一个实例**，**第二个迭代器指向该键关联的最后一个实例的下一位置**。

## 小结

关联容器的元素按键排序和访问。关联容器支持通过键高效地查找和读取元素。键的使用，使关联容器区别于顺序容器，顺序容器的元素是根据位置访问的。 

map 和 multimap 类型存储的元素是键－值对。它们使用在 utility 头文件中定义的标准库 pair 类，来表示这些键－值对元素。对 map 或multimap 迭代器进行解引用将获得 pair 类型的值。pair 对象的 first成员是一个 const 键，而 second 成员则是该键所关联的值。set 和multiset 类型则专门用于存储键。在 map 和 set类型中，一个键只能关联一个元素。而 multimap 和 multiset类型则允许多个元素拥有相同的键。 

关联容器共享了顺序容器的许多操作。除此之外，关联容器还定义一些新操作，并对某些顺序容器同样提供的操作重新定义了其含义或返回类型，这些操作的差别体现了关联容器中键的使用。 

关联容器的元素可用迭代器访问。标准库保证迭代器按照键的次序访问元素。begin 操作将获得拥有最小键的元素，对此迭代器作自增运算则可以按非降序依次访问各个元素。