cmake_minimum_required(VERSION 2.8.4)
project(test_variable)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# 编译出可执行文件
add_executable(global_a global_a.c)
add_executable(global_b global_b.c)
add_executable(local_a local_a.c)
add_executable(local_static local_static.c)

