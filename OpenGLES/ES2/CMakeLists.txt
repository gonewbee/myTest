cmake_minimum_required(VERSION 2.8.4)
project(OpenGLES)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_subdirectory(GLES_Render)

include_directories(GLES_Render)

if (ANDROID)
    add_subdirectory(Android)
else()
    add_subdirectory(X11)
endif ()

