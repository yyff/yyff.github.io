---
layout: article
title:  "c++ primer读书笔记--第十二章 类"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

在 C++ 中，用类来定义自己的**抽象数据类型**（abstract data types）。通过定义类型来对应所要解决的问题中的各种概念，可以使我们更容易编写、调试和修改程序。

## 类的定义和声明

再来看看Sales_item 类：


    class Sales_item { 
    public: 
        // operations on Sales_item objects 
        double avg_price() const; 
        bool same_isbn(const Sales_item &rhs) const 
            { return isbn == rhs.isbn; } 
        // default constructor needed to initialize members of built-in type 
        Sales_item(): units_sold(0), revenue(0.0) { } 
    private: 
        std::string isbn; 
        unsigned units_sold; 
        double revenue; 
    }; 
     
    double Sales_item::avg_price() const 
    { 
        if (units_sold)
            return revenue/units_sold; 
        else 
            return 0; 
    }


### 类定义  

最简单地说，**类就是定义了一个新的类型和一个新作用域**。

#### 类成员

每个类可以没有成员，也可以定义多个成员，**成员可以是数据、函数或类型别名**。 
一个类可以包含若干公有的、私有的和受保护的部分.



#### 构造函数

**构造函数是一个特殊的、与类同名的成员函数，用于给每个数据成员设置适当的初始值**。
构造函数一般就使用一个构造函数初始化列表（第 7.7.3节），来初始化
对象的数据成员：


    // default constructor needed to initialize members of built-in type 
    Sales_item(): units_sold(0), revenue(0.0) { }


#### 成员函数

**在类内部**，**声明成员函数是必需的**，而**定义成员函数则是可选的**。
**在类外部定义的成员函数必须指明它们是在类的作用域中**。Sales_item::avg_price 的定义使用作用域操作符（第 1.2.2 节）来指明这是 Sales_item 类中 avg_price 函数的定义。 

成员函数有一个附加的隐含实参，将函数绑定到调用函数的对象:

    trans.avg_price();

就是在调用名 trans 的对象的 avg_price 函数。**在 avg_price函数内部对 Sales_item 类成员引用就是对 trans 成员的引用**。

可以将成员函数声明为常量：

    double avg_price() const;

**const 成员不能改变其所操作的对象的数据成员**。trans.avg_price()这条语句中就不能修改trans的成员.

### 数据抽象和封装

类背后蕴涵的基本思想是数据抽象和封装。
**数据抽象**是一种依赖于**接口和实现分离**的编程（和设计）技术。

**封装**是一项**低层次的元素组合起来的形成新的、高层次实体**的技术。例如:
函数是封装的一种形式：可以调用一个函数但不能访问它所执行的语句。
类也是一个封装的实体：它代表若干成员的聚焦，大多数（良好设计的）类类型隐藏了实现该类型的成员。

数组在概念上类似于vector，但既不是抽象的，也不是封装的。可以通过访问存放数组的内存来直接操纵数组。

#### 访问标号实施抽象和封装

在 C++ 中，**使用访问标号来定义类的抽象接口和实施封装。**
一个类可以没有访问标号，也可以包含多个访问标号： 
**private**：只能由1.该类中的函数、2.其友元函数访问。不能被任何其他访问，该类的对象也不能访问。

**protected**：可以被1.该类中的函数、2.其友元函数访问、3.子类的函数。但不能被该类的对象访问。

**public**：可以被1.该类中的函数、2.子类的函数、3.其友元函数访问，**4.该类的对象访问**。
 
注：**友元函数**包括3种：设为友元的普通的非成员函数；设为友元的其他类的成员函数；设为友元类中的所有成员函数。

如果**类是用 struct**关键字定义的，则在**默认的成员是公有的(public)**；
如果**类是用 class** 关键字是定义的，则**这些成员是私有的(private)**。

#### 数据抽象和封装的好处

•   避免类内部出现无意的、可能破坏对象状态的用户级错误。 
•   随时间推移可以根据需求改变或缺陷（bug）报告来完美类实现，而无须改变用户级代码。

