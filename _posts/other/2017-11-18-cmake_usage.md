---
layout: article
title:  "cmake用法"
categories: other
excerpt: "讲述跨平台构建工具cmake的基本使用"
---


> 文章欢迎转载，但转载时请保留本段文字，并置于文章的顶部 作者：c0ks 本文原文地址：http://yyff.github.io/other/cmake_usage/

{% include toc.html %}

# cmake用法


## 0x1 介绍
cmake（cross platform make）是个开源的构建系统，它通过配置文件（CMakeLists.txt）控制构建过程（包含一组内置的命令）。cmake不能直接产出最终软件，其产出的是一个标准的构建文件（如Unix的Makefile或Windows Visual C++的projects/workspaces）[^1]。CMake由一组高层逻辑目标（target, 可使用`make target`执行构建）组成，每个目标都对应于一个二进制或库，目标间的依赖决定了构建次序以及目标改变后的重生成规则[^2]。

## 0x2 常用命令行选项

* `-D<var>:<type>=<value>`: 设置变量及它的值
	* 如果变量在CMakeLists.txt文件中存在，则CMakeLists.txt中的优先级更高）
	* 变量类型为String时可省略
	* 例子: `-DCMAKE_BUILD_TYPE:STRING=Debug`设置构建类型`CMAKE_BUILD_TYPE`为Debug，MinGW Generator默认生成`CMAKE_BUILD_TYPE`为空，即release；NMake Generator默认生成`CMAKE_BUILD_TYPE`为Debug。
* `-G <generator-name>`: 指定makefile生成器
	* 常见的：`MinGW Makefiles`, `Unix Makefiles`...
	* 更多参考[^3]中的Generators小节


## 0x3 常见内置变量(来自[^4])

```cmake
# 编译目录路径，下面三个变量均是同样的值
CMAKE_BINARY_DIR
PROJECT_BINARY_DIR
<projectname>_BINARY_DIR
# 工程顶层路径，三个均是同样的值
CMAKE_SOURCE_DIR
PROJECT_SOURCE_DIR
<projectname>_SOURCE_DIR
# 当前CMakeLists.txt的路径，如果子目录下有CMakeLists.txt，那么该值与CMAKE_SOURCE_DIR不一样
CMAKE_CURRENT_SOURCE_DIR
# 当前CMakeLists.txt的目标文件输出路径，意义同上
CMAKE_CURRRENT_BINARY_DIR
ADD_SUBDIRECTORY(src "更改为子目录新的目标文件输出路径")
# 设置目标文件的存放路径
EXECUTABLE_OUTPUT_PATH
# 设置库文件的存放路径
LIBRARY_OUTPUT_PATH
# 设置工程名
PROJECT_NAME
# 主版本号，比如2.4.6中的2，通过SET命令来设置
CMAKE_MAJOR_VERSION，CMAKE
# 次版本号，比如2.4.6中的4
CMAKE_MINOR_VERSION，CMAKE
# 补丁等级，比如2.4.6 中的6
CMAKE_PATCH_VERSION，CMAKE
# 系统名称，比如Linux-2.6.22
CMAKE_SYSTEM
# 不包含版本的系统名，比如Linux
CMAKE_SYSTEM_NAME
# 系统版本，比如2.6.22
CMAKE_SYSTEM_VERSION
# 处理器名称，比如i686
CMAKE_SYSTEM_PROCESSOR
# 在所有的类UNIX平台为TRUE，包括OS X和cygwin
UNIX
# 在所有的win32平台为TRUE，包括cygwin
WIN32
# 动/静态库生成全局控制
BUILD_SHARED_LIBS
# C编译器选项(ADD_DEFINITIONS也可以添加选项)
CMAKE_C_FLAGS/CMAKE_C_FLAGS_DEBUG/CMAKE_C_FLAGS_RELEASE
SET(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# C++编译器选项
CMAKE_CXX_FLAGS/CMAKE_CXX_FLAGS_DEBUG/CMAKE_CXX_FLAGS_RELEASE
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -lpthread -lm")
```

### 构建类型变量`CMAKE_BUILD_TYPE`(来自[^5])

`CMAKE_BUILD_TYPE` | 对应的c编译选项变量 | 对应的c++编译选项变量
--- | --- | ---
None |	`CMAKE_C_FLAGS` | `CMAKE_CXX_FLAGS`
Debug |	`CMAKE_C_FLAGS_DEBUG`	| `CMAKE_CXX_FLAGS_DEBUG`
Release | `CMAKE_C_FLAGS_RELEASE` |	 `CMAKE_CXX_FLAGS_RELEASE`
RelWithDebInfo |	`CMAKE_C_FLAGS_RELWITHDEBINFO` | `CMAKE_CXX_FLAGS_RELWITHDEBINFO`
MinSizeRel | `CMAKE_C_FLAGS_MINSIZEREL` | 	`CMAKE_CXX_FLAGS_MINSIZEREL`


