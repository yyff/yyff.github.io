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