### 关于类定义的更多内容

#### 使用类型别名来简化类

除了定义数据和函数成员之外，**类还可以定义自己的局部类型名字**。

    typedef std::string::size_type index;

#### 成员函数可被重载

成员函数只能重载本类的其他成员函数。类的成员函数与普通的非成员函数以及在其他类中声明的函数不相关，也不能重载它们。

#### 定义重载成员函数


   


 	class Screen { 
    public: 
        typedef std::string::size_type index; 
        char get() const { return contents[cursor]; } 
        char get(index ht, index wd) const; 
    private: 
        std::string contents; 
        index cursor; 
        index height, width; 
    };


一个版本返回由当前光标指示的字符，另一个返回指定行列处的字符. 

#### 显式指定 inline 成员函数
在类内部定义的成员函数，例如不接受实参的 get 成员，将自动作为 **inline** 处理。也就是说，**当它们被调用时，编译器将试图在同一行内扩展该函数**（第 7.6 节）。也可以显式地将成员函数声明为 inlin,且**声明和定义处指定 inline 都是合法的**。

    index get_cursor() const;

或

    inline Screen::index Screen::get_cursor() const 
     { 
         return cursor; 
     } 


### 类声明与类定义
可以声明一个类而不定义它： 
 

    class Screen; // declaration of the Screen class

在声明之后、定义之前，类 Screen 是一个不完全类型（incompete type），即已知 Screen 是一个类型，但不知道包含哪些成员。**不完全类型只能用于定义指向该型的指针及引用**。可以定义：

    Screen *sc;
    Screen &sc;

但不能定义对象:

    Screen sc;

**在创建类的对象之前，必须完整地定义该类**。这样，编译器就会给类的对象预定相应的存储空间。

#### 为类的成员使用类声明
因为只有当类定义体完成后才能定义类，因此类不能具有自身类型的数据成员。然而，**只要类名一出现就可以认为该类已声明。因此，类的数据成员可以是指向自身类型的指针或引用：**


    class LinkScreen { 
             Screen window; 
             LinkScreen *next; 
             LinkScreen *prev; 
         };


### 类对象
定义一个类时，也就是定义了一个类型。一旦定义了类，就可以定义该类型的对象。**定义对象时，将为其分配存储空间**，**但（一般而言）定义类型时不进行存储分配**：

#### 定义类类型的对象
•   将类的名字直接用作类型名。 
•   指定关键字 class 或 struct，后面跟着类的名字：

    Sales_item item1;       // default initialized object of type Sales_item 
         class Sales_item item1; // equivalent definition of item1

 #### 为什么类的定义以分号结束
 类的定义分号结束。分号是必需的，**因为在类定义之后可以接一个对象定义列表**。

     class Sales_item { /* ... */ }; 
    class Sales_item { /* ... */ } accum, trans;

## 隐含的  this 指针
成员函数具有一个附加的隐含形参，即指向该类对象的一个指针。这个隐含形参命名为 this。
成员函数不能定义 this 形参，成员函数的函数体可以显式使用 this指针，但不是必须这么做。**如果对类成员的引用没有限定，编译器会将这种引用处理成通过 this 指针的引用。**

### 何时使用 this 指针

尽管**在成员函数内部显式引用 this** 通常是不必要的，但有一种情况下必须这样做：**当我们需要将一个对象作为整体引用而不是引用对象的一个成员时**。最常见的情况是在这样的函数中使用this：该函数返回对调用该函数的对象的引用。 
某种类可能具有某些操作，这些操作应该返回引用.我们可以给Screen类添加下面的
操作。
•   一对 set 操作，将特定字符或光标指向的字符设置为给定值。 
•   一个 move 操作，给定两个 index 值，将光标移至新位置
理想情况下，希望用户能够将这些操作的序列连接成一个单独的表达式：

    // move cursor to given position, and set that character 
         myScreen.move(4,0).set('#');