## 0x4 常用命令表

项目命令 | 简介
--- | ---
`add_custom_command` | 自定义命令
`add_custom_target` | 自定义target
`add_definitions` | 定义所有target的宏
`target_compile_definitions` | 定义单个target的宏
`add_dependencies` | 定义target依赖
`add_executable` | 可执行target
`add_library` | 添加库
`add_subdirectory` | 添加子工程目录
`aux_source_directory` | 获取单个目录下的源文件，也可用file(GLOB ...)
`include_directories` | 头文件目录
`link_directories` | 库的查找目录（相对路径'.'是cmake执行目录，可使用`find_library`或`find_path`代替）
`link_libraries` | 所有目标的链接库
`project` | 设置项目名、版本和允许的语言
`set_target_properties` | 设置[target属性](https://cmake.org/cmake/help/v3.10/manual/cmake-properties.7.html#target-properties)，比如：输出名（默认和target同名）、编译选项、链接选项
`add_compile_options(<option> ...)` | 添加编译选项（包括宏、编译选项和链接选项，如：-std=c++11、-lpthread）
`target_compile_options` | 定义target的编译选项（target属性中的COMPILE_OPTIONS）：
`target_include_directories` | 定义target的头文件目录(target属性中的INCLUDE_DIRECTORIES)
`target_link_libraries` | 定义target的链接库



脚本命令 | 简介
--- | ---
`set` | 设置变量
`file( WRITE|READ|APPEND|GLOB| ...)` | 文件操作 
`list` | 列表操作
`message` | 打印消息
`string` | 字符串操作：正则、时间戳
`find_file` | 查找指定文件
`find_library` | 查找库
`find_package` | 查找库包
`find_path` | 查找包含指定文件的路径
`foreach``while` | 循环控制语句
`if` | 条件语句


## 0x5 常用命令详解


### 5.1 项目命令


#### 1.(1)有输出的自定义命令

```cmake
add_custom_command(OUTPUT output1 [output2 ...]
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [DEPENDS [depends...]])
```
   
#### 实例 (来自[5])

```
# 编译proto文件（使用`make proto`）
set(PROTO_IN  news.proto)
set(PROTO_SRC news.pb.cc)
set(PROTO_OUT news.pb.h news.pb.cc proto/)
add_custom_command(
    OUTPUT ${PROTO_OUT}
    COMMAND protoc --cpp_out . --java_out . ${PROTO_IN}
    DEPENDS ${PROTO_IN}
)
add_custom_target(proto DEPENDS ${PROTO_OUT}) # 定义proto目标
```

                   
#### 1.(2)无输出的自定义命令

```
add_custom_command(TARGET <target>
                   PRE_BUILD | PRE_LINK | POST_BUILD
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...])                
```

#### 实例

```
# 将mymath静态库和头文件目录include拷贝到output
# 命令中的相对路径是从cmake命令的执行路径开始的(所以'..' = ${PROJECT_SOURCE_DIR})
add_library(mymath STATIC ${SRC_LIST})
add_custom_command(TARGET mymath
                   POST_BUILD
                   COMMAND mkdir -p ../output/lib/ 
                   COMMAND cp -r ../include ../*.a ../output/lib/)
```

#### 2.自定义target

```
add_custom_target(Name [ALL] [command1 [args1...]]
                  [COMMAND command2 [args2...] ...]
                  [DEPENDS depend depend depend ... ])
                  

# ALL: 该目标被添加都默认目标中，每次构建都会执行；
# DEPENDS: 依赖的在add_custom_command中定义的OUTPUT
```


#### 3.宏定义
```
# 定义所有target的宏
add_definitions(-DFOO -DBAR=XX ...)

# 定义单个target的宏
target_compile_definitions(<target>
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE| PUBLIC |PRIVATE> [items2...] ...])

# INTERFACE: 只定义该target依赖target的宏； 
# PRIVATE: 只定义该target的宏
# PUBLIC: 同时定义target及其依赖target的宏 
```

#### 实例
```
# 定义宏_GNU_SOURCE，__STDC_LIMIT_MACROS，VERSION
STRING(TIMESTAMP VERSION "%Y-%m-%d %H:%M")
add_definitions(-D_GNU_SOURCE -D__STDC_LIMIT_MACROS -DVERSION=${VERSION})
```


#### 4.定义target依赖
```
add_dependencies(<target> [<target-dependency>]...)
```

#### 实例 (来自[5])
```
add_dependencies(demo proto)
```


#### 5. 定义可执行target
```
add_executable(<name>
               [EXCLUDE_FROM_ALL]
               source1 [source2 ...])
# EXCLUDE_FROM_ALL: 从默认target中移除，即make不执行该target            
```

#### 6. 定义库target
```
add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            source1 [source2 ...])            
```

#### 7. 添加子工程目录（该目录下也有CMakeList.txt）
```
add_subdirectory(source_dir [binary_dir]
                 [EXCLUDE_FROM_ALL])
# [binary_dir]: 子目录工程的输出目录(${PROJECT_SOURCE_DIR})                      
```


#### 实例 (来自[5])
```
# 构建单测
add_subdirectory(unittest)
```


#### 8. 获取目录下的源文件
```
（1）
aux_source_directory(<dir> <variable>)

（2）
file(GLOB_RECURSE <variable> [<globbing-expressions>...])               
```

#### 实例
```
aux_source_directory(./src SRC_LIST) # 只支持一个目录

file(GLOB SRC_LIST ./src/*.cpp ./src/base/*.cpp) # 支持多个目录
```

#### 9. 头文件目录
```
# 所有目标的头文件目录
include_directories([AFTER|BEFORE] dir1 [dir2 ...])
  
#BEFORE（默认）:  将当前头文件目录添加到默认头文件目录列表前，即该头文件中会被优先包含
#AFTER: 将当前头文件目录添加到默认头文件目录列表前

# 单个目标的头文件目录
target_include_directories(<target> [SYSTEM] [BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

#### 实例
```
include_directories(./include ./lib/include)
```

#### 10. 库的查找目录
```
link_directories(dir1 dir2 ...)
```

#### 实例
```
link_directories(./lib)
```

#### 11. 链接库
```
# 所有target的链接库(item可以是库文件名libpthread.a，库名pthread或库的完整路径名)
link_libraries([item1 [item2 [...]]]
               [[debug|optimized|general] <item>] ...)
               
# 单个target的链接库
target_link_libraries(<target> ... <item>... ...)            
```

#### 实例
```
link_libraries(mymath pthread)

# target链接
target_link_libraries(demo pthread) 
```

### 5.1 脚本命令


*  `set(<variable> <value>)`: 设置变量
	*  取变量值用`${variable}`）
* `set(<variable> <value1> ... <valueN>)`: 设置有多个值的变量。
	* 最终的值时用`;`分隔的字符串，即：`variable=“<value1>;<value2>...”`



## 0x6 示例


### 项目目录
```
cmake-test
├── CMakeLists.txt
├── build
├── include
│   └── func.h
├── lib
│   ├── include
│   │   └── mymath.h
│   ├── libmymath.a
└── src
    ├── func.cpp
    └── main.cpp

```
> 解释：main.cpp依赖func.h/cpp, func.cpp依赖mymath.h/cpp

该测试代码可访问：![cmake-test]({{ site.url }}/codes/cmake-test)


### CMakeLists.txt

```cmake
# 版本限定
cmake_minimum_required(VERSION 2.8)

# 项目名称
project(cmake-demo)

# 预处理参数
string(TIMESTAMP VERSION "%Y-%m-%d %H:%M")
add_definitions(-D_GNU_SOURCE -D__STDC_LIMIT_MACROS -DVERSION=${VERSION})

# 编译器选项
set(CMAKE_C_FLAGS "-g -pipe -W -Wall -fPIC -O2")
set(CMAKE_CXX_FLAGS "-g -pipe -W -Wall -fPIC -O2")

# 依赖库路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
# 依赖库
link_libraries(mymath)

# 头文件目录
include_directories(./include ./lib/include)

# 源文件
file(GLOB SRC_LIST ./src/*.cpp)

# 设置可执行程序输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# 动态库
#add_library(test SHARED ${SRC_LIST})
# 静态库
#add_library(cmake-demo-lib STATIC ${SRC_LIST})
# 可执行程序
add_executable(cmkae-demo ${SRC_LIST})
```



## 0x7 参考
[^1] [CMake Wiki](https://zh.wikipedia.org/wiki/CMake)

[^2] [cmake-buildsystem(7)](https://cmake.org/cmake/help/v3.10/manual/cmake-buildsystem.7.html#introduction)

[^3] [cmake Command-Line Options](https://cmake.org/cmake/help/cmake-2.4.html)

[^4] [cmake常用工程示例大集合](http://blog.csdn.net/freeape/article/details/52567087)

[^5] [CMake使用总结](https://www.mawenbao.com/note/cmake.html)

[^6] [cmake-commands](https://cmake.org/cmake/help/v3.0/manual/cmake-commands.7.html)

