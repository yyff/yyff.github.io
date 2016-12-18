---
layout: article
title:  "c++ primer读书笔记--第十五章 面向对象编程"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

面向对象编程基于三个基本概念：**数据抽象、继承和动态绑定**。
在 C++ 中，用类进行数据抽象，用派生类从一个类继承另一个：派生类继承基类的成员。动态绑定使编译器能够在运行时决定是使用基类中定义的函数还是派生类中定义的函数。

## 面向对象编程：概述

面向对象编程的关键思想是**多态性**（polymorphism）。意思是“许多形态”。在 C++ 中，多态性仅用于**通过继承而相关联的类型的引用或指针**。

### 继承
派生类（derived class）能够继承基类（base class）定义的成员，派生类可以无须改变而使用那些与派生类型具体特性不相关的操作，派生类可以重定义那些与派生类型相关的成员函数。

### 动态绑定(virtual函数)
动态绑定使我们能够编写程序**使用继承层次中任意类型的对象**，无须关心对象的具体类型。**使用这些类的程序无须区分函数是在基类还是在派生类中定义的**。
下列函数print_total打印 ISBN 以及购买给定数量的某书的总价：

	 void print_total(ostream &os, const Item_base &item, size_t n) 
     {
     os << "ISBN: " << item.book() // calls Item_base::book 
             << "\tnumber sold: " << n << "\ttotal price: " 
             // virtual call: which version of net_price to call is resolved at run time 
             << item.net_price(n) << endl; 
     }

第一，虽然这个**函数的第二形参是 Item_base 的引用但可以将 Item_base 对象或 Bulk_item 对象传给它**。
第二，因为形参是引用且 net_price 是虚函数，所以对 net_price 的调用将在运行时确定。调用哪个版本的 net_price 将依赖于传给 print_total 的实参是Item_base 对象还是Bulk_item 。

在 C++ 中，通过基类的引用（或指针）调用虚函数时，发生动态绑定。**基类的引用（或指针）既可以指向基类对象也可以指向派生类对象**，这一事实是动态绑定的关键。用引用（或指针）调用的**虚函数在运行时确定**，**被调用的函数是由引用（或指针）所指对象决定的**。

## 定义基类和派生类

### 定义基类

	 class Item_base { 
     public: 
         Item_base(const std::string &book = "", 
                   double sales_price = 0.0): 
                          isbn(book), price(sales_price) { } 
         std::string book() const { return isbn; } 
     
         virtual double net_price(std::size_t n) const 
                    { return n * price; } 
         virtual ~Item_base() { } 
     private: 
         std::string isbn;     // identifier for the item 
     protected: 
         double price;         // normal, undiscounted price 
     };

构造函数使用默认实参（第 7.4.1 节），允许用 0 个、1 个或2个实参进行调用，它用这些实参初始化数据成员。
**调用包含默认实参的函数时，可以为该形参提供实参，也可以不提供。如果提供了实参，则它将覆盖默认的实参值；否则，函数将使用默认实参值**。

### 基类成员函数 

Item_base 类定义了两个函数，其中一个前面带有保留字virtual。**保留字 virtual 的目的是启用动态绑定，指明函数为虚函数**。成员默认为非虚函数，对非虚函数的调用在编译时确定。
除了构造函数之外，任意非 static成员函数都可以是虚函数。**保留字只在类内部的成员函数声明中出现**，不能用在类定义体外部出现的函数定义上。
基类通常应将派生类需要重定义的任意函数定义为虚函数。

### 访问控制和继承

**private**：只能由1.该类中的函数、2.其友元函数访问。不能被任何其他访问，该类的对象也不能访问。

**protected**：可以被1.该类中的函数、2.其友元函数访问、3.子类的函数。**但不能被该类的对象访问**。

**public**：可以被1.该类中的函数、2.其友元函数访问、3.子类的函数，4.该类的对象访问。

