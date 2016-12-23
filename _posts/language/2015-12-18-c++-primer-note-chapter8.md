---
layout: article
title:  "c++ primer读书笔记--第八章 标准IO库"
categories: language
---

> 本系列文章是关于我的c++ primer读书笔记

{% include toc.html %}

前面的程序已经使用了多种 IO 标准库提供的工具： 
•   istream（输入流）类型，提供输入操作。 
•   ostream（输出流）类型，提供输出操作。 
•   cin（发音为 see-in）：读入标准输入的 istream 对象。 
•   cout（发音为 see-out）：写到标准输出的 ostream 对象。 
•   cerr（发音为 see-err）：输出标准错误的 ostream 对象。cerr常用于程序错误信息。 
•   **>>** 操作符，用于**从 istream 对象中读入输入**。 
•   **<<** 操作符，用于**把输出写到 ostream 对象中**。 
•   getline 函数，需要分别取 istream 类型和 string类型的两个引用形参，其功能是从 istream对象读取一个单词，然后写入 string 对象中。

## 面向对象的标准库

**同时支持或使用不同类型设备以及不同大小的字符流**，其复杂程度似乎相当可怕。为了管理这样的复杂性，标准库使用了继承（inheritance）来定义一组面向对象（object-oriented）类。**如果两种类型存在继承关系，则可以说一个类“继承”了其父类的行为——接口**。C++中所提及的父类称为基类（base class），而继承而来的类则称为派生类（derived class）。
IO 类型在三个独立的头文件中定义：**iostream定义读写控制窗口的类型**，
**fstream 定义读写已命名文件的类型**，而 **sstream所定义的类型则用于读写存储在内存中的 string 对象**。在fstream 和 sstream 里定义的每种类型都是从 iostream头文件中定义的相关类型派生而来。

**简单的  iostream  继承层次:**

