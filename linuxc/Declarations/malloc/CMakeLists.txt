cmake_minimum_required(VERSION 2.8.4)
project(test_malloc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SOURCE_FILES foo.c)

# 编译出可执行文件
add_executable(test_malloc ${SOURCE_FILES})