如果没有继承，类只有两种用户：类本身的成员和该类的用户。将类划分为 private 和 public 访问级别反映了用户种类的这一分隔：用户只能访问 public 接口，类成员和友元既可以访问 public 成员也可以访问 private 成员。 
有了继承，就有了类的第三种用户：从类派生定义新类的程序员。**派生类的提供者通常（但并不总是）需要访问（一般为 private 的）基类实现**，为了允许这种访问而仍然禁止对实现的一般访问，**提供了附加的 protected 访问标号**。类的 **protected部分仍然不能被一般程序访问，但可以被派生类访问**。只有类本身和友元可以访问基类的 private 部分，派生类不能访问基类的 private 成员。

#### **类如何使用访问标号**

接口函数应该为 public 而数据一般不应为 public。
希望**禁止派生类访问的成员应该设为 private**，**提供派生类实现所需操作或数据的成员应设为 protected**。
换句话说，提供给派生类型的接口是 protected 成员和 public成员的组合。 

### 派生类

为了定义派生类，使用**类派生列表**指定基类。类派生列表指定了一个或多个基类，具有如下形式： 
 

    class classname: access-label base-class

这里access-label（访问标号） 是 public、protected 或private，base-class 是已定义的类的名字

#### **定义派生类**

	 class Bulk_item : public Item_base { 
     public: 
     // redefines base version so as to implement bulk purchase    discount policy
         double net_price(std::size_t) const; 
     private: 
         std::size_t min_qty; // minimum purchase for discount  to apply 
         double discount;     // fractional discount to apply 
      };

每个 Bulk_item 对象包含四个数据成员：从 Item_base 继承的 isbn 和 price，自己定义的 min_qty 和 discount。

#### **派生类和虚函数**

派生类一般会重定义所继承的虚函数。派生类没有重定义某个虚函数，则使用基类中定义的版本。
派生类中虚函数的声明（第 7.4节）必须与基类中的定义方式完全匹配，但有一个例外：返回对基类型的引用（或指针）的虚函数。派生类中的虚函数可以返回基类函数所返回类型的派生类的引用（或指针）。 
例如，Item_base 类可以定义返回 Item_base* 的虚函数，如果这样，Bulk_item 类中定义的实例可以定义为返回 Item_base* 或Bulk_item*。

#### **派生类对象包含基类对象作为子对象**
派生类本身定义的（非 static）成员加上由基类（非 static）成员组成的子对象。

#### **派生类中的函数可以使用基类的成员**
因为每个派生类对象都有基类部分，**类可以访问基类的 public 和 protected 成员，就好像那些成员是派生类自己的成员一样**。

#### **用派生类作基类**

    class Base { /* ... */ }; 
     class D1: public Base { /* ... */ }; 
     class D2: public D1 { /* ... */ };

D2可以访问D1和Base的public 和 protected 成员。

#### **派生类的声明**
如果需要声明（但并不实现）一个派生类，则**声明应包含类名但不能包含派生列表**。例如，下面的前向声明会导致编译时错误：

// error: a forward declaration must not include the deriv ation list 
     class Bulk_item : public Item_base;

正确的前向声明为：

	 // forward declarations of both derived and nonderived class 
     class Bulk_item; 
     class Item_base;

#### **从派生类型到基类的转换**
**因为每个派生类对象都包含基类部分**，**所以可将基类类型的引用绑定到派生类对象的基类部分，也可以用指向基类的指针指向派生类对象**。

	Item_base item;
	Item_base *p = &item // p points to an Item_base object
	Bulk_item bulk;
	p = &bulk; // p points to the Item_base part of bulk 


#### **可以在运行时确定 virtual 函数的调用**

    Item_base base; 
     Bulk_item derived; 
     // print_total makes a virtual call to net_price 
     print_total(cout, base, 10);     // calls Item_base::net_price 
     print_total(cout, derived, 10);  // calls Bulk_item::net_price

编译时只知道上述代码会调用print_total函数（），却不知道该函数的第二个形参是Item_base类对象还是Bulk_item类对象，只能在运行时确定。
c++动态性：只有通过引用或指针调用，虚函数才在运行时确定。只有在这些情况下，直到运行时才知道对象的动态类型。

