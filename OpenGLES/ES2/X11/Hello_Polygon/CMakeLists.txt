set(SOURCE_FILES hello_polygon.c)
# 编译出可执行文件
add_executable(hello_polygon ${SOURCE_FILES})

# 添加依赖库
target_link_libraries(hello_polygon common)
target_link_libraries(hello_polygon glesRender)