#### 返回 *this
在单个表达式中调用 move 和 set 操作时，每个操作必须返回一个引用，该引用指向执行操作的那个对象：

    class Screen { 
         public:
              Screen& move(index r, index c); 
              Screen& set(char); 
         }; 

**这些函数的返回类型是 Screen&，指明该成员函数返回对其自身类类型的对象的引用。**
下面是对两个新成员的实现：

    Screen& Screen::set(char c) 
         { 
             contents[cursor] = c; 
             return *this; 
         } 
         Screen& Screen::move(index r, index c) 
         { 
             index row = r * width; // row location 
             cursor = row + c; 
             return *this; 
         }


#### 从 const 成员函数返回 *this 
在**普通的非 const 成员函数**中，this 的类型是一个**指向类类型的 const 指针**（第 4.2.5 节）。可以改变 this 所指向的值，但不能改变 this 所保存的地址。

在 **const 成员函数中**，this 的类型是一个**指向 const 类类型对象的 const 指针**。既不能改变 this 所指向的对象，也不能改变 this 所保存的地址。

不能从 const 成员函数返回指向类对象的普通引用。**const 成员函数只能返回 *this 作为一个 const 引用。**

#### 基于 const 的重载
基于成员函数是否为 const，可以重载一个成员函数；同样地，基于一个指针形参是否指向 const，可以重载一个函数。
const 对象只能使用 const 成员。非 const 对象可以使用任一成员，但非 const 版
本是一个更好的匹配（重载时先调用这个版本）。

    Screen& display(std::ostream &os) 
                           { do_display(os); return *this; } 
             const Screen& display(std::ostream &os) const 
                           { do_display(os); return *this; }

当我们将 display 嵌入到一个长表达式中时，将调用非 const 版本。当我们 display 一个 const 对象时，就调用 const 版本： 
 

    Screen myScreen(5,3); 
     const Screen blank(5, 3); 
     myScreen.set('#').display(cout); // calls nonconst version 
     blank.display(cout);             // calls const version


#### 可变数据成员
有时（但不是很经常），我们希望类的数据成员（甚至在 const 成员函数内）可以修改。这可以通过将它们声明为 mutable 来实现。
可变数据成员（mutable data member） 永远都不能为 const。

    mutable size_t access_ctr; // may change in a const members


## 类作用域
**每个类都定义了自己的新作用域和唯一的类型**。即使两个类具有完全相同的成员列表，**它们也是不同的类型（对象之间不能直接赋值）**。每个类的成员不同于任何其他类（或任何其他作用域）的成员。

### 使用类的成员

    Class obj;   // Class is some class type
    Class *ptr = &obj; 
     ptr->member;   
     obj.member;  

#### 形参表和函数体处于类作用域中
在定义于类外部的成员函数中，形参表和成员函数体都出现在成员名之后。
这些都是在类作用域中定义，所以可以不用限定而引用其他成员。

    char Screen::get(index r, index c) const 
         { 
             index row = r * width;      // compute the row location 
     
             return contents[row + c];   // offset by c to fetch specified character 
         }

因为形参表是在 Screen 类的作用域内，所以不必指明我们想要的是 Screen::index。同样，使用 index、width 和 contents 时指的都是 Screen 类中声明的名字。

#### 函数返回类型不一定在类作用域中

    class Screen { 
         public: 
             typedef std::string::size_type index; 
             index get_cursor() const; 
         };
         
         Screen::index Screen::get_cursor() const 
         { 
             return cursor; 
         }

**在类外定义成员时,返回类型是类内的类型则要加作用域(Screen::index)**:如果在类定义体之外定义get_cursor且**类型前不加作用域，则在函数名被处理之前，代码在不在类作用域内**。当看到返回类型时，其名字是在类作用域之外使用。必须用完全限定的类型名 Screen::index 来指定所需要的 index 是在类 Screen 中定义的名字。

#### 类作用域中的名字查找
1.  首先，在使用该名字的块中查找名字的声明。只考虑在该项使用之前声明
的名字。 
2.  如果找不到该名字，则在包围的作用域中查找。如下：