#### 在编译时确定非  virtual   调用
非虚函数总是在编译时根据调用该函数的对象、引用或指针的类型而确定。

#### **覆盖虚函数机制**
希望覆盖虚函数机制并强制函数调用使用虚函数的特定版本，这里可以使用作用域操作符：

	Item_base *baseP = &derived; 
     // calls version from the base class regardless of the dynamic type of baseP 
     double d = baseP->Item_base::net_price(42);

这段代码强制将 net_price 调用确定为 Item_base中定义的版本，**该调用将在编译时确定**。
为什么会希望覆盖虚函数机制？最常见的理由是为了派生类虚函数调用基
类中的版本。
**派生类虚函数调用基类虚函数版本时**，**必须显式使用作用域操作符**。如果派生类函数忽略了这样做，则函数调用会在运行时确定并且将是一个自身调用，从而导致无穷递归。

### 公用、私有和受保护的继承

派生类不能访问基类的 private 成
员，也不能使自己的用户能够访问那些成员。**如果基类成员为 public 或protected，则派生列表中使用的访问标号决定该成员在派生类中的访问级别**：
•   如果是**公用继承**，基类成员保持自己的访问级别：基类的 public成员为派生类的 public 成员，基类的 protected 成员为派生类的 protected 成员。 
•   如果是**受保护继承**，基类的 public 和 protected成员在派生类中为protected 成员。 
•   如果是**私有继承**，基类的的所有成员在派生类中为 private 成员。

#### **继承与组合** 

定义一个类作为另一个类的公用派生类时，派生类应反映与基类的“**是一种（Is A）”关系**。例如:一个表示人的基类,派生类可以为男人和女人,**都表示是一种人**.

#### **去除个别成员**

	 class Derived : private Base { 
     public: 
        using Base::size; 
     protected: 
         using Base::n; 
         // ... 
      }; 

上述的派生类Derived 可以是私有继承Base的情况下,通过一个using 声明来使用基类Base中的成员。

#### **默认继承保护级别**

    class Base { /* ... */ }; 
     struct D1 : Base { /* ... */ };   // public inheritance by default 
     class D2 : Base { /* ... */ };    // private inheritance by default


#### **友元关系与继承**
**友元关系不能继承**。基类的友元对派生类的成员没有特殊访问权限。如果基类被授予友元关系，则只有基类具有特殊访问权限，该基类的派生类不能访问授予友元关系的类。

### 继承与静态成员
如果基类定义 **static 成员**（第 12.6节），则**整个继承层次中只有一个这样的成员**。
static 成员遵循常规访问控制：如果成员在基类中为private，则派生类不能访问它。

## 转换与继承 
我们已经看到，每个派生类对象包含一个基类部分，这意味着可以像使用基类对象一样在派生类对象上执行操作。因为派生类对象也是基类对象，所以**存在从派生类型引用到基类类型引用的自动转换**，即，可以将派生类对象的引用转换为基类子对象的引用，对指针也类似。

### 派生类到基类的转换
如果有一个派生类型的对象，则可以使用它的地址对基类类型的指针进行赋值或初始化。同样，可以使用派生类型的引用或对象初始化基类类型的引用。

#### **引用转换不同于转换对象**
1.**将对象传给希望接受引用的函数时，引用直接绑定到该对象**，虽然看起来在传递对象，实际上实参是该对象的引用，**对象本身未被复制**，并且，转换不会在任何方面改变派生类型对象，该对象仍是派生类型对象。
2.**将派生类对象传给希望接受基类类型对象（而不是引用）的函数时**，情况完全不同。在这种情况下，**形参的类型是固定的——在编译时和运行时形参都是基类类型对象**。如果**用派生类型对象调用这样的函数**，则该**派生类对象的基类部分被复制到形参**。
 
#### **用派生类对象对基类对象进行初始化或赋值** 

对基类对象进行初始化或赋值，**实际上是在调用函数**：初始化时调用构造函数，赋值时调用赋值操作符。

	 class Derived; 
     class Base { 
     public: 
         Base(const Derived&);  // create a new Base from a Derived 
         Base &operator=(const Derived&);  // assign from a Derived 
         // ... 
     }; 


