cmake_minimum_required(VERSION 3.21.1)
project(CStudy_Server)

#  头文件搜索路径 g++ -i
include_directories(./include)

# 库文件搜索路径 g++ -L [不建议使用]
# link_libraries(/usr/lib)

# 生成库文件
# add_library(libname [SHARED|STATIC|MODULE][EXCLUDE_FROM_ALL] source1 source2)

# 添加编译参数
add_compile_options(-Wall -std=c++14 -O2)

# 发现一个目录下的所有源代码并将列表存储在一个变量中
aux_source_directory(src CPP_FILES)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${CPP_FILES})

# 为 target 添加共享库 g++ -l
target_link_libraries(${PROJECT_NAME} pthread odb odb-mysql)

# 向当前工程添加存放源文件的子目录，并且可以指定中间二进制和目标二进制存放的位置
# add_subdirectory(source_dir [binary_dir][EXCLUDE_FROM_ALL])
# add_subdirectory(src) # 添加src子目录，src需中有一个CMakeLists.txt

##### 常用变量

# 在CMAKE_CXX_FLAGS编译选项后追加-std=c++14
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# 编译类型（Debug，Release）
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_COMPILER g++) # 指定C++编译器
set(EXECUTABLE_OUTPUT_PATH ./) # 可执行文件输出的存放路径
set(LIBRARY_OUTPUT_PATH ./lib) # 库文件输出的存放路径