**（1）函数作用域之后，在类作用域中查找；**

**（2）类作用域之后，在外围作用域中查找（全局函数，全局变量等）。** 
  
## 构造函数
构造函数是特殊的成员函数，**只要创建类类型的新对象，都要执行构造函数**。
1.构造函数可以被重载

2.实参决定使用哪个构造函数

3.构造函数自动执行

### 构造函数初始化式
与任何其他函数一样，构造函数具有名字、形参表和函数体。与其他函数不同的是，**构造函数也可以包含一个构造函数初始化列表:**

    Sales_item::Sales_item(const string &book): 
              isbn(book), units_sold(0), revenue(0.0) { }

这个构造函数将 isbn 成员初始化为 book 形参的值，将 units_sold 和 revenue 初始化为 0。
**省略初始化列表在构造函数的函数体内对数据成员赋值是合法的**。例如，可以将接受一个 string 的 Sales_item 构造函数编写为： 

    Sales_item::Sales_item(const string &book) 
         { 
             isbn = book; 
             units_sold = 0; 
             revenue = 0.0; 
         } 

**这个构造函数给类 Sales_item 的成员赋值**，**但没有进行显式初始化**。不管
是否有显式的初始化式，在执行构造函数之前，要初始化 isbn 成员。这个构造函数**隐式使用了默认的 string 构造函数来初始化isbn**。
执行构造函数的函数体时，isbn 成员已经有值了。**该值被构造函数函数体中的赋值所覆盖**。
这个构造函数分两段执行:（1）初始化阶段；（2）普通的计算阶段

#### 有时需要构造函数初始化列表

如果没有为类成员提供初始化式，则编译器会隐式地使用成员类型的默认构造函数。如果那个**类没有默认构造函数，则编译器尝试使用默认构造函数将会失败**。

> 因为内置类型的成员不进行隐式初始化，所以对这些成员是进行初始化还是赋值似乎都无关紧要。除了const对象和引用类型的对象，对非类类型的数据成员进行赋值或使用初始化式在结果和性能上都是等价的。


**可以初始化 const 对象或引用类型的对象，但不能对它们赋值**:

    class ConstRef { 
         public: 
             ConstRef(int ii); 
         private: 
             int i; 
             const int ci; 
             int &ri; 
         }; 
         ConstRef::ConstRef(int ii) 
         {              // assignments: 
              i = ii;   // ok 
              ci = ii;  // error: cannot assign to a const 
              ri = i;   // assigns to ri which was not bound to an object 
         }

**结论:必须对任何 const 或引用类型成员以及没有默认构造函数的类类型的任何成员使用初始化式。**

#### 成员初始化的次序

**按照与成员声明一致的次序编写构造函数初始化列表**是个好主意。此外，尽可能避免使用成员来初始化其他成员。

    class X { 
             int i; 
             int j; 
         public: 
            X(int val): i(val), j(val) { } //根据类中成员的声明顺序写初始化列表
         };

#### 初始化式可以是任意表达式

#### 类类型的数据成员的初始化式

**初始化类类型的成员时**，要指定实参并传递给成员类型的一个构造函数。**可以使用该类型的任意构造函数**。Sales_item 类可以使用任意一个 string 构造函数来初始化 isbn:

    Sales_item(): isbn(10, '9'), units_sold(0), revenue(0.0) {}

### 默认实参与构造函数

        Sales_item(const std::string &book): 
                   isbn(book), units_sold(0), revenue(0.0) { } 
                   
         Sales_item(): units_sold(0), revenue(0.0) { }
        //（隐式地）使用 string 的默认构造函数来初始化 isbn

可以通过为 string 初始化式提供一个默认实参将这些构造函数组合起来：

    Sales_item(const std::string &book = ""): 
                       isbn(book), units_sold(0), revenue(0.0) { }
                      //定义了string的默认形参

**在 下方empty 的情况下，使用默认实参("")，而 Primer_3rd_ed提供了一个显式实参。**  
 

    Sales_item empty; 
     Sales_item Primer_3rd_Ed("0-201-82470-1");