用 **Bulk_item 类型的对象调用 Item_base类的复制构造函数或赋值操作符**时，将发生下列步骤： 
•   **将 Bulk_item 对象转换为 Item_base 引用**，这仅仅意味着将一个 Item_base 引用绑定到 Bulk_item 对象。 
•   将该引用作为实参传给复制构造函数或赋值操作符。 
•   那些操作符使用 Bulk_item 的 Item_base部分分别对调用构造函数或赋值的 Item_base 对象的成员进行初始化或赋值。 
•   一旦操作符执行完毕，对象即为 Item_base。它包含 Bulk_item 的 Item_base 部分的副本，但实参的 Bulk_item 部分被忽略。

#### **派生类到基类转换的可访问性**

转换后是否可访问取决于在派生类的派生列表中指定的访问标号。


### 基类到派生类的转换

没有从基类类型到派生类型的（自动）转换，原因在于基类对象只能是基类对象，它不能包含派生类型成员。
如果知道从基类到派生类的转换是安全的，就可以**使用 static_cast（第 5.12.4 节）强制编译器进行转换**。或者，可以用 dynamic_cast 申请在运行时进行检查。

## 构造函数和复制控制

每个派生类对象由派生类中定义的（非static）成员加上一个或多个基类
子对象构成，这样对派生类型对象的操作，也会构造、复制、赋值和撤销这些基类子对象。
构造函数和复制控制成员不能继承，每个类定义自己的构造函数和复制控制成员。

### 基类构造函数和复制控制

像任意其他成员一样，构造函数可以为 protected 或 private，某些类需要只希望派生类使用的特殊构造函数，这样的构造函数应定义为protected。

### 派生类构造函数

派生类的构造函数受继承关系的影响，每个**派生类构造函数除了初始化自己的数据成员之外，还要初始化基类**。

#### **合成的派生类默认构造函数**

派生类的合成默认构造函数除了初始化派生类的数据成员之外，它还初始化派生类对象的基类部分。**基类部分由基类的默认构造函数初始化**。
对于 Bulk_item 类，合成的默认构造函数会这样执行： 
1.  调用 Item_base 的默认构造函数，将 isbn 成员初始化空串，将price 成员初始化为 0。 
2.  用常规变量初始化规则初始化 Bulk_item 的成员，也就是说，qty 和 discount 成员会是未初始化的。

#### **定义默认构造函数**

**因为 Bulk_item 具有内置类型成员，所以应定义自己的默认构造函数**（全为类类型时，可以隐式调用他们各自的默认构造函数）： 

	 class Bulk_item : public Item_base { 
     public: 
         Bulk_item(): min_qty(0), discount(0.0) { } 
         // as before 
     }; 

首先使用 Item_base 的默认构造函数初始化 Item_base 部分，那个构造函数将 isbn 置为空串并将 price 置为 0。再初始化 Bulk_item 部分的成员并执行构造函数的函数体

#### **向基类构造函数传递实参**

派生类构造函数的初始化列表只能初始化派生类的成员，不能直接初始化继承成员。相反**派生类构造函数通过将基类包含在构造函数初始化列表中来间接初始化继承成员**。

	 class Bulk_item : public Item_base { 
     public: 
         Bulk_item(const std::string& book, double sales_price, 
                   std::size_t qty = 0, double disc_rate = 0.0): 
                      Item_base(book, sales_price), 
                      min_qty(qty), discount(disc_rate) { } 
         // as before 
      };

这个构造函数使用有两个形参 Item_base 构造函数初始化基类子对象。

#### **在派生类构造函数中使用默认实参**

	 class Bulk_item : public Item_base { 
     public: 
         Bulk_item(const std::string& book, double sales_price, 
                   std::size_t qty = 0, double disc_rate = 0.0): 
                      Item_base(book, sales_price), 
                      min_qty(qty), discount(disc_rate) { } 
         // as before 
      }; 

这里为每个形参提供了默认值，因此，可以用 0 至 4个实参使用该构造函数。

