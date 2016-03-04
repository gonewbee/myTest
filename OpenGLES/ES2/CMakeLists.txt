cmake_minimum_required(VERSION 2.8.4)
project(OpenGLES)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# 头文件目录
include_directories(./Common)

add_subdirectory(Common)
add_subdirectory(Hello_Triangle)