### 默认构造函数

使用默认构造函数:
初级 C++ 程序员常犯的一个错误是，采用以下方式声明一个用默认构造函数初始化的对象：

    Sales_item myobj(); // oops! declares a function, not an object

这种方式声明了一个名为myobj的函数.

使用默认构造函数定义一个对象的正确方式是去掉最后的空括号： 
 

    // ok: defines a class object ... 
     Sales_item myobj; 
或者

    // ok: create an unnamed, empty Sales_item and use to initialize myobj 
         Sales_item myobj = Sales_item(); 
在这里，我们创建并初始化一个 Sales_item 对象，然后用它来按值初始化 myobj。

#### 隐式类型转换

    Sales_item(const std::string &book = ""): 
                       isbn(book), units_sold(0), revenue(0.0) { } 
             Sales_item(std::istream &is);

这里的每个构造函数都定义了一个隐式转换。因此，在期待一个 Sales_item 
类型对象的地方，可以使用一个 string ： 

    string null_book = "9-999-99999-9"; 
         // ok: builds a Sales_itemwith 0 units_soldand revenue from 
         // and isbn equal to null_book 
         item.same_isbn(null_book); 

编译器使用接受一个 string 的 Sales_item 构造函数从 null_book 生成一个新的 Sales_item 对象。**新生成的（临时的）Sales_item 被传递给 same_isbn**。

从 istream 到 Sales_item 的转换：
 

    item.same_isbn(cin);

该构造函数通过读标准输入来创建一个（临时的）Sales_item 对象。然后该对象被传递给 same_isbn。

#### 抑制由构造函数定义的隐式转换
**将构造函数声明为 explicit,则构造函数不能用于隐式地创建对象**。explicit 关键字只能用于类内部的构造函数声明上，不能用于类的外部定义。

             explicit Sales_item(std::istream &is);

#### 为转换而显式地使用构造函数

    string null_book = "9-999-99999-9"; 
    item.same_isbn(Sales_item(null_book));
    

显式使用构造函数只是中止了隐式地使用构造函数。**任何构造函数都可以用来显式地创建临时对象。**

### 类成员的显式初始化

对于没有定义构造函数并且其全体数据成员均为 public 的类，**可以采用与初始化数组元素相同的方式初始化其成员**

    struct Data { 
             int ival; 
             char *ptr; 
         };
    Data val2 = { 1024, "Anna Livia Plurabelle" };

必须根据数据成员的声明次序来使用初始化式。

> 显式初始化容易出错，使用构造函数更加好！

## 友元

**友元机制允许一个类将对其非公有成员的访问权授予指定的函数或类。**
友元的声明以关键字 friend 开始。它只能出现在类定义的内部。友元声明可以出现
在类中的任何地方：友元不是授予友元关系的那个类的成员，所以它们不受声明出现部分的访问控制影响。
Screen 应该允许 Window_Mgr 像下面这样访问其成员： 

    class Screen { 
             // Window_Mgr members can access private parts of clas s Screen 
             friend class Window_Mgr; 
             // ...rest of the Screen class 
         }; 

Window_Mgr 的成员可以直接引用 Screen 的私有成员。

    Window_Mgr& 
         Window_Mgr::relocate(Screen::index r, Screen::index c, Screen& s) 
         { 
              // ok to refer to height and width 
              s.height += r; 
              s.width += c; 
     
              return *this; 
         }

**友元**可以是**普通的非成员函数**，**其他类的成员函数**，或**整个类**。
**将一个类设为友元，友元类的所有成员函数都可以访问授予友元关系的那个类的非公有成员。** 

### 友元声明与作用域

必须先定义包含成员函数的类，才能将成员函数设为友元。另一方面，不必预先声明类和非成员函数来将它们设为友元。

## static 类成员