#### 只能初始化直接基类

一个类只能初始化自己的直接基类。直接就是在派生列表中指定的类。

#### 重构

将 Disc_item (折扣类)加到 Item_base和Bulk_item层次之间是重构（refactoring）的一个例子。重构包括重新定义类层次，将操作和／或数据从一个类移到另一个类。
重构常见在面向对象应用程序中非常常见。值得注意的是，虽然改变了
继承层次，使用 Bulk_item 类或 Item_base 类的代码不需要改变。
Disc_item类的设计如下:

	 class Disc_item : public Item_base { 
     public: 
         Disc_item(const std::string& book = "", 
                   double sales_price = 0.0, 
                   std::size_t qty = 0, double disc_rate = 0.0): 
                      Item_base(book, sales_price), 
                      quantity(qty), discount(disc_rate) { } 
         protected: 
             std::size_t quantity; // purchase size for discount to apply 
             double discount;      // fractional discount to apply 
      };

定义了自己的 discount 和 quantity 成员来实现折扣.
重新实现 Bulk_item 以继承 Disc_item:

	 class Bulk_item : public Disc_item { 
     public: 
         Bulk_item(const std::string& book = "", 
                   double sales_price = 0.0, 
                   std::size_t qty = 0, double disc_rate = 0.0): 
              Disc_item(book, sales_price, qty, disc_rate) { } 
         double net_price(std::size_t) const; 
     };

### 复制控制和继承

只包含类类型或内置类型数据成员、不含指针的类一般可以使用合成操作，复制、赋值或撤销这样的成员不需要特殊控制。具有指针成员的类一般需要定义自己的复制控制来管理这些成员。

#### **定义派生类复制构造函数**

**如果派生类显式定义自己的复制构造函数或赋值操作符，则该定义将完全覆盖默认定义**。被继承类的复制构造函数和赋值操作符负责对基类成分以及类自己的成员进行复制或赋值。
如果**派生类定义了自己的复制构造函数**，该复制构造函数一般**应显式使用基类复制构造函数**初始化对象的基类部分：

	 class Base { /* ... */ }; 
     class Derived: public Base { 
     public: 
         // Base::Base(const Base&) not invoked automatically 
         Derived(const Derived& d): 
              Base(d) /*Derived  member initialization */ { /*... */ } 
     }; 

初始化函数 Base(d) 将派生类对象 d 转换（第 15.3 节）为它的基类部分的引用，并调用基类复制构造函数。
若不显示指定基类构造函数,派生类中的基类部分会使用基类的默认构造函数,而不会使用实参中的基类部分.如下:

	Derived(const Derived& d) /* derived member initizations */ 
	                                   {/* ... */ }

#### **派生类赋值操作符**

	 Derived &Derived::operator=(const Derived &rhs) 
     { 
        if (this != &rhs) { 
            Base::operator=(rhs); // assigns the base part 
            // assign the members from the derived 
        } 
        return *this; 
     }

赋值操作符必须防止自身赋值。假定左右操作数不同，则调用 Base 类的赋
值操作符给基类部分赋值。

#### **派生类析构函数(通过delete调用)**

析构函数的工作与复制构造函数和赋值操作符不同：**派生类析构函数不负责撤销基类对象的成员**。**编译器总是显式调用派生类对象中基类部分的析构函数**。每个析构函数只负责清除自己的成员：

	 class Derived: public Base { 
     public: 
         // Base::~Base invoked automatically 
         ~Derived()    { /* do what it takes to clean up derived members */ } 
      };

对象的撤销顺序与构造顺序相反：首先运行派生析构函数，然后按继承层次依次向上调用各基类析构函数。

### **虚析构函数**
处理继承层次中的对象时，**指针的静态类型可能与被删除对象的动态类型不同**，**可能会错误的删除实际指向派生类对象的基类类型指针**。
如果删除基类指针，则需要运行基类析构函数并清除基类的成员，如果对象
实际是派生类型的，则没有定义该行为。
要保证运行适当的析构函数，基类中的析构函数必须为虚函数：

	 class Item_base { 
     public: 
         virtual ~Item_base() { } 
     };