![这里写图片描述](http://img.blog.csdn.net/20150705183215728)



**对 IO 类型使用继承还有另外一个重要的含义：如果函数有基类类型的引用形参时，可以给函数传递其派生类型的对象。这就意味着：对istream& 进行操作的函数，也可使用 ifstream 或者 istringstream对象来调用**。类似地，形参为 ostream&类型的函数也可用 ofstream 或者 ostringstream 对象调用。因为 IO类型通过继承关联，所以可以只编写一个函数，而将它应用到三种类型的流上：控制台、磁盘文件或者字符串流（string streams）。

### IO  对象不可复制或赋值


        ofstream out1, out2; 
        out1 = out2;   // error: cannot assign stream objects

这个要求有两层特别重要的含义。正如在第九章看到的，只有支持复制的元素类型可以存储在 vector或其他容器类型里。由于**流对象不能复制，因此不能存储在vector（或其他）容器中**（即不存在存储流对象的 vector 或其他容器）。 第二个含义是：**形参或返回类型也不能为流类型。**如果需要传递或返回IO 对象，则必须传递或返回指向该对象的指针或引用： 

## 条件状态

实现 IO 的继承正是错误发生的根源。一些错误是可恢复的；一些错误则发生在系统底层，位于程序可修正的范围之外。IO标准库管理一系列条件状态（condition state）成员，用来标记给定的 IO对象是否处于可用状态，或者碰到了哪种特定的错误。

![这里写图片描述](http://img.blog.csdn.net/20150705182847150)




  
考虑下面 IO 错误的例子：

    int ival; 
         cin >> ival;

 
 
如果在标准输入设备输入 Borges，则 cin 在尝试将输入的字符串读为 int 型数据失败后，会生成一个错误状态。类似地，如果输入文件结束符（end-of-file），cin也会进入错误状态。而如果输入1024，则成功读取，cin将处于正确的无错误状态。流必须处于无错误状态，才能用于输入或输出。检测流是否用的最简单的方法是检查其真值： 
      

    if (cin) 
               // ok to use cin, it is in a valid state

所有流对象都包含一个条件状态成员，该成员由 setstate 和 clear操作
管理。这个状态成员为 iostate 类型，这是由各个 iostream类分别定义的机器相关的整型。该状态成员以二进制位（bit）的形式使用。
每个 IO 类还定义了三个 iostate类型的常量值，分别表示特定的位模式。这些常量值用于指出特定类型的 IO 条件，可与位操作符（第 5.3 节）一起使用，以便在一次操作中检查或设置多个标志。
**badbit 标志着系统级的故障**，如无法恢复的读写错误。如果出现了这类错误，则**该流通常就不能再继续使用了**。如果出现的是可恢复的错误，如在希望获得数值型数据时输入了字符，此时则设置 failbit标志，这种导致**设置 failbit 的问题通常是可以修正的**。**eofbit 是在遇到文件结束符时设置的，此时同时还设置了 failbit**。
流的状态由 bad、fail、eof 和 good 操作提示。如果 bad、fail 或者 eof 的任意一个为 true，则检查流本身将显示该流处于错误状态。类似地，**如果这三个条件没有一个为 true，则 good 操作将返回 true。**

**clear 和 setstate 操作用于改变条件成员的状态**。
clear操作将条件重设为有效状态。
使用 setstate 操作可打开某个指定的条件，用于表示某个问题的发生。

### 流状态的查询和控制

可以如下管理输入操作 
 

       int ival; 
        // read cin and test only for EOF; loop is executed even if  there are 
    other IO failures 
        while (cin >> ival, !cin.eof()) { 
            if (cin.bad())         // input stream is corrupted; bail out 
                throw runtime_error("IO stream corrupted"); 
            if (cin.fail()) {                        // bad input 
                cerr<< "bad data, try again";        // warn the user 
                cin.clear(istream::failbit);         // reset the stream 
                continue;                            // get next input 
            } 
            // ok to process ival 
        } 

这个循环不断读入 cin，直到到达文件结束符或者发生不可恢复的读取错误为止。**循环条件使用了逗号操作符,所以首先计算它的每一个操作数，然后返回最右边操作数作为整个操作的结果。因此，循环条件只读入 cin 而忽略了其结果。该条件的结果是 !cin.eof() 的值:即没到文件结尾都进入循环.**
在循环中，首先检查流是否已破坏。如果是的放，抛出异常并退出循环。如果输入无效，则输出警告并清除 failbit 状态。

### 条件状态的访问

**rdstate 成员函数返回一个 iostate类型值，该值对应于流当前的整个条件状态**： 
  

       // remember current state of cin 
         istream::iostate old_state = cin.rdstate(); 
         cin.clear(); 
         process_input();  // use cin 
         cin.clear(old_state); // now reset cin to old state

### 多种状态的处理 
 
可以通过多次调用 setstate 或者 clear函数实现。另外一种方法则是使用按位或（OR）操作符（第 5.3节）在一次调用中生成“传递两个或更多状态位”的值。按位或操作使用其操作数的二进制位模式产生一个整型数值。

    // sets both the badbit and the failbit 
       is.setstate(ifstream::badbit | ifstream::failbit); 

实参： is.badbit | is.failbit 
 
生成了一个值，其对应于 badbit 和 failbit的位都打开了，**也就是将这
两个位都设置为 1**，该值的其他位则都为 0。

## 输出缓冲区的管理

每个 IO 对象管理一个缓冲区，用于存储程序读写的数据。如有下面语句： 

    os << "please enter a value: ";

系统将字符串字面值存储在与流 os关联的缓冲区中。下面几种情况将导致**缓冲区的内容被刷新，即写入到真实的输出设备或者文件**：
1.  **程序正常结束**。作为 main返回工作的一部分，将清空所有输出缓冲区。 
2.  在一些不确定的时候，**缓冲区可能已经满了**，在这种情况下，缓冲区将会在写下一个值之前刷新。 
3.  用**操纵符（第 1.2.2 节）显式地刷新缓冲区**，例如行结束符 endl。
4.  在每次输出操作执行完后，**用unitbuf操作符设置流的内部状态**，从而清空缓冲区。 
5.  **可将输出流与输入流关联（tie）起来。在这种情况下，在读输入流时将刷新其关联的输出缓冲区。**

### 输出缓冲区的刷新 

第一个经常使用的 **flush**，用于刷新流，但**不在输出中添加任何字符**。第二个则是比较少用的 **ends**，这个操纵符**在缓冲区中插入空字符 null**，然后后刷新它;第三**个endl,插入换行符\n后刷新**:

    cout << "hi!" << flush;      // flushes the buffer; adds no data 
        cout << "hi!" << ends;       // inserts a null, then flushes the buffer 
        cout << "hi!" << endl;       // inserts a newline, then flushes shes the buffer 

### unitbuf   操纵符

如果需要**刷新所有输出，最好使用 unitbuf操纵符**。这个操纵符在每次执
行完写操作后都刷新流：

    cout << unitbuf << "first" << " second" << nounitbuf;

等价于： 
   

     cout << "first" << flush << " second" << flush;

nounitbuf 操纵符将流恢复为使用正常的、由系统管理的缓冲区刷新方式。

### 如果程序崩溃了，则不会刷新缓冲区

如果需要使用最后的输出给程序错误定位，则必须确定所有要输出的都已经输出。为了确保用户看到程序实际上处理的所有输出，**最好的方法是保证所有的输出操作都显式地调用了 flush 或endl**。如果仅因为缓冲区没有刷新，程序员将浪费大量的时间跟踪调试并没有执行的代码。

### 将输入和输出绑在一起

当输入流与输出流绑在一起时，**任何读输入流的尝试都将首先刷新其输出流关联的缓冲区**。标准库将 cout 与 cin 绑在一起，因此语句： 

         cin >> ival; 
 
导致 cout 关联的缓冲区被刷新。

> 交互式系统通常应确保它们的输入和输出流是绑在一起的。这样做意味着可以保证任何输出，包括给用户的提示，都在试图读之前输出。

tie 函数可用 istream 或 ostream 对象调用，使用一个指向 ostream 对象的指针形参。调用 tie函数时，将实参流绑在调用该函数的对象上。如果一个流调用 tie函数将其本身绑在传递给 tie 的 ostream 实参对象上，则该流上的任何 IO 操作都会刷新实参所关联的缓冲区。

     cin.tie(&cout);   // illustration only: the library ties cin and cout for us 
        ostream *old_tie = cin.tie(); 
        cin.tie(0); // break tie to cout, cout no longer flushed when cin is read 
        cin.tie(&cerr);   // ties cin and cerr, not necessarily a good idea! 
        // ... 
        cin.tie(0);       // break tie between cin and cerr 
        cin.tie(old_tie); // restablish normal tie between cin and cout

一个 ostream 对象每次只能与一个 istream对象绑在一起。**如果在调用 tie 函数时传递实参 0，则打破该流上已存在的捆绑**。

## 文件的输入和输出 

fstream 头文件定义了三种支持文件 IO 的类型： 
1.  ifstream，由 istream 派生而来，提供读文件的功能。 
2.  ofstream，由 ostream 派生而来，提供写文件的功能。 
3.  fstream，由 iostream 派生而来，提供读写同一个文件的功能。
fstream 类型除了继承下来的行为外，还定义了两个自己的新操作—— open 和 close，以及形参为要打开的文件名的构造函数。fstream、ifstream 或 ofstream 对象可调用这些操作，而其他的 IO 类型则不能调用。 

### 文件流对象的使用

我们的程序已经使用过标准库定义的对象：cin、cout 和cerr,需要**读写文件时，则必须定义自己的对象，并将它们绑定在需要的文件上**。假设 ifile 和 ofile 是存储希望读写的文件名的 strings 对象，可如下编写代码：

    // construct an ifstream and bind it to the file named ifile 
    ifstream infile(ifile.c_str()); 
        // ofstream output file object to write file named ofile 
        ofstream outfile(ofile.c_str()); 

上述代码定义并打开了一对 fstream 对象。infile 是读的流，而 outfile 则是写的流。**为 ifstream 或者 ofstream对象提供文件名作为初始化式，就相当于打开了特定的文件。**
还可以用另一种形式:

    ifstream infile;    // unbound input file stream 
        ofstream outfile;   // unbound output file stream 

 
上述语句将 **infile 定义为读文件的流对象，将 outfile定义为写文件的对象**。这两个对象都没有捆绑具体的文件。在使用 fstream 对象之前，还必须使这些对象捆绑要读写的文件： 

        infile.open("in");   // open file named "in" in the current  directory 
        outfile.open("out"); // open file named "out" in the current 
    directory

**调用 open 成员函数将已存在的 fstream 对象与特定文件绑定。**为了实现读写，需要将指定的文件打开并定位，open 函数完成系统指定的所有需要的操作。

> 警告：C++ 中的文件名 由于历史原因，IO 标准库使用 C 风格字符串（第 4.3 节）而不是 C++ strings
> 类型的字符串作为文件名。在创建 fstream 对象时，**如果调用 open 或使用文件名作初始化式，需要传递的实参应为 C
> 风格字符串，而不是标准库 strings对象**。程序常常从标准输入获得文件名。通常，比较好的方法是将文件名读入 string 对象，而不是
> C 风格字符数组。**假设要使用的文件名保存在 string 对象中，则可调用 c_str 成员（第 4.3.2 节）获取 C
> 风格字符串。**

### 检查文件打开是否成功

    // check that the open succeeded 
        if (!infile) { 
            cerr << "error: unable to open input file: " 
                 << ifile << endl; 
            return -1; 
        }

检查流等效于检查对象是否“适合”输入或输出。如果打开（open）失败，则说明 fstream 对象还没有为 IO 做好准备。

    if (outfile) // ok to use outfile? 

 返回 true 意味着文件已经可以使用。由于希望知道文件是否未准备好，则对返回值取反来检查流： 

    if (!outfile) // not ok to use outfile? 

### 将文件流与新文件重新捆绑

    ifstream infile("in");      // opens file named "in" for reading 
         infile.close();             // closes "in" 
         infile.open("next");        // opens file named "next" for  reading

### 清除文件流的状态

如果打算**重用已存在的流对象，那么 while循环必须在每次循环进记得关闭（close）和清空（clear）文件流：** 

    ifstream input; 
        vector<string>::const_iterator it = files.begin(); 
        //   for each file in the vector 
        while (it != files.end()) { 
            input.open(it->c_str());  // open the file 
            // if the file is ok, read and "process" the input 
            if (!input) 
                break;                    // error: bail out! 
            while(input >> s) // do the work on this file 
                process(s); 
            input.close();        // close file when we're done with it 
            input.clear();        // reset state to ok 
            ++it;                 // increment iterator to get next file 
        } 

如果忽略 clear 的调用，则循环只能读入第一个文件。要了解其原因，就需要考虑在循环中发生了什么：首先打开指定的文件。**假设打开成功，则读取文件直到文件结束或者出现其他错误条件为止。在这个点上，input 处于错误状态**。**如果在关闭（close）该流后没有调用 clear 清除流的状态，接着在 input 上做的任何输入运算都会失败**。一旦关闭该文件，再打开 下一个文件时，在内层 while 循环上读 input仍然会失败——**毕竟最后一次对流的读操作到达了文件结束符**，事实上该文件结束符对应的是另一个与本文件无关的其他文件。

> 如果程序员需要重用文件流读写多个文件，必须在读另一个文件之前调用 clear 清除该流的状态。

### 文件模式

在打开文件时，无论是调用 open还是以文件名作为流初始化的一部分，都需指定文件模式（file mode）。

![这里写图片描述](http://img.blog.csdn.net/20150705182857793)


默认时，与 ifstream 流对象关联的文件将以 in模式打开，该模式允许文件做读的操作：与 ofstream关联的文件则以 out 模式打开，使文件可写。**以 out 模式打开的文件会被清空：丢弃该文件存储的所有数据。**
从效果来看，为 ofstream 对象指定 out 模式等效于同时指定了 out 和 trunc 模式。

    //  output mode by default; truncates file named "file1" 
        ofstream outfile("file1"); 
        // equivalent effect: "file1" is explicitly truncated 
        ofstream outfile2("file1", ofstream::out | ofstream::trunc); 

对于用 ofstream 打开的文件,要保存文件中存在的数据，唯一方法是显式地指定 app 模式打开：

     //  append mode; adds new data at end of existing file named "file2" 
        ofstream appfile("file2", ofstream::app); 

### 对同一个文件作输入和输出运算
 
**默认情况下，fstream 对象以 in 和 out 模式同时打开。当文件同时以 in 和 out 打开时不清空**。也可以显示指定:

    // open for input and output 
        fstream inOut("copyOut", fstream::in | fstream::out); 

### 模式是文件的属性而不是流的属性

只要调用 open 函数，就要设置文件模式，其模式的设置可以是显式的也可以是隐式的。如果没有指定文件模式，将使用默认值。

### 打开模式的有效组合

有些模式组合是没有意义的，例如:同时以 in 和 trunc 模式打开文件，准备读取所生成的流，但却因为 trunc 操作而导致无数据可读。
![!\[enter description here\]\[4\]](http://img.blog.csdn.net/20150705183024228)


上述所有的打开模式组合还可以添加 ate 模式。即在第一次读或写之前，将文件定位于文件末尾处。

### 一个打开并检查输入文件的程序 

本书有好几个程序都要**打开给定文件用输入**,我们编写一个名为 open_file 的函数实现这个功能。这个函数有两个引用形参，分别是 ifstream 和 string 类型，其中 string 类型的引用形参存储与指定 ifstream 对象关联的文件名： 

    // opens in binding it to the given file 
        ifstream& open_file(ifstream &in, const string &file) 
        { 
            in.close();     // close in case it was already open 
            in.clear();     // clear any existing errors 
            // if the open fails, the stream will be in an invalid state 
            in.open(file.c_str()); // open the file we were given 
            return in; // condition state is good if open succeeded 
        } 

于不清楚流 in 的当前状态，因此首先调用 close 和 clear 将这个流设置为有效状态。然后尝试打开给定的文件。如果打开失败，流的条件状态将标志这个流是不可用的。**最后返回流对象 in，此时，in 要么已经与指定文件绑定起来了，要么处于错误条件状态。**     

### 字符串流

**iostream 标准库支持内存中的输入／输出，只要将流与存储在程序内存中的 string 对象捆绑起来即可。**此时，可使用 iostream 输入和输出操作符读写这个 string 对象。标准库定义了三种类型的字符串流：
•   **istringstream**，由 istream 派生而来，提供读 string 的功能。 
•   **ostringstream**，由 ostream 派生而来，提供写 string 的功能。 
•   **stringstream**，由 iostream 派生而来，提供读写 string的功能。
要使用上述类，必须包含 sstream 头文件。
与 fstream 类型一样，上述类型由 iostream 类型派生而来.
sstream 类型除了继承的操作外，还各自定义了一个有 string 形参的构造函数，这个构造函数将 string 类型的实参复制给 stringstream 对象。**对 stringstream 的读写操作实际上读写的就是该对象中的 string 对象**。这些类还定义了名为 str 的成员，用来读取或设置 stringstream 对象所操纵的 string 值。 

> 注意到尽管 fstream 和 sstream共享相同的基类，但它们没有其他相互关系。特别是，stringstream 对象不使用 open和 close 函数，而 fstream 对象则不允许使用 str。


![这里写图片描述](http://img.blog.csdn.net/20150705183051683)




### stringstream  对象的和使用

    string line, word;      // will hold a line and word from input, 
    respectively 
        while (getline(cin, line))   {            // read a line from the 
    input into line 
           // do per-line processing 
           istringstream stream(line);            // bind to stream to the line we read 
           while (stream >> word){          // read a word from line 
               // do per-word processing 
           } 
        }

将一个 istringstream对象与所读取的行绑定起来，之后stream>>word 将从line中读单词到word中.

## stringstream  提供的转换和／或格式化

stringstream 对象的一个常见用法是，需要在多种数据类型之间实现自动格式化时使用该类类型。

    int val1 = 512, val2 = 1024; 
        ostringstream format_message; 
        // ok: converts values to a string representation 
        format_message << "val1: " << val1 << "\n" 
                       << "val2: " << val2 << "\n";

重点在于 int 型值自动转换为等价的可打印的字符串。format_message 的内容是以下字符：
val1: 512\nval2: 1024

相反，用 istringstream 读 string对象，即可重新将数值型数据找回来。读取 istringstream对象自动地将数值型数据的字符表示方式转换为相应的算
术值。

    // str member obtains the string associated with a stringstream 
       istringstream input_istring(format_message.str()); 
       string dump; // place to dump the labels from the formatted message 
       // extracts the stored ascii values, converting back to arithmetic 
    types 
       input_istring >> dump >> val1 >> dump >> val2; 
       cout << val1 << " " << val2 << endl;  // prints 512 1024

str 成员获取与之前创建的 ostringstream 对象关联的 string 副本。再将 input_istring 与 string 绑定起来。在读 input_istring 时，相应的值恢复为它们原来的数值型表示形式。 

> 为了读取 input_string，必须把该 string
> 对象分解为若干个部分。我们要的是数值型数据；为了得到它们，必须读取（和忽略）处于所需数据周围的标号（用一个string对象dump来过滤）。

## 小结

C++ 使用标准库类处理输入和输出： 
•   iostream 类处理面向流的输入和输出。 
•   fstream 类处理已命名文件的 IO。 
•   stringstream 类处理内存中字符串的 IO。

所有 IO 对象都有一组条件状态，用来指示是否可以通过该对象进行 IO 操作。如果出现了错误（例如遇到文件结束符）对象的状态将标志无法再进行输入，直到修正了错误为止。标准库提供了一组函数设置和检查这些状态。