对于特定类类型的全体对象而言，访问一个全局对象有时是必要的。
然而，**全局对象会破坏封装**：对象需要支持特定类抽象的实现。如果对象是全局的，一般的用户代码就可以修改这个值。
**类可以定义类静态成员，而不是定义一个可普遍访问的全局对象。** 
不像普通的数据成员，**static 数据成员独立于该类的任意对象而存在**；每个 **static 数据成员是与类关联的对象，并不与该类的对象相关联**。 

正如类可以定义共享的 static 数据成员一样，类也可以定义 static 成员函数。**static 成员函数没有 this 形参，它可以直接访问所属类的 static 成员，但不能直接使用非 static 成员**。

使用 static 成员而不是全局对象有三个优点：
1.  static 成员的名字是**在类的作用域中**，因此可以**避免与其他类的成员或全局对象名字冲突**。 
2.  可以**实施封装**。static 成员可以是私有成员，而全局对象不可以。 
3.  通过阅读程序容易看出 static 成员是与特定类关联的。这种可见性可清晰地显示程序员的意图。

### 定义 static 成员

在成员声明前加上关键字 static 将成员设为 static。**static 成员遵循正常的公有／私有访问规则**。

    class Account { 
         public: 
             // interface functions here 
             void applyint() { amount += amount * interestRate; } 
             static double rate() { return interestRate; } 
             static void rate(double); // sets a new rate 
         private: 
             std::string owner; 
             double amount; 
             static double interestRate; 
             static double initRate(); 
         }; 

interestRate 成员由 Account 类型的全体对象共享。

### 使用类的 static 成员

可以通过作用域操作符从类直接调用 static 成员，或者通过对象、引用或指向该类类型对象的指针间接调用。

    Account ac1; 
         Account *ac2 = &ac1; 
         // equivalent ways to call the static member rate function 
         double rate; 
         rate = ac1.rate();        // through an Account object or reference 
         rate = ac2->rate();       // through a pointer to an Account object 
         rate = Account::rate();   // directly from the class using the scope operator

像使用其他成员一样，**类成员函数可以不用作用域操作符**来**引用类的 static 成员**：

    class Account { 
         public: 
              // interface functions here 
              void applyint() { amount += amount * interestRate; } 
         };

### static 成员函数

1.static 关键字只出现在类定义体内部的声明处，外部定义处没有。
2.static 函数没有 this 指针
3.因为 static 成员不是任何对象的组成部分，所以 **static 成员函数不能被声明为 const**。毕竟，将成员函数声明为 const 就是承诺不会修改该函数所属的对象。

### static 数据成员


static 数据成员必须在类定义体的外部定义,且不是通过类构造函数进行初始化，而是**在定义时进行初始化**。
可以定义如下 interestRate：

    // define and initialize static class member 
         double Account::interestRate = initRate();

这个语句定义名为 interestRate 的 static 对象，它是类 Account 的成员，为 double 型。**像其他成员定义一样，一旦成员名出现，static 成员的就是在类作用域中**。因此，我们可以没有限定地直接使用名为 initRate 的 static 成员函数，作为 interestRate 初始化式。

#### 特殊的整型 const static 成员

一般而言，**类的 static 成员**，**像普通数据成员一样，不能在类的定义体中初始化**。相反，static 数据成员通常在定义时才初始化。
例外是，只要初始化式是一个常量表达式，整型 const static 数据成员就可以在类的定义体中进行初始化： 

    private: 
             static const int period = 30;
             

## 小结

数据抽象是指定义数据和函数成员的能力，而封装是指从常规访问中保护类成员的能力，它们都是类的基础。成员函数定义类的接口。**通过将类的实现所用到的数据和函数设置为 private 来封装类**。

类可以定义构造函数，它们是特殊的成员函数，控制如何初始化类的对象。

类可以将**对其非 public 成员的访问权授予其他类或函数**，并通过**将其他的类或函数设为友元来授予其访问权**。 

类也可以定义 mutable 或 static 成员**。mutable 成员**永远都不能为 const；**它的值可以在 const 成员函数中修改**。**static** 成员可以是函数或数据，**独立于类类型的对象而存在**。