如果**析构函数为虚函数**，那么通过指针调用时，**运行哪个析构函数将因指针所指对象类型的不同而不同**： 

    Item_base *itemP = new Item_base; // 指针的静态类型与被删除对象的动态类型相同
     delete itemP;          // ok: destructor for Item_base called 
     itemP = new Bulk_item; // 指针的静态类型与被删除对象的动态类型不同 
     delete itemP;          // ok: destructor for Bulk_item called

像其他虚函数一样，析构函数的虚函数性质都将继承。因此，如果层次中根类的析构函数为虚函数，则派生类析构函数也将是虚函数，无论派生类显式定义析构函数还是使用合成析构函数，**派生类析构函数都是虚函数**。 

即使析构函数没有工作要做，**继承层次的根类也应该定义一个虚析构函数**。

#### **构造函数和赋值操作符不应是虚函数**

### 构造函数和析构函数中的虚函数

构造派生类对象时首先运行基类构造函数初始化对象的基类部分。**在执行基类构造函数时**，对象的派生类部分是未初始化的。实际上，**此时对象还不是一个派生类对象**。
为了适应这种不完整,初始化派生类对象时,在基类构造函数或析构函数中，将派生类对象当作基类类型对象对待。
如果在构造函数或析构函数中调用虚函数，则运行的是为构造函数或析构函数自身类型定义的版本。

## 继承情况下的类作用域 

每个类都保持着自己的作用域，在该作用域中定义了成员的名字。在继承情况下，派生类的作用域嵌套在基类作用域中。**如果不能在派生类作用域中确定名字，就在基类作用域中查找该名字的定义**。
 
### 名字查找在编译时发生
**对象、引用或指针的静态类型决定了对象能够完成的行为**。甚至当静态类型和动态类型可能不同的时候(**就像使用基类类型的引用或指针指向派生类对象时,它仍然只能访问派生类对象的基类部分**)。静态类型仍然决定着可以使用什么成员。

### 名字冲突与继承
与基类成员同名的派生类成员将屏蔽对基类成员的直接访问。
**可以使用作用域操作符访问被屏蔽成员。**

### 作用域与成员函数
**在基类和派生类中使用同一名字的成员函数(即使形参表不同)**，在派生类作用域中派生类成员**将屏蔽基类成员**。

	 struct Base { 
         int memfcn(); 
     }; 
     struct Derived : Base { 
         int memfcn(int); // hides memfcn in the base 
     }; 
     Derived d;
     d.memfcn(10);      // calls Derived::memfcn 
     d.memfcn();        // error: memfcn with no arguments is hidden
     d.Base::memfcn();  // ok: calls Base::memfcn

d.memfcn()这个调用编译器需要查找名字 memfcn，并在 Derived 类中找到。**一旦找到了名字，编译器就不再继续查找了**。
**局部作用域中声明的函数不会重载全局作用域中定义的函数**（第 7.8.1 节），同样，**派生类中定义的函数也不重载基类中定义的成员**。

### 虚函数与作用域

要获得**动态绑定，必须通过基类的引用或指针调用虚成员（根据指向的对象区分）**。当我们这样做时，编译器器将在基类中查找函数。假定找到了名字，编译器就检查实参是否与形参匹配。
**虚函数必须在基类和派生类中拥有同一原型（函数名和形参表相同）**。如果基类成员与派生类成员接受的实参不同，就没有办法通过基类类型的引用或指针调用派生类函数。

	 class Base { 
     public: 
         virtual int fcn(); 
     }; 
     class D1 : public Base { 
     public: 
          
          int fcn(int); //  隐藏了Base中的虚函数fcn()
          // D1 继承了 Base::fcn() ，但是没有重定义fcn()，因此用指向D1对象的基类指针调用fcn()时只会调用Base::fcn()
     }; 
     class D2 : public D1 { 
     public: 
         int fcn(int); // 隐藏了D1::fcn(int)
         int fcn();    // 重定义Base::fcn()，且是虚函数，根据基类指针指向的对象来决定动态调用D2还是Base中的版本
     };

