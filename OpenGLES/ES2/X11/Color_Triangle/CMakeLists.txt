set(SOURCE_FILES color_triangle.c)
# 编译出可执行文件
add_executable(color_triangle ${SOURCE_FILES})

# 添加依赖库
target_link_libraries(color_triangle common)
target_link_libraries(color_triangle glesRender)