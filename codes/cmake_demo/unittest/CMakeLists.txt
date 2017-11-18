# 版本限定 
cmake_minimum_required(VERSION 2.8)

# 项目名称 
project(cmake-demo-ut) 

# 预处理参数
string(TIMESTAMP VERSION "%Y-%m-%d %H:%M")
add_definitions(-D_GNU_SOURCE -D__STDC_LIMIT_MACROS -DVERSION=${VERSION})

# 编译器选项
set(CMAKE_C_FLAGS "-g -pipe -W -Wall -fPIC -O2") 
set(CMAKE_CXX_FLAGS "-g -pipe -W -Wall -fPIC -O2") 

# 依赖库路径
cmake_policy(SET CMP0015 NEW)
link_directories(../lib ./)
# 依赖库
link_libraries(pthread mymath gtest gtest_main)

# 头文件目录
include_directories(../include ../lib/include)

# 源文件
file(GLOB SRC_LIST ../src/*.cpp)
# 删除源文件中的main.cpp
foreach(src ${SRC_LIST})
    string(FIND ${src} main.cpp out) # 未找到返回-1
    if (NOT ${out} EQUAL -1)
        list(REMOVE_ITEM SRC_LIST ${src})
    endif()
endforeach()

# 设置可执行程序输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# 动态库
#add_library(cmake_demo_lib SHARED ${SRC_LIST}) 
# 静态库
add_library(cmake_demo_lib STATIC ${SRC_LIST})
set_target_properties(cmake_demo_lib PROPERTIES OUTPUT_NAME "cmake_demo") 
# 可执行程序
#add_executable(cmkae_demo ${SRC_LIST})

# unittest
file(GLOB UT_SRC_LIST ./*.cpp)
foreach(ut ${UT_SRC_LIST})
    get_filename_component(file_name ${ut} NAME_WE)
    add_executable(${file_name} ${ut})
    add_dependencies(${file_name} cmake_demo_lib)
    target_link_libraries(${file_name} cmake_demo_lib)
endforeach()