**D1** 中的 fcn 版本没有重定义 Base 的虚函数 fcn，相反，**它屏蔽了基类的 fcn**（用fcn(int)这个函数屏蔽了）。结果 **D1 有两个名为 fcn 的函数**：类从 Base **继承了一个名为 fcn 的虚函数**，类又定义了**自己的名为 fcn的非虚成员函数**，该函数接受一个 int 形参。
**类 D2** 重定义了它继承的两个函数，它重定义了 Base 中定义的 fcn的原始版本并重定义了 D1 中定义的非虚版本。

#### **通过基类调用被屏蔽的虚函数**

     Base bobj;  D1 d1obj;  D2 d2obj; 
     Base *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj; 
     bp1->fcn();   // ok: virtual call, will call Base::fcnat run time 
     bp2->fcn();   // ok: virtual call, will call Base::fcnat run time 
     bp3->fcn();   // ok: virtual call, will call D2::fcnat run time

在 bp2 的情况，基本对象是 D1 类的，**D1 类没有重定义不接受实参的虚函数版本，因此通过 bp2 的函数调用（在运行时）调用 Base 中定义的版本。**

### 名字查找与继承
确定函数调用遵循以下四个步骤：
1.  首先确定进行函数调用的对象、引用或指针的静态类型。 
2.  在该类中查找函数，如果找不到，就在直接基类中查找，如此循着类的继承链往上找，直到找到该函数或者查找完最后一个类。如果不能在类或其相关基类中找到该名字，则调用是错误的。 
3.  一旦找到了该名字，就进行常规类型检查（第 7.1.2节），查看如果给定找到的定义，该函数调用是否合法。 
4.  假定函数调用合法，编译器就生成代码。**如果函数是虚函数且通过引用或指针调用，则编译器生成代码以确定根据对象的动态类型运行哪个函数版本**，否则，编译器生成代码直接调用函数。

## 纯虚函数
在函数形参表后面写上 = 0 以指定纯虚函数。

	 class Disc_item : public Item_base { 
     public: 
         double net_price(std::size_t) const = 0; 
     };

将函数定义为纯虚能够说明，**该函数为后代类型提供了可以覆盖的接口**，**但是这个类中的版本决不会调用**。
创建含有纯虚函数的类的对象时将发生编译时错误：

    Disc_item discounted; // error: can't define a Disc_item object 
     Bulk_item bulk;       // ok: Disc_item subobject within Bulk_item

**含有（或继承）一个或多个纯虚函数的类是抽象基类**。除了作为抽象基类的派生类的对象的组成部分，**不能创建抽象类型的对象**。 

## 容器与继承

如果定义 multiset 保存基类类型的对象： 

     multiset<Item_base> basket; 
     Item_base base; 
     Bulk_item bulk; 
     basket.insert(base);  // ok: add copy of base to basket 
     basket.insert(bulk);  // ok: but bulk sliced down to its b ase part 

加入派生类型的对象时，只将对象的基类部分保存在容器中。
若容器保存派生类类型对象，则不能插入基类对象。

## 句柄类与继承

使用指针或引用会加重类用户的负担。
C++ 中一个通用的技术是定义包装（cover）类或**句柄类**。句柄类**存储和管理基类指针**。指针所指对象的类型可以变化，它既可以指向基类类型对象又可以指向派生类型对象。**因此用户通过句柄类访问继承层次的操作**。
因为句柄类使用指针执行操作，虚成员的行为将在运行时根据句柄实际绑定的对象的类型而变化。因此，**句柄的用户可以获得动态行为但无须操心指针的管理**。
句柄有两个重要的设计考虑因素：
•   像对任何保存指针（第 13.5 节）的类一样，必须确定对复制控制做些什么。包装了继承层次的句柄通常表现得**像一个智能指针**（第 13.5.1 节）或者像一个值（第 13.5.2 节）。 
•   句柄类决定句柄接口屏蔽还是不屏蔽继承层次，如果不屏蔽继承层次，用户必须了解和使用基本层次中的对象。


