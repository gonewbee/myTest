cmake_minimum_required(VERSION 2.8)
project(OGL_Tutorials)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_subdirectory(1_open_a_window)
add_subdirectory(2_hello_dot)
add_subdirectory(3_first_triangle)
add_subdirectory(4_shaders)
add_subdirectory(5_uniform_variables)
add_subdirectory(6_translation_transformation)
add_subdirectory(7_rotation_transformation)
add_subdirectory(8_scaling_transformation)
add_subdirectory(9_interpolation)
add_subdirectory(10_indexed_draws)